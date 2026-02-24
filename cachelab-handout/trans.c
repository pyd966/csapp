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
    for (int i = 0;i < xs;i++){
        for (int j = 0;j < ys;j++){
            B[bx + i][ax + j] = A[ax + j][ay + i];
        }
    }
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

void solve88_6464(int M,int N,int A[N][M],int B[M][N],int ax,int ay,int bx,int by){
    for (int i = 0;i < 4;i++){
        for (int j = 0;j < 4;j++){
            B[bx + j][by + i] = A[ax + i][ay + j];
        }
    }
    int p0,p1,p2,p3,p4,p5,p6,p7,p8,p9,p10,p11;
    p0 = A[ax][ay + 4],p1 = A[ax][ay + 5],p2 = A[ax][ay + 6],p3 = A[ax][ay + 7];
    p4 = A[ax + 1][ay + 4],p5 = A[ax + 1][ay + 5],p6 = A[ax + 1][ay + 6],p7 = A[ax + 1][ay + 7];
    p8 = A[ax + 2][ay + 4],p9 = A[ax + 2][ay + 5],p10 = A[ax + 2][ay + 6],p11 = A[ax + 2][ay + 7];
    for (int i = 4;i < 7;i++){
        for (int j = 0;j < 4;j++) B[bx + j][by + i] = A[ax + i][ay + j];
    }
    B[bx + 4][by] = p0,B[bx + 4][by + 1] = p4,B[bx + 4][by + 2] = p8;
    B[bx + 5][by] = p1,B[bx + 5][by + 1] = p5,B[bx + 5][by + 2] = p9;
    p0 = A[ax + 3][ay + 4],p1 = A[ax + 3][ay + 5],p4 = A[ax + 3][ay + 6],p5 = A[ax + 3][ay + 7];
    B[bx + 2][by + 7] = A[ax + 7][ay + 2],B[bx + 3][by + 7] = A[ax + 7][ay + 3];
    B[bx + 4][by + 3] = p0,B[bx + 5][by + 3] = p1;
    B[bx + 6][by] = p2,B[bx + 6][by + 1] = p6,B[bx + 6][by + 2] = p10,B[bx + 6][by + 3] = p4;
    B[bx + 7][by] = p3,B[bx + 7][by + 1] = p7,B[bx + 7][by + 2] = p11,B[bx + 7][by + 3] = p5;
    for (int i = 4;i < 8;i++){
        for (int j = 4;j < 8;j++){
            B[bx + j][by + i] = A[ax + i][ay + j];
        }
    }
    B[bx][by + 7] = A[ax + 7][ay],B[bx + 1][by + 7] = A[ax + 7][ay + 1];
}

void solved8_6464(int M,int N,int A[N][M],int B[M][N],int s){
    int a0,a1,a2,a3,a4,a5,a6,a7,a8,a9,a10,a11;
    B[s+2][s+0] = A[s+0][s+2],B[s+2][s+1] = A[s+1][s+2],B[s+3][s+0] = A[s+0][s+3],B[s+3][s+1] = A[s+1][s+3];
    a0 = A[s+0][s+0],a1 = A[s+0][s+1],a2 = A[s+1][s+0],a3 = A[s+1][s+1];
    a4 = A[s+0][s+4],a5 = A[s+0][s+5],a6 = A[s+1][s+4],a7 = A[s+1][s+5];
    a8 = A[s+0][s+6],a9 = A[s+0][s+7],a10 = A[s+1][s+6],a11 = A[s+1][s+7];
    B[s+4][s+0] = a4,B[s+4][s+1] = a6,B[s+5][s+0] = a5,B[s+5][s+1] = a7;
    B[s+4][s+2] = A[s+2][s+4],B[s+4][s+3] = A[s+3][s+4],B[s+5][s+2] = A[s+2][s+5],B[s+5][s+3] = A[s+3][s+5];
    a4 = A[s+2][s+0],a5 = A[s+2][s+1],a6 = A[s+3][s+0],a7 = A[s+3][s+1];
    B[s+0][s+0] = a0,B[s+1][s+0] = a1,B[s+0][s+1] = a2,B[s+1][s+1] = a3;
    B[s+0][s+2] = a4,B[s+1][s+2] = a5,B[s+0][s+3] = a6,B[s+1][s+3] = a7;
    B[s+0][s+6] = A[s+6][s+0],B[s+0][s+7] = A[s+7][s+0],B[s+1][s+6] = A[s+6][s+1],B[s+1][s+7] = A[s+7][s+1];
    a0 = A[s+6][s+4],a1 = A[s+6][s+5],a2 = A[s+7][s+4],a3 = A[s+7][s+5];
    a4 = A[s+6][s+6],a5 = A[s+6][s+7],a6 = A[s+7][s+6],a7 = A[s+7][s+7];
    B[s+6][s+4] = A[s+4][s+6],B[s+6][s+5] = A[s+5][s+6],B[s+7][s+4] = A[s+4][s+7],B[s+7][s+5] = A[s+5][s+7];
    B[s+6][s+0] = a8,B[s+6][s+1] = a10,B[s+7][s+0] = a9,B[s+7][s+1] = a11;
    B[s+6][s+6] = a4,B[s+6][s+7] = a6,B[s+7][s+6] = a5,B[s+7][s+7] = a7;
    a4 = A[s+4][s+4],a5 = A[s+4][s+5],a6 = A[s+5][s+4],a7 = A[s+5][s+5];
    B[s+4][s+4] = a4,B[s+5][s+4] = a5,B[s+4][s+5] = a6,B[s+5][s+5] = a7;
    B[s+4][s+6] = a0,B[s+5][s+6] = a1,B[s+4][s+7] = a2,B[s+5][s+7] = a3;
    a0 = A[s+2][s+2],a1 = A[s+2][s+3],a2 = A[s+3][s+2],a3 = A[s+3][s+3];
    a4 = A[s+2][s+6],a5 = A[s+2][s+7],a6 = A[s+3][s+6],a7 = A[s+3][s+7];
    B[s+6][s+2] = a4,B[s+6][s+3] = a6,B[s+7][s+2] = a5,B[s+7][s+3] = a7;
    a4 = A[s+4][s+0],a5 = A[s+4][s+1],a6 = A[s+5][s+0],a7 = A[s+5][s+1];
    a8 = A[s+4][s+2],a9 = A[s+4][s+3],a10 = A[s+5][s+2],a11 = A[s+5][s+3];
    B[s+0][s+4] = a4,B[s+0][s+5] = a6,B[s+1][s+4] = a5,B[s+1][s+5] = a7;
    a4 = A[s+6][s+2],a5 = A[s+6][s+3],a6 = A[s+7][s+2],a7 = A[s+7][s+3];
    B[s+2][s+2] = a0,B[s+3][s+2] = a1,B[s+2][s+3] = a2,B[s+3][s+3] = a3;
    B[s+2][s+4] = a8,B[s+2][s+5] = a10,B[s+3][s+4] = a9,B[s+3][s+5] = a11;
    B[s+2][s+6] = a4,B[s+2][s+7] = a6,B[s+3][s+6] = a5,B[s+3][s+7] = a7;
    // for (int i=0;i<=1;i++) for (int j=0;j<=7;j++) B[s+i][s+j] = A[s+j][s+i];
    
    // for (int i=2;i<=3;i++) for (int j=0;j<=1;j++) B[s+i][s+j] = A[s+j][s+i];
    // for (int i=2;i<=3;i++) for (int j=2;j<=3;j++) B[s+i][s+j] = A[s+j][s+i];
    // for (int i=2;i<=3;i++) for (int j=4;j<=5;j++) B[s+i][s+j] = A[s+j][s+i];
    // for (int i=2;i<=3;i++) for (int j=6;j<=7;j++) B[s+i][s+j] = A[s+j][s+i];

    // for (int i=4;i<=5;i++) for (int j=0;j<=1;j++) B[s+i][s+j] = A[s+j][s+i];
    // for (int i=4;i<=5;i++) for (int j=2;j<=3;j++) B[s+i][s+j] = A[s+j][s+i];
    // for (int i=4;i<=5;i++) for (int j=4;j<=5;j++) B[s+i][s+j] = A[s+j][s+i];
    // for (int i=4;i<=5;i++) for (int j=6;j<=7;j++) B[s+i][s+j] = A[s+j][s+i];

    // for (int i=6;i<=7;i++) for (int j=0;j<=7;j++) B[s+i][s+j] = A[s+j][s+i];
}

static inline __attribute__((always_inline)) void solve64times64(int M,int N,int A[N][M],int B[M][N]){
    for (int i = 0;i < 8;i++){
        for (int j = 0;j < 8;j++){
            if (i == j) continue;
            solve88_6464(M,N,A,B,i * 8,j * 8,j * 8,i * 8);
        }
    }
    for (int i = 0;i < 8;i++) solved8_6464(M,N,A,B,i * 8);
}

inline int getlineid(int st,int x,int y,int M,int N){

}

void transpose_line(int M,int N,int A[N][M],int B[M][N],int s){
    int a0,a1,a2,a3,a4,a5,a6,a7,a8,a9,a10,a11;
    a0 = A[64][s],a1 = A[65][s],a2 = A[66][s];
    a3 = A[64][s + 1],a4 = A[65][s + 1],a5 = A[66][s + 1];
    a6 = A[64][s + 2],a7 = A[65][s + 2],a8 = A[66][s + 2];
    a9 = A[64][s + 3],a10 = A[65][s + 3],a11 = A[66][s + 3];
    for (int j = 0;j < 8;j++) B[s][j] = A[j][s];
    B[s][64] = a0,B[s][65] = a1,B[s][66] = a2;
    for (int j = 0;j < 8;j++) B[s+1][j] = A[j][s+1];
    B[s+1][64] = a3,B[s+1][65] = a4,B[s+1][66] = a5;
    for (int j = 0;j < 8;j++) B[s+2][j] = A[j][s+2];
    B[s+2][64] = a6,B[s+2][65] = a7,B[s+2][66] = a8;
    for (int j = 0;j < 8;j++) B[s+3][j] = A[j][s+3];
    B[s+3][64] = a9,B[s+3][65] = a10,B[s+3][66] = a11;
    for (int j = 0;j < 8;j++) B[s+4][j] = A[j][s+4];

    a0 = A[64][s+4],a1 = A[65][s+4],a2 = A[66][s+4];
    a3 = A[64][s+5],a4 = A[65][s+5],a5 = A[66][s+5];
    a6 = A[64][s+6],a7 = A[65][s+6],a8 = A[66][s+6];
    a9 = A[64][s+7],a10 = A[65][s+7],a11 = A[66][s+7];

    B[s+4][64] = a0,B[s+4][65] = a1,B[s+4][66] = a2;
    for (int j = 0;j < 8;j++) B[s+5][j] = A[j][s+5];
    B[s+5][64] = a3,B[s+5][65] = a4,B[s+5][66] = a5;
    for (int j = 0;j < 8;j++) B[s+6][j] = A[j][s+6];
    B[s+6][64] = a6,B[s+6][65] = a7,B[s+6][66] = a8;
    for (int j = 0;j < 8;j++) B[s+7][j] = A[j][s+7];
    B[s+7][64] = a9,B[s+7][65] = a10,B[s+7][66] = a11;
    
    transpose_submat(M,N,A,B,0,s,s,0,8,8);
    for (int i = 0;i < 8;i++){
        for (int j = 64;j < 67;j++) B[s + i][j] = A[j][s + i];
    }

    for (int i = 1;i < 8;i++){
        transpose_submat(M,N,A,B,i * 8,s,s,i * 8,8,8);
    }

    
}

int mymin(int x,int y) {return x < y ? x : y;}

void solve61times67(int M,int N,int A[N][M],int B[M][N]){
    int PM = 17,PN = 17;
    // for (int i = 0;i < 7;i++){
    //     transpose_line(M,N,A,B,i * 8);
    //     for (int ii = 0;ii < 8;ii++){
    //         for (int j = 64;j < 67;j++) B[i * 8 + ii][j] = A[j][i * 8 + ii];
    //     }
    // }
    // for (int i = 0;i < 7;i++) solve_diag8(M,N,A,B,i * 8);
    // for (int i = 56;i < 61;i++){
    //     for (int j = 0;j < 67;j++) B[i][j] = A[j][i];
    // }
    // for (int i = 0;i < N;i++){
    //     for (int j = 0;j < M;j++){
    //         B[j][i] = A[i][j];
    //     }
    // }
    for (int i = 0;i < M;i += PM){
        for (int j = 0;j < N;j += PN){
            transpose_submat(M,N,A,B,j,i,i,j,mymin(PM,M - i),mymin(PN,N - j));
        }
    }
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
    if (M == 61 && N == 67){
        solve61times67(M,N,A,B);
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
    for (i = 0;i < 50;i++) tmp = A[0][0];
    for (i = 0;i < 50;i++) tmp = B[0][0];
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
                fprintf(stderr,"Error: A[%d][%d] != B[%d][%d], where A[%d][%d]=%d, B[%d][%d]=%d\n",i,j,j,i,i,j,A[i][j],j,i,B[j][i]);
                return 0;
            }
        }
    }
    return 1;
}

