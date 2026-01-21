/* 
 * trans.c - Matrix transpose B = A^T
 *
 * Each transpose function must have a prototype of the form:
 * void trans(int M, int N, int A[N][M], int B[M][N]);
 *
 * A transpose function is evaluated by counting the number of misses
 * on a 1KB direct mapped cache with a block size of 32 bytes.
 */ 
#include <stdio.h>
#include "cachelab.h"



int is_transpose(int M, int N, int A[N][M], int B[M][N]);

/* 
 * transpose_submit - This is the solution transpose function that you
 *     will be graded on for Part B of the assignment. Do not change
 *     the description string "Transpose submission", as the driver
 *     searches for that string to identify the transpose function to
 *     be graded. 
 */

static inline __attribute__((always_inline)) void transpose_submat(int M,int N,int A[N][M],int B[M][N],int ax,int ay,int bx,int by,int xs,int ys){//xs ys based on A
    B[bx + 0][ax + 0] = A[ax + 0][ay + 0];
    B[bx + 1][ax + 0] = A[ax + 0][ay + 1];
    B[bx + 2][ax + 0] = A[ax + 0][ay + 2];
    B[bx + 3][ax + 0] = A[ax + 0][ay + 3];
    B[bx + 4][ax + 0] = A[ax + 0][ay + 4];
    B[bx + 5][ax + 0] = A[ax + 0][ay + 5];
    B[bx + 6][ax + 0] = A[ax + 0][ay + 6];
    B[bx + 7][ax + 0] = A[ax + 0][ay + 7];
    B[bx + 0][ax + 1] = A[ax + 1][ay + 0];
    B[bx + 1][ax + 1] = A[ax + 1][ay + 1];
    B[bx + 2][ax + 1] = A[ax + 1][ay + 2];
    B[bx + 3][ax + 1] = A[ax + 1][ay + 3];
    B[bx + 4][ax + 1] = A[ax + 1][ay + 4];
    B[bx + 5][ax + 1] = A[ax + 1][ay + 5];
    B[bx + 6][ax + 1] = A[ax + 1][ay + 6];
    B[bx + 7][ax + 1] = A[ax + 1][ay + 7];
    B[bx + 0][ax + 2] = A[ax + 2][ay + 0];
    B[bx + 1][ax + 2] = A[ax + 2][ay + 1];
    B[bx + 2][ax + 2] = A[ax + 2][ay + 2];
    B[bx + 3][ax + 2] = A[ax + 2][ay + 3];
    B[bx + 4][ax + 2] = A[ax + 2][ay + 4];
    B[bx + 5][ax + 2] = A[ax + 2][ay + 5];
    B[bx + 6][ax + 2] = A[ax + 2][ay + 6];
    B[bx + 7][ax + 2] = A[ax + 2][ay + 7];
    B[bx + 0][ax + 3] = A[ax + 3][ay + 0];
    B[bx + 1][ax + 3] = A[ax + 3][ay + 1];
    B[bx + 2][ax + 3] = A[ax + 3][ay + 2];
    B[bx + 3][ax + 3] = A[ax + 3][ay + 3];
    B[bx + 4][ax + 3] = A[ax + 3][ay + 4];
    B[bx + 5][ax + 3] = A[ax + 3][ay + 5];
    B[bx + 6][ax + 3] = A[ax + 3][ay + 6];
    B[bx + 7][ax + 3] = A[ax + 3][ay + 7];
    B[bx + 0][ax + 4] = A[ax + 4][ay + 0];
    B[bx + 1][ax + 4] = A[ax + 4][ay + 1];
    B[bx + 2][ax + 4] = A[ax + 4][ay + 2];
    B[bx + 3][ax + 4] = A[ax + 4][ay + 3];
    B[bx + 4][ax + 4] = A[ax + 4][ay + 4];
    B[bx + 5][ax + 4] = A[ax + 4][ay + 5];
    B[bx + 6][ax + 4] = A[ax + 4][ay + 6];
    B[bx + 7][ax + 4] = A[ax + 4][ay + 7];
    B[bx + 0][ax + 5] = A[ax + 5][ay + 0];
    B[bx + 1][ax + 5] = A[ax + 5][ay + 1];
    B[bx + 2][ax + 5] = A[ax + 5][ay + 2];
    B[bx + 3][ax + 5] = A[ax + 5][ay + 3];
    B[bx + 4][ax + 5] = A[ax + 5][ay + 4];
    B[bx + 5][ax + 5] = A[ax + 5][ay + 5];
    B[bx + 6][ax + 5] = A[ax + 5][ay + 6];
    B[bx + 7][ax + 5] = A[ax + 5][ay + 7];
    B[bx + 0][ax + 6] = A[ax + 6][ay + 0];
    B[bx + 1][ax + 6] = A[ax + 6][ay + 1];
    B[bx + 2][ax + 6] = A[ax + 6][ay + 2];
    B[bx + 3][ax + 6] = A[ax + 6][ay + 3];
    B[bx + 4][ax + 6] = A[ax + 6][ay + 4];
    B[bx + 5][ax + 6] = A[ax + 6][ay + 5];
    B[bx + 6][ax + 6] = A[ax + 6][ay + 6];
    B[bx + 7][ax + 6] = A[ax + 6][ay + 7];
    B[bx + 0][ax + 7] = A[ax + 7][ay + 0];
    B[bx + 1][ax + 7] = A[ax + 7][ay + 1];
    B[bx + 2][ax + 7] = A[ax + 7][ay + 2];
    B[bx + 3][ax + 7] = A[ax + 7][ay + 3];
    B[bx + 4][ax + 7] = A[ax + 7][ay + 4];
    B[bx + 5][ax + 7] = A[ax + 7][ay + 5];
    B[bx + 6][ax + 7] = A[ax + 7][ay + 6];
    B[bx + 7][ax + 7] = A[ax + 7][ay + 7];
}

static inline __attribute__((always_inline)) void solve_diag8(int M,int N,int A[N][M],int B[M][N],int s){
    int a0,a1,a2,a3,a4,a5,a6,a7,a8,a9,a10,a11;
    B[s + 0][s + 4] = A[s + 4][s + 0];
    B[s + 0][s + 5] = A[s + 5][s + 0];
    B[s + 0][s + 6] = A[s + 6][s + 0];
    B[s + 0][s + 7] = A[s + 7][s + 0];
    B[s + 1][s + 4] = A[s + 4][s + 1];
    B[s + 1][s + 5] = A[s + 5][s + 1];
    B[s + 1][s + 6] = A[s + 6][s + 1];
    B[s + 1][s + 7] = A[s + 7][s + 1];
    B[s + 2][s + 4] = A[s + 4][s + 2];
    B[s + 2][s + 5] = A[s + 5][s + 2];
    B[s + 2][s + 6] = A[s + 6][s + 2];
    B[s + 2][s + 7] = A[s + 7][s + 2];
    B[s + 3][s + 4] = A[s + 4][s + 3];
    B[s + 3][s + 5] = A[s + 5][s + 3];
    B[s + 3][s + 6] = A[s + 6][s + 3];
    B[s + 3][s + 7] = A[s + 7][s + 3];

    a0 = A[s + 4][s + 4],a1 = A[s + 4][s + 5],a2 = A[s + 4][s + 6],a3 = A[s + 4][s + 7];
    a4 = A[s + 5][s + 4],a5 = A[s + 5][s + 5],a6 = A[s + 5][s + 6],a7 = A[s + 5][s + 7];
    a8 = A[s + 6][s + 4],a9 = A[s + 6][s + 5],a10 = A[s + 6][s + 6],a11 = A[s + 6][s + 7];
    B[s + 4][s + 4] = a0,B[s + 5][s + 4] = a1,B[s + 6][s + 4] = a2;
    B[s + 4][s + 5] = a4,B[s + 5][s + 5] = a5,B[s + 6][s + 5] = a6;
    B[s + 4][s + 6] = a8,B[s + 5][s + 6] = a9,B[s + 6][s + 6] = a10;
    a0 = A[s + 7][s + 4],a1 = A[s + 7][s + 5],a2 = A[s + 7][s + 6],a4 = A[s + 7][s + 7];
    B[s + 7][s + 4] = a3,B[s + 7][s + 5] = a7,B[s + 7][s + 6] = a11;
    B[s + 4][s + 7] = a0,B[s + 5][s + 7] = a1,B[s + 6][s + 7] = a2,B[s + 7][s + 7] = a4;

    B[s + 4][s + 0] = A[s + 0][s + 4];
    B[s + 4][s + 1] = A[s + 1][s + 4];
    B[s + 4][s + 2] = A[s + 2][s + 4];
    B[s + 4][s + 3] = A[s + 3][s + 4];
    B[s + 5][s + 0] = A[s + 0][s + 5];
    B[s + 5][s + 1] = A[s + 1][s + 5];
    B[s + 5][s + 2] = A[s + 2][s + 5];
    B[s + 5][s + 3] = A[s + 3][s + 5];
    B[s + 6][s + 0] = A[s + 0][s + 6];
    B[s + 6][s + 1] = A[s + 1][s + 6];
    B[s + 6][s + 2] = A[s + 2][s + 6];
    B[s + 6][s + 3] = A[s + 3][s + 6];
    B[s + 7][s + 0] = A[s + 0][s + 7];
    B[s + 7][s + 1] = A[s + 1][s + 7];
    B[s + 7][s + 2] = A[s + 2][s + 7];
    B[s + 7][s + 3] = A[s + 3][s + 7];

    a0 = A[s][s],a1 = A[s][s + 1],a2 = A[s][s + 2],a3 = A[s][s + 3];
    a4 = A[s + 1][s],a5 = A[s + 1][s + 1],a6 = A[s + 1][s + 2],a7 = A[s + 1][s + 3];
    a8 = A[s + 2][s],a9 = A[s + 2][s + 1],a10 = A[s + 2][s + 2],a11 = A[s + 2][s + 3];
    B[s][s] = a0,B[s + 1][s] = a1,B[s + 2][s] = a2;
    B[s][s + 1] = a4,B[s + 1][s + 1] = a5,B[s + 2][s + 1] = a6;
    B[s][s + 2] = a8,B[s + 1][s + 2] = a9,B[s + 2][s + 2] = a10;
    a0 = A[s + 3][s],a1 = A[s + 3][s + 1],a2 = A[s + 3][s + 2],a4 = A[s + 3][s + 3];
    B[s + 3][s] = a3,B[s + 3][s + 1] = a7,B[s + 3][s + 2] = a11;
    B[s][s + 3] = a0,B[s + 1][s + 3] = a1,B[s + 2][s + 3] = a2,B[s + 3][s + 3] = a4;
}

static inline __attribute__((always_inline)) void solve32times32(int M,int N,int A[N][M],int B[M][N]){
    transpose_submat(M,N,A,B,0,8,8,0,8,8);
    transpose_submat(M,N,A,B,0,16,16,0,8,8);
    transpose_submat(M,N,A,B,0,24,24,0,8,8);
    transpose_submat(M,N,A,B,8,0,0,8,8,8);
    transpose_submat(M,N,A,B,8,16,16,8,8,8);
    transpose_submat(M,N,A,B,8,24,24,8,8,8);
    transpose_submat(M,N,A,B,16,0,0,16,8,8);
    transpose_submat(M,N,A,B,16,8,8,16,8,8);
    transpose_submat(M,N,A,B,16,24,24,16,8,8);
    transpose_submat(M,N,A,B,24,0,0,24,8,8);
    transpose_submat(M,N,A,B,24,8,8,24,8,8);
    transpose_submat(M,N,A,B,24,16,16,24,8,8);
    // for (i = 0;i < 4;i++){
    //     for (j = 0;j < 4;j++){
    //         if (i == j) continue;
    //         transpose_submat(M,N,A,B,i * 8,j * 8,j * 8,i * 8,8,8);
    //     }
    // }
    solve_diag8(M,N,A,B,0);
    solve_diag8(M,N,A,B,8);
    solve_diag8(M,N,A,B,16);
    solve_diag8(M,N,A,B,24);
}

static inline __attribute__((always_inline)) void solve64times64(int M,int N,int A[N][M],int B[M][N]){
    transpose_submat(M,N,A,B,0,8,8,0,8,8);
    transpose_submat(M,N,A,B,0,16,16,0,8,8);
    transpose_submat(M,N,A,B,0,24,24,0,8,8);
    transpose_submat(M,N,A,B,0,32,32,0,8,8);
    transpose_submat(M,N,A,B,0,40,40,0,8,8);
    transpose_submat(M,N,A,B,0,48,48,0,8,8);
    transpose_submat(M,N,A,B,0,56,56,0,8,8);
    transpose_submat(M,N,A,B,8,0,0,8,8,8);
    transpose_submat(M,N,A,B,8,16,16,8,8,8);
    transpose_submat(M,N,A,B,8,24,24,8,8,8);
    transpose_submat(M,N,A,B,8,32,32,8,8,8);
    transpose_submat(M,N,A,B,8,40,40,8,8,8);
    transpose_submat(M,N,A,B,8,48,48,8,8,8);
    transpose_submat(M,N,A,B,8,56,56,8,8,8);
    transpose_submat(M,N,A,B,16,0,0,16,8,8);
    transpose_submat(M,N,A,B,16,8,8,16,8,8);
    transpose_submat(M,N,A,B,16,24,24,16,8,8);
    transpose_submat(M,N,A,B,16,32,32,16,8,8);
    transpose_submat(M,N,A,B,16,40,40,16,8,8);
    transpose_submat(M,N,A,B,16,48,48,16,8,8);
    transpose_submat(M,N,A,B,16,56,56,16,8,8);
    transpose_submat(M,N,A,B,24,0,0,24,8,8);
    transpose_submat(M,N,A,B,24,8,8,24,8,8);
    transpose_submat(M,N,A,B,24,16,16,24,8,8);
    transpose_submat(M,N,A,B,24,32,32,24,8,8);
    transpose_submat(M,N,A,B,24,40,40,24,8,8);
    transpose_submat(M,N,A,B,24,48,48,24,8,8);
    transpose_submat(M,N,A,B,24,56,56,24,8,8);
    transpose_submat(M,N,A,B,32,0,0,32,8,8);
    transpose_submat(M,N,A,B,32,8,8,32,8,8);
    transpose_submat(M,N,A,B,32,16,16,32,8,8);
    transpose_submat(M,N,A,B,32,24,24,32,8,8);
    transpose_submat(M,N,A,B,32,40,40,32,8,8);
    transpose_submat(M,N,A,B,32,48,48,32,8,8);
    transpose_submat(M,N,A,B,32,56,56,32,8,8);
    transpose_submat(M,N,A,B,40,0,0,40,8,8);
    transpose_submat(M,N,A,B,40,8,8,40,8,8);
    transpose_submat(M,N,A,B,40,16,16,40,8,8);
    transpose_submat(M,N,A,B,40,24,24,40,8,8);
    transpose_submat(M,N,A,B,40,32,32,40,8,8);
    transpose_submat(M,N,A,B,40,48,48,40,8,8);
    transpose_submat(M,N,A,B,40,56,56,40,8,8);
    transpose_submat(M,N,A,B,48,0,0,48,8,8);
    transpose_submat(M,N,A,B,48,8,8,48,8,8);
    transpose_submat(M,N,A,B,48,16,16,48,8,8);
    transpose_submat(M,N,A,B,48,24,24,48,8,8);
    transpose_submat(M,N,A,B,48,32,32,48,8,8);
    transpose_submat(M,N,A,B,48,40,40,48,8,8);
    transpose_submat(M,N,A,B,48,56,56,48,8,8);
    transpose_submat(M,N,A,B,56,0,0,56,8,8);
    transpose_submat(M,N,A,B,56,8,8,56,8,8);
    transpose_submat(M,N,A,B,56,16,16,56,8,8);
    transpose_submat(M,N,A,B,56,24,24,56,8,8);
    transpose_submat(M,N,A,B,56,32,32,56,8,8);
    transpose_submat(M,N,A,B,56,40,40,56,8,8);
    transpose_submat(M,N,A,B,56,48,48,56,8,8);
    for (int i = 0;i < 8;i++) solve_diag8(M,N,A,B,i * 8);
}

void solve61times67(int M,int N,int A[N][M],int B[M][N]){

}

char transpose_submit_desc[] = "Transpose submission";
void transpose_submit(int M, int N, int A[N][M], int B[M][N])
{
    // fprintf(stderr,"Address of A[][]:%p\tAddress of B[][]:%p\n",A,B);
    if (M == 32 && N == 32){
        solve32times32(M,N,A,B);
       return ;
    }
    if (M == 64 && N == 64){
        solve64times64(M,N,A,B);
    }
}

/* 
 * You can define additional transpose functions below. We've defined
 * a simple one below to help you get started. 
 */ 

/* 
 * trans - A simple baseline transpose function, not optimized for the cache.
 */
char trans_desc[] = "Simple row-wise scan transpose";
void trans(int M, int N, int A[N][M], int B[M][N])
{
    int i, j, tmp;

    for (i = 0; i < N; i++) {
        for (j = 0; j < M; j++) {
            tmp = A[i][j];
            B[j][i] = tmp;
        }
    }    

}

char show_addr_desc[] = "Just to show the address";
void show_addr(int M,int N,int A[N][M],int B[M][N]){
    A[0][0] = B[0][0];
    A[N - 1][M - 1] = B[M - 1][N - 1];
}

/*
 * registerFunctions - This function registers your transpose
 *     functions with the driver.  At runtime, the driver will
 *     evaluate each of the registered functions and summarize their
 *     performance. This is a handy way to experiment with different
 *     transpose strategies.
 */
void registerFunctions()
{
    /* Register your solution function */
    registerTransFunction(transpose_submit, transpose_submit_desc); 

    /* Register any additional transpose functions */
    registerTransFunction(trans, trans_desc); 

    registerTransFunction(show_addr,show_addr_desc);

}

/* 
 * is_transpose - This helper function checks if B is the transpose of
 *     A. You can check the correctness of your transpose by calling
 *     it before returning from the transpose function.
 */
int is_transpose(int M, int N, int A[N][M], int B[M][N])
{
    int i, j;

    for (i = 0; i < N; i++) {
        for (j = 0; j < M; ++j) {
            if (A[i][j] != B[j][i]) {
                return 0;
            }
        }
    }
    return 1;
}

