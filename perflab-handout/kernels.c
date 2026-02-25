/********************************************************
 * Kernels to be optimized for the CS:APP Performance Lab
 ********************************************************/

#include <stdio.h>
#include <stdlib.h>
#include "defs.h"
#include <assert.h>

/* 
 * Please fill in the following team struct 
 */
team_t team = {
    "Boruvka",              /* Team name */

    "PYD1",     /* First member full name */
    "3250106394@zju.edu.cn",  /* First member email address */

    "",                   /* Second member full name (leave blank if none) */
    ""                    /* Second member email addr (leave blank if none) */
};

/***************
 * ROTATE KERNEL
 ***************/

/******************************************************
 * Your different versions of the rotate kernel go here
 ******************************************************/

/* 
 * naive_rotate - The naive baseline version of rotate 
 */
char naive_rotate_descr[] = "naive_rotate: Naive baseline implementation";
void naive_rotate(int dim, pixel *src, pixel *dst) 
{
    int i, j;

    for (i = 0; i < dim; i++)
	for (j = 0; j < dim; j++)
	    dst[RIDX(dim-1-j, i, dim)] = src[RIDX(i, j, dim)];
}

/* 
 * rotate - Your current working version of rotate
 * IMPORTANT: This is the version you will be graded on
 */
char rotate_descr[] = "rotate: Current working version";
void rotate(int dim, pixel *src, pixel *dst) 
{
    int XN = 16,YN = 16;
    int t0 = (dim - 1) * dim;
    for (int x = 0;x < dim;x += XN){
        for (int y = 0;y < dim;y += YN){
            int xlm = x + XN,ylm = y + YN;
            // for (int i = x;i < xlm;i++){
            //     for (int j = y;j < ylm;j++){
            //         dst[t0 - j * dim + i] = src[i * dim + j];
            //     }
            // }
            for (int i = x;i < xlm;i++){
                int v0 = t0 - y * dim + i,v1 = i * dim + y;
                for (int j = y;j < ylm;j++){
                    dst[v0] = src[v1];
                    v0 -= dim,++v1;
                }
                // for (int j = y;j < ylm;j += 2){
                //     dst[v0] = src[v1];
                //     dst[v0 - dim] = src[v1 + 1];
                //     v0 -= dim * 2,v1 += 2;
                // }
                
            }
        }
    }
    // naive_rotate(dim, src, dst);
}

char rotate_0_descr[] = "rotate_0: Withour loop unrolling version";
void rotate_0(int dim, pixel *src, pixel *dst) 
{
    int XN = 16,YN = 16;
    int t0 = (dim - 1) * dim;
    for (int x = 0;x < dim;x += XN){
        for (int y = 0;y < dim;y += YN){
            int xlm = x + XN,ylm = y + YN;
            for (int i = x;i < xlm;i++){
                int v0 = t0 - y * dim + i,v1 = i * dim + y;
                for (int j = y;j < ylm;j++){
                    dst[v0] = src[v1];
                    v0 -= dim,++v1;
                }
            }
        }
    }
}

char rotate_2_descr[] = "rotate_2: loop unrolling 2 version";
void rotate_2(int dim, pixel *src, pixel *dst) 
{
    int XN = 16,YN = 16;
    int t0 = (dim - 1) * dim;
    for (int x = 0;x < dim;x += XN){
        for (int y = 0;y < dim;y += YN){
            int xlm = x + XN,ylm = y + YN;
            for (int i = x;i < xlm;i++){
                int v0 = t0 - y * dim + i,v1 = i * dim + y;
                for (int j = y;j < ylm;j += 2){
                    dst[v0] = src[v1];
                    dst[v0 - dim] = src[v1 + 1];
                    v0 -= dim * 2,v1 += 2;
                }
            }
        }
    }
}

char rotate_4_descr[] = "rotate_4: loop unrolling 4 version";
void rotate_4(int dim, pixel *src, pixel *dst) 
{
    int XN = 16,YN = 16;
    int t0 = (dim - 1) * dim;
    for (int x = 0;x < dim;x += XN){
        for (int y = 0;y < dim;y += YN){
            int xlm = x + XN,ylm = y + YN;
            for (int i = x;i < xlm;i++){
                int v0 = t0 - y * dim + i,v1 = i * dim + y;
                for (int j = y;j < ylm;j += 4){
                    dst[v0] = src[v1];
                    dst[v0 - dim] = src[v1 + 1];
                    dst[v0 - dim * 2] = src[v1 + 2];
                    dst[v0 - dim * 3] = src[v1 + 3];
                    v0 -= dim * 4,v1 += 4;
                }
            }
        }
    }
}

/*********************************************************************
 * register_rotate_functions - Register all of your different versions
 *     of the rotate kernel with the driver by calling the
 *     add_rotate_function() for each test function. When you run the
 *     driver program, it will test and report the performance of each
 *     registered test function.  
 *********************************************************************/

void register_rotate_functions() 
{
    add_rotate_function(&naive_rotate, naive_rotate_descr);   
    add_rotate_function(&rotate, rotate_descr);   
    add_rotate_function(&rotate_0,rotate_0_descr);
    add_rotate_function(&rotate_2,rotate_2_descr);
    add_rotate_function(&rotate_4,rotate_4_descr);
    /* ... Register additional test functions here */
}


/***************
 * SMOOTH KERNEL
 **************/

/***************************************************************
 * Various typedefs and helper functions for the smooth function
 * You may modify these any way you like.
 **************************************************************/

/* A struct used to compute averaged pixel value */
typedef struct {
    int red;
    int green;
    int blue;
    int num;
} pixel_sum;

/* Compute min and max of two integers, respectively */
static int min(int a, int b) { return (a < b ? a : b); }
static int max(int a, int b) { return (a > b ? a : b); }

/* 
 * initialize_pixel_sum - Initializes all fields of sum to 0 
 */
static void initialize_pixel_sum(pixel_sum *sum) 
{
    sum->red = sum->green = sum->blue = 0;
    sum->num = 0;
    return;
}

/* 
 * accumulate_sum - Accumulates field values of p in corresponding 
 * fields of sum 
 */
static void accumulate_sum(pixel_sum *sum, pixel p) 
{
    sum->red += (int) p.red;
    sum->green += (int) p.green;
    sum->blue += (int) p.blue;
    sum->num++;
    return;
}

/* 
 * assign_sum_to_pixel - Computes averaged pixel value in current_pixel 
 */
static void assign_sum_to_pixel(pixel *current_pixel, pixel_sum sum) 
{
    current_pixel->red = (unsigned short) (sum.red/sum.num);
    current_pixel->green = (unsigned short) (sum.green/sum.num);
    current_pixel->blue = (unsigned short) (sum.blue/sum.num);
    return;
}

/* 
 * avg - Returns averaged pixel value at (i,j) 
 */
static pixel avg(int dim, int i, int j, pixel *src) 
{
    int ii, jj;
    pixel_sum sum;
    pixel current_pixel;

    initialize_pixel_sum(&sum);
    for(ii = max(i-1, 0); ii <= min(i+1, dim-1); ii++) 
	for(jj = max(j-1, 0); jj <= min(j+1, dim-1); jj++) 
	    accumulate_sum(&sum, src[RIDX(ii, jj, dim)]);

    assign_sum_to_pixel(&current_pixel, sum);
    return current_pixel;
}

/******************************************************
 * Your different versions of the smooth kernel go here
 ******************************************************/

int mymax(int a,int b) {return a > b ? a : b;}
int mymin(int a,int b) {return a < b ? a : b;}

/*
 * naive_smooth - The naive baseline version of smooth 
 */
char naive_smooth_descr[] = "naive_smooth: Naive baseline implementation";
void naive_smooth(int dim, pixel *src, pixel *dst) 
{
    int i, j;

    for (i = 0; i < dim; i++)
	for (j = 0; j < dim; j++)
	    dst[RIDX(i, j, dim)] = avg(dim, i, j, src);
}

/*
 * smooth - Your current working version of smooth. 
 * IMPORTANT: This is the version you will be graded on
 */

pixel_sum arr[512][512];
char smooth_descr[] = "smooth: Current working version";
void smooth(int dim, pixel *src, pixel *dst) 
{
    pixel *p = src;
    for (int i = 0;i < dim;i++){
        arr[i][0].blue = p->blue + (p+1)->blue;
        arr[i][0].green = p->green + (p+1)->green;
        arr[i][0].red = p->red + (p+1)->red;
        arr[i][0].num = 2;
        ++p;
        for (int j = 1;j < dim - 1;j++){
            arr[i][j].blue = (p-1)->blue + p->blue + (p+1)->blue;
            arr[i][j].green = (p-1)->green + p->green + (p+1)->green;
            arr[i][j].red = (p-1)->red + p->red + (p+1)->red;
            arr[i][j].num = 3;
            ++p;
        }
        arr[i][dim-1].blue = (p-1)->blue + p->blue;
        arr[i][dim-1].green = (p-1)->green + p->green;
        arr[i][dim-1].red = (p-1)->red + p->red;
        arr[i][dim-1].num = 2;
        ++p;
    }
    pixel *dstcur = dst;
    for (int i = 0;i < dim;i++){
        for (int j = 0;j < dim;j++){
            int r = 0,g = 0,b = 0,n = 0;
            int iilim = mymin(i + 1,dim - 1);
            int iist = mymax(i - 1,0);
            pixel_sum *cur = (pixel_sum*)(arr) + (iist * 512 + j);
            for (int ii = iist;ii <= iilim;ii++){
                r += cur->red,g += cur->green,b += cur->blue,n += cur->num;
                // assert(cur == &arr[ii][j]);
                cur += 512;
                
            }
            dstcur->red = r / n,dstcur->green = g / n,dstcur->blue = b / n;
            // assert(n != 0);
            ++dstcur;
        }
    }
}


/********************************************************************* 
 * register_smooth_functions - Register all of your different versions
 *     of the smooth kernel with the driver by calling the
 *     add_smooth_function() for each test function.  When you run the
 *     driver program, it will test and report the performance of each
 *     registered test function.  
 *********************************************************************/

void register_smooth_functions() {
    add_smooth_function(&smooth, smooth_descr);
    add_smooth_function(&naive_smooth, naive_smooth_descr);
    /* ... Register additional test functions here */
}

