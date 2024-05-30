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
char transpose_submit_desc[] = "Transpose submission";
void transpose_submit(int M, int N, int A[N][M], int B[M][N])
{
    int i, j, col_block = 0, row_block = 0;
    int block_size = 8;
    if(N == 67)
    {
        block_size = 16;
    }
    int temp = -1;
    int temp2_0 = -1;
    int temp2_1 = -1;
    int temp3 = 0;
    int m, n;
    for(i=row_block*block_size;i<(row_block+1)*block_size && i<N;i++)
    {
        if(N == 64)
        {
            if(row_block == 7)
            {
                for(j=col_block*block_size;j<(col_block+1)*block_size && j<M;j++)
                {
                    if(col_block == row_block && (i == j))
                    {
                        temp = A[i][j];
                        continue;
                    }
                    
                    if(j >= 4+col_block*block_size && i < 4+row_block*block_size)
                    {
                        if(col_block == row_block && ((2*col_block+1)*block_size-1-j) == i)
                        {
                            temp2_0 = A[i][j]; 
                            continue;
                        }
                        B[(2*col_block+1)*block_size-1-j][i+4] = A[i][j];
                        continue;
                    }
                    if(j < 4+col_block*block_size && i >= 4+row_block*block_size)
                    {
                        if(col_block == row_block && ((2*col_block+1)*block_size-1-j) == i)
                        {
                            temp2_1 = A[i][j]; 
                            continue;
                        }
                        B[(2*col_block+1)*block_size-1-j][i-4] = A[i][j];
                        continue;
                    }
                    B[j][i] = A[i][j];
                }
                if(temp != -1)
                {
                    B[i][i] = temp;
                    temp = -1;
                }
                if(temp2_1 != -1)
                {
                    B[i][i-4] = temp2_1;
                    temp2_1 = -1;
                }
                if(temp2_0 != -1)
                {
                    B[i][i+4] = temp2_0;
                    temp2_0 = -1;
                }
                if(i == ((row_block+1)*block_size-1) && j == (col_block+1)*block_size)
                {
                    for(m=0;m<4;m++)
                    {
                        for(n=0;n<4;n++)
                        {
                            temp3 = B[m+4+col_block*block_size][n+row_block*block_size];
                            B[m+4+col_block*block_size][n+row_block*block_size] = B[3-m+col_block*block_size][n+4+row_block*block_size];
                            B[3-m+col_block*block_size][n+4+row_block*block_size] = temp3;
                        }
                    }
                } 
            }
            else
            {
                for(j=col_block*block_size;j<(col_block+1)*block_size && j<M;j++)
                {
                    if(col_block == row_block && (i == j) && j<4+col_block*block_size)
                    {
                        temp = A[i][j];
                        continue;
                    }
                    if(j >= 4+col_block*block_size)
                    {
                        B[j-4-col_block*block_size][i-row_block*block_size+7*block_size] = A[i][j];
                        continue;
                    }
                    if(col_block == row_block && (i-4) == j && j<4+col_block*block_size)
                    {
                        temp2_0 = A[i][j];
                        continue;
                    }
                    B[j][i] = A[i][j];
                }
                if(temp != -1)
                {
                    B[i][i] = temp;
                    temp = -1;
                }
                if(temp2_0 != -1)
                {
                    B[i-4][i] = temp2_0;
                    temp2_0 = -1;
                }
                if(i == ((row_block+1)*block_size-1) && j == (col_block+1)*block_size)
                {
                    for(m=0;m<4;m++)
                    {
                        for(n=0;n<8;n++)
                        {
                            B[col_block*block_size+4+m][row_block*block_size+n] = B[m][7*block_size+n];
                        }
                    }

                } 
            }
            
        }
        else
        {
            for(j=col_block*block_size;j<(col_block+1)*block_size && j<M;j++)
            {
                if(col_block == row_block && (i == j))
                {
                    temp = A[i][j];
                    continue;
                }
                B[j][i] = A[i][j];
            }
            if(temp != -1)
            {
                B[i][i] = temp;
                temp = -1;
            }
        }
        
        if((i == (row_block+1)*block_size -1 || i == N - 1) && (j == M))
        {
            row_block++;
            col_block = 0;
            i = row_block*block_size-1;
            continue;
        }
        if(i == (row_block+1)*block_size -1 || i == N - 1)
        {
            i = row_block*block_size-1;
            col_block++;
        }
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

