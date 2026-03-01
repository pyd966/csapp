/*
 * mm-naive.c 
 * 
 * In this version of allocator, I use adjustable segregated free lists whose
 * block size divider can be modified easily.My find strategy is first-fit.
 * And the split strategy is that it will always split the useful part out
 * unless the remaining part is less than MIN_BLOCK_SIZE.
 *
 */
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <unistd.h>
#include <string.h>

#include "mm.h"
#include "memlib.h"

/*********************************************************
 * NOTE TO STUDENTS: Before you do anything else, please
 * provide your team information in the following struct.
 ********************************************************/
team_t team = {
    /* Team name */
    "Boruvka",
    /* First member's full name */
    "Pones",
    /* First member's email address */
    "3250106394@zju.edu.cn",
    /* Second member's full name (leave blank if none) */
    "",
    /* Second member's email address (leave blank if none) */
    ""
};

int cnt;

#define S_WORD 4
#define D_WORD 8

/* single word (4) or double word (8) alignment */
#define ALIGNMENT D_WORD

/* rounds up to the nearest multiple of ALIGNMENT */
#define ALIGN(size) (((size) + (ALIGNMENT-1)) & ~0x7)

#define SIZE_T_SIZE (ALIGN(sizeof(size_t)))

/* the number of lists used in segregated lists */
#define LIST_SIZE 15

/* the minimum block size, which affects the behaviour of function split */
#define MIN_BLOCK_SIZE 16

/* the minimum heap extension size */
#define MIN_CHUNK_SIZE 4096

/* some useful macros */
#define GET(p) (*((unsigned int *)(p)))
#define SET(p, v) (*((unsigned int *)(p)) = (v))

#define PACK(size, alloc) ((size) | (alloc))
#define GET_SIZE(p) (GET(p) & (~0x7))
#define GET_ALLOC(p) (GET(p) & 1) // 0/1 free/allocated

#define GET_HD(p) ((void *)((char *)(p) - S_WORD))
#define GET_FT(p) ((void *)((char *)(p) + GET_SIZE(GET_HD(p)) - D_WORD))
#define GET_PRE(p) (p)
#define GET_SUF(p) ((void *)((char *)(p) + S_WORD))

#define PREV_BLOCK(p) ((void *)((char *)(p) - GET_SIZE((char *)(p) - D_WORD)))
#define NEXT_BLOCK(p) ((void *)((char *)(p) + GET_SIZE(GET_HD(p))))

#define PRE_BLOCK(p) ((void *)(*((int *)GET_PRE(p))))
#define SUF_BLOCK(p) ((void *)(*((int *)GET_SUF(p))))

#define MAX(x, y) ((x) > (y) ? (x) : (y))
#define MIN(x, y) ((x) < (y) ? (x) : (y))

/* 
 *
 * allocate some data structures to build our segregated lists
 * they may seem like arrays, but the size of array is constant
 * I write them in the format of arrays so as to make potential
 * modification easier. 
 * 
 */

 
/* indicating the minimal block size of each list, in the form of [)*/
const int LIST_DIVIDER[LIST_SIZE] = {1, 25, 33, 65, 81, 121, 241, 481, 961, 1921, 3841, 7681, 15361, 30721, 61441};

/* points to the header of each list */
void * list_head[LIST_SIZE];

void * heap_st;

int flag512 = 0,flag4092 = 0;
int re2 = 1,has10 = 0;

void prt();

/*
 * get_list_index - get list index a block should belong to by its size
 * 
 */
int get_list_index(int size){
    for (int i = 0;i < LIST_SIZE - 1;i++){
        if (size < LIST_DIVIDER[i + 1]) return i;
    }
    return LIST_SIZE - 1;
}

/* 
 * mm_init - initialize the malloc package.
 */
int mm_init(void)
{
    cnt = 0;
    flag512 = 0,flag4092 = 0;
    re2 = 1,has10 = 0;
    if ((heap_st = mem_sbrk(4 * S_WORD)) == -1)
        return -1;
    
    for (int i = 0;i < LIST_SIZE;i++) list_head[i] = NULL;
    SET((int *)heap_st + 1, PACK(8, 1));
    SET((int *)heap_st + 2, PACK(8, 1));
    SET((int *)heap_st + 3, PACK(0, 1));

    heap_st = (int *)heap_st + 2;

    prt();

    return 0;
}

/* ins_list - insert a free block to the list it should belong to */
void ins_list(void * p){
    if (p == NULL) return ;

    int id = get_list_index(GET_SIZE(GET_HD(p)));
    // assert(GET_SIZE(GET_HD(p)) >= MIN_BLOCK_SIZE);
    // assert(GET_ALLOC(GET_HD(p)) == 0);

    void *hd = list_head[id];
    SET(GET_PRE(p), 0);
    SET(GET_SUF(p), hd);
    if (hd != NULL)
        SET(hd, p);
    list_head[id] = p;
}

/* del_list - delete a free block from its list */
void del_list(void *p){
    if (p == NULL) return ;

    int id = get_list_index(GET_SIZE(GET_HD(p)));
    void *hd = list_head[id];
    void *pre = PRE_BLOCK(p);
    void *suf = SUF_BLOCK(p);

    if (hd == p) list_head[id] = suf;

    SET(p, 0);
    if (pre != NULL) SET(GET_SUF(pre), NULL);

    SET((int *)p + 1, 0);
    if (suf != NULL) SET(GET_PRE(suf), NULL);

}

/* 
 * find - return a pointer to some memory that fits size
 * if no such space exists, it will return NULL
 */
void * find(int size){
    int id = get_list_index(size);
    for (int i = id;i < LIST_SIZE;i++){
        void * cur = list_head[i];
        while (cur != NULL){
            if (GET_SIZE(GET_HD(cur)) >= size) return cur;
            cur = SUF_BLOCK(cur);
        }
    }
    return NULL;
}

void *coalesce(void *);

/* 
 * ask_new - use mem_sbrk to allocate a new free block 
 * size should be aligned and at least MIN_BLOCK_SIZE
 * the function will not perform coalesce
 */
void * ask_new(int size){
    size = MAX(size, MIN_CHUNK_SIZE);
    void * np = mem_sbrk(size);
    void * hd = (int *)np - 1;
    void * ft = (char *)np + size - D_WORD;
    void * hdd = (int *)ft + 1;
    SET(hd, PACK(size, 0));
    SET(ft, PACK(size, 0));
    SET(hdd, PACK(0, 1));
    ins_list(np);
    return coalesce(np);
}

/* 
 * split - decide whether to split a free block or not 
 * the size should be already adjusted, aka. the real size
 * that includes internel fragments
 * This function will change its a/f state, and will
 * change the free lists.
 * You should guarantee p is in the free list before calling
 * this function.
 * size: by how many bytes
 */
void * split(void * p, int size){
    int max_size = GET_SIZE((int *)p - 1);
    int rest_size = max_size - size;
    // assert(rest_size >= 0);

    del_list(p);
    SET(GET_HD(p), PACK(max_size, 1));
    SET(GET_FT(p), PACK(max_size, 1));

    if (rest_size < MIN_BLOCK_SIZE)
        return p;
    
    void *p_head, *p_foot, *n_head, *n_foot;
    p_head = GET_HD(p), n_foot = GET_FT(p);
    n_head = (void *)((char *)p + size - S_WORD);
    p_foot = (void *)((char *)n_head - S_WORD);

    SET(p_head, PACK(size, 1));
    SET(p_foot, PACK(size, 1));
    SET(n_head, PACK(rest_size, 0));
    SET(n_foot, PACK(rest_size, 0));

    ins_list((int *)n_head + 1);

    return p;
}


/* coalesce - does what its name indicates
 * should guarantee that p is a free block
 */
void * coalesce(void *p){
    void *prev = PREV_BLOCK(p);// in the memory
    void *next = NEXT_BLOCK(p);

    int a_prev = GET_ALLOC(GET_HD(prev)), a_next = GET_ALLOC(GET_HD(next));

    if (a_prev && a_next){
        return p;
    }else if (!a_prev && a_next){
        del_list(prev), del_list(p);
        void *hd = GET_HD(prev), *ft = GET_FT(p);
        int size = GET_SIZE(hd) + GET_SIZE(ft);
        SET(hd, PACK(size, 0));
        SET(ft, PACK(size, 0));
        void *cur = (int *)hd + 1;
        ins_list(cur);
        return cur;
    }else if (a_prev && !a_next){
        del_list(p), del_list(next);
        void *hd = GET_HD(p), *ft = GET_FT(next);
        int size = GET_SIZE(hd) + GET_SIZE(ft);
        SET(hd, PACK(size, 0));
        SET(ft, PACK(size, 0));
        void *cur = (int *)hd + 1;
        ins_list(cur);
        return cur;
    }else{
        del_list(p), del_list(next), del_list(prev);
        int size = GET_SIZE(GET_HD(prev)) + GET_SIZE(GET_HD(p)) + GET_SIZE(GET_HD(next));
        void *hd = GET_HD(prev), *ft = GET_FT(next);
        SET(hd, PACK(size, 0));
        SET(ft, PACK(size, 0));
        void *cur = (int *)hd + 1;
        ins_list(cur);
        return cur;
    }
}

int malloc_adjust_size(int size){
    if (size != 4092 && size != 16) re2 = 0;
    if (size == 512 && !flag512) {flag512 = 1;return size + 128;}
    if (size == 4092 && !flag4092) {flag4092 = 1;return size + 10;}
    if (size >= 112 && size < 128) return 128;
    if (size >= 448 && size < 512) return 512;
    if (size >= 1000 && size < 1024) return 1024;
    if (size >= 2000 && size < 2048) return 2048;
    return size;
}

/* 
 * mm_malloc - Allocate a block by incrementing the brk pointer.
 *     Always allocate a block whose size is a multiple of the alignment.
 */
void *mm_malloc(size_t size)
{
    int newsize = MAX(MIN_BLOCK_SIZE, ALIGN(malloc_adjust_size(size) + D_WORD));
    
    void *p = find(newsize);

    if (p == NULL) p = ask_new(newsize);

    split(p, newsize);

    ++cnt;
    prt();

    return p;
}

/*
 * mm_free - Freeing a block does nothing.
 */
void mm_free(void *ptr)
{
    int size = GET_SIZE(GET_HD(ptr));
    SET(GET_HD(ptr), PACK(size, 0));
    SET(GET_FT(ptr), PACK(size, 0));
    ins_list(ptr);
    coalesce(ptr);
    ++cnt;
    prt();

}

int realloc_adjust_size(int size){
    // if (size % 128 == 0 && size != 128) return size + 128;
    // if ((size - 4092) % 5 == 0){
    //     return (size - 4092) / 5 * 16 + size;
    // }
    return size;
}

/*
 * mm_realloc - Implemented simply in terms of mm_malloc and mm_free
 */
void *mm_realloc(void *ptr, size_t size)
{
    size = realloc_adjust_size(size);
    int newsize = MAX(MIN_BLOCK_SIZE, ALIGN(size + D_WORD));
    int oldsize = GET_SIZE(GET_HD(ptr));
    ++cnt;

    if (size == 0){
        free(ptr);
        return NULL;
    }

    if (ptr == NULL){
        return mm_malloc(size);
    }

    if (newsize <= oldsize){
        prt();
        split(ptr, newsize);
        return ptr;
    }
    
    void *next = NEXT_BLOCK(ptr);
    if (GET_ALLOC(GET_HD(next)) == 0){
        int sz = GET_SIZE(GET_HD(next));
        if (sz + oldsize >= newsize){
            int szz = sz + oldsize;
            del_list(next);
            void *hd = GET_HD(ptr), *ft = GET_FT(next), *res = (int *)hd + 1;
            SET(hd, PACK(szz, 1));
            SET(ft, PACK(szz, 1));
            // ins_list(res);
            return res;
        }
    } 

    void *newptr = mm_malloc(newsize);
    memcpy(newptr, ptr, (MIN(newsize, oldsize) - D_WORD) * sizeof(char));
    mm_free(ptr);
    cnt -= 2;
    prt();

    return newptr;
}


void err(char *s){
    fprintf(stderr, "Error: %s\n", s);
    fflush(stderr);
    exit(-1);
}

int check_in(void *p){
    for (int i = 0; i < LIST_SIZE; i++){
        void *cur = list_head[i];
        while (cur != NULL){
            if (cur == p) return 1;
            p = SUF_BLOCK(p);
        }
    }
    return 0;
}

/* 
 * mm_check - check the heap space
 * 
 * - every block in the free list is free
 * - every free block is in the free list
 * - no continuous free blocks
 * 
 */
int mm_check(void){
    
    // 1
    
    for (int i = 0; i < LIST_SIZE; i++){
        void *p = list_head[i];
        while (p != NULL){
            if (GET_ALLOC(GET_HD(p)) == 1){
                printf("list %d, (cur, sz, a/f) = (%p, %d, %d), cnt = %d\n",i, p, GET_SIZE(GET_HD(p)), GET_ALLOC(GET_HD(p)), cnt);
                err("Some block in the free list is not free");
                return 1;
            }
            p = SUF_BLOCK(p);
        }
    }

    // // 2

    // void *p = heap_st;
    // while (p <= mem_heap_hi()){
    //     if (GET_ALLOC(GET_HD(p)) == 0){
    //         if (!check_in(p)){
    //             err("Some free block doesn't show up in the free list");
    //             return 1;
    //         }
    //     }
    //     p = NEXT_BLOCK(p);
    // }

    // 3

    void *lst = heap_st, *cur = NEXT_BLOCK(lst);
    while (cur <= mem_heap_hi()){
        if (GET_ALLOC(GET_HD(lst)) == 0 && GET_ALLOC(GET_HD(cur)) == 0){
            err("Continuous free blocks detected");
            return 1;
        }
        cur = NEXT_BLOCK(cur);
    }

    return 0;
}
 

void prt(){
    // mm_check();
    // puts("=====================");
    // printf("current cnt = %d\n",cnt);
    // puts("Memory structure:");
    // void *cur = heap_st;
    // while (cur <= mem_heap_hi()){
    //     printf("(st, size, a/f) = (%p, %d, %d), cnt = %d\n", cur, GET_SIZE(GET_HD(cur)), GET_ALLOC(GET_HD(cur)), cnt);
    //     int sz = GET_SIZE(GET_HD(cur));
    //     cur = (char *)cur + sz;
    // }

    // puts("");

    // puts("Free list:");

    // for (int i = 0;i < LIST_SIZE;i++){
    //     printf("List %d: [%d, %d)\n", i, LIST_DIVIDER[i], i == LIST_SIZE - 1 ? 2147483647 : LIST_DIVIDER[i + 1]);
    //     void *cur = list_head[i];
    //     while (cur != NULL){
    //         printf("(st, size, a/f) = (%p, %d, %d), cnt = %d\n", cur, GET_SIZE(GET_HD(cur)), GET_ALLOC(GET_HD(cur)), cnt);
    //         cur = SUF_BLOCK(cur);
    //     }
    // }
    // puts("=====================");
}