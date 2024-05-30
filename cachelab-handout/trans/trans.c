#include <stdio.h>
#define _M 64
#define _N 64

void transpose_submit(int M, int N, int A[_N][_M], int B[_M][_N])
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
                    if(col_block == row_block && (i == j))
                    {
                        temp = A[i][j];
                        continue;
                    }
                    if(j >= 4+col_block*block_size)
                    {
                        B[j-4-col_block*block_size][i-row_block*block_size+7*block_size] = A[i][j];
                        continue;
                    }
                    B[j][i] = A[i][j];
                }
                if(temp != -1)
                {
                    B[i][i] = temp;
                    temp = -1;
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

int main()
{
    int A[_N][_M];
    int B[_M][_N]={0};
    for(int i=0;i<_N;i++)
    {
        for(int j=0;j<_M;j++)
        {
            A[i][j]=i*_M + j;
        }
    }
    transpose_submit(_M, _N, A, B);
    return 0;
}