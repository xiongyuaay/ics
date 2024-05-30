#include "cachelab.h"
#include <getopt.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <math.h>

/* Type: Memory address */
typedef unsigned long long int mem_addr;

typedef struct _cache_line
{
    /* data */
    int value;
    int b;
    int count;
    mem_addr tag;
}Cache_line;


int main(int argc, char** argv)
{
    int opt,s,E,b;
    char *TraceFilePath = NULL;
    /* looping over arguments */
    while(-1 != (opt = getopt(argc, argv, "s:E:b:t:")))
    {
        /* determine which argument it’s processing */
        switch(opt)
        {
            case 's':
                s = atoi(optarg);
                break;

            case 'E':
                E = atoi(optarg);
                break;

            case 'b':
                b = atoi(optarg);
                break;
            
            case 't':
                TraceFilePath = optarg;
                break;

            default:
                printf("wrong argument\n");
                break;
        }
    }
    int S = pow(2,s);
    int B = pow(2,b);

    // create cache
    Cache_line **cache = NULL;
    cache = (Cache_line**)malloc(S*sizeof(Cache_line*));
    if(cache == NULL)
    {
        printf("cache set error");
        return 1;
    }
    for(int i=0;i<S;i++)
    {
        cache[i] = NULL;
        cache[i] = (Cache_line*)malloc(E*sizeof(Cache_line));
        if(cache[i] == NULL)
        {
            printf("cache line error");
            return 1;
        }
        for(int j=0;j<E;j++)
        {
            cache[i][j].value = 0;
            cache[i][j].count = 0;
        }
    }



    // read the file
    FILE * f;  //pointer  to  FILE  object
    f = fopen(TraceFilePath,"r");  //open file for reading
    char identifier;
    mem_addr address;
    int size;
    // Reading lines like " L 10,1" or " M 20,1"
    int time=0;
    mem_addr tag=0;
    mem_addr set=0;
    int hits = 0;
    int misses = 0;
    int evicts = 0;
    while(fscanf(f," %c  %llx,%d",  &identifier, &address, &size)>0) 
    {
        if(identifier == 'I')
        {
            continue;
        }
        time++;
        address = address / B;
        set = address % S;
        address = address / S;
        tag = address;
        int hit = 0;
        int min = 0;
        int empty = -1;
        for(int i=0;i<E;i++)
        {
            if(cache[set][i].value == 1 && cache[set][i].tag == tag)
            {
                hits++;
                if(identifier == 'M')
                {
                    hits++;
                }
                hit = 1;
                cache[set][i].count = time;
                break;
            }
            if(cache[set][i].value == 0)
            {
                empty = i;
            }
            if(cache[set][i].value == 1 && cache[set][i].count < cache[set][min].count)
            {
                min = i;
            }
        }
        if(hit == 1) continue;
        if(empty != -1)
        {
            cache[set][empty].value = 1;
            cache[set][empty].count = time;
            cache[set][empty].tag = tag;
            misses++;
            if(identifier == 'M')
            {
                hits++;
            }
            continue;
        }
        cache[set][min].value = 1;
        cache[set][min].tag = tag;
        cache[set][min].count = time;
        misses++;
        evicts++;
        if(identifier == 'M')
        {
            hits++;
        }
    }

    printSummary(hits, misses, evicts);
    return 0;
}
