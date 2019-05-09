//***** ***** ***** ***** ***** ***** ***** ***** ***** ***** *****
//Author:   jcyuan
//E-Mail:   yuan861025184@163.com
//Project:  hybrid cache simulator
/*Function: the simple write through protocol of Cache*/
//***** ***** ***** ***** ***** ***** ***** ***** ***** ***** *****

#ifndef WRT_THROUGH_H_INCLUDED
#define WRT_THROUGH_H_INCLUDED
#include<list>
#include<iterator>
#include "../../cache/cache.h"

#ifdef LRU
    #include "../replace/lru.h"
#endif // LRU

/*Check the data from CMD and CacheBlock if is the same when read operation happens*/
int check_data(CMD* cmd, CACHEBLCOK* block,int cache_size,int assoc);

/*execute the read command in the Cache*/
int read(CMD* cmd,CACHE* cache,int cache_size,int assoc);

/* One Level Cache, execute the write command in the Cache*/
int write(CMD* cmd,CACHE* cache,int cache_size,int assoc);

/*Two Level Cache, execute the write command in the Cache*/
int write(CMD* cmd, CACHE* L1, CACHE* L2, int l1_size, int l2_size, int l1_assoc, int l2_assoc,int cpu);

/*just one Level Cache, the data in memory, Load the data from the memory to cache*/
int load(CMD* cmd,MEMORY* memory,CACHE* cache,int cache_size,int assoc);

/* two Level Cache, the data in L2Cache, Load the data from the L2Cache to L1cache*/
int load(CMD* cmd, CACHE* L2, CACHE* L1, int l1_size, int l2_size, int l1_assoc, int l2_assoc);

/* two Level Cache, the data in Memory, Load the data from  Memory to L2Cache and L1Cache*/
int load(CMD* cmd,MEMORY* memory,CACHE* L1, CACHE* L2, int l1_size, int l2_size, int l1_assoc, int l2_assoc,int cpu);

/*store the data that cmd write to memory for write through*/
int store(CMD* cmd,MEMORY* memory);

/*check the memory to ensure that the memory has the data that command need,if not has,need to push the data to memory,
normally we don't need to do this if you can ensure the trace is right and complete*/
int avoid_null(CMD* cmd,MEMORY* memory);

/*the write through protocol of one Level Cache*/
int wrt_through(CMD* cmd,MEMORY* memroy,CACHE* cache,int cache_size,int assoc);

/*the write through protocol of two Level Cache*/
int wrt_through(CMD* cmd,MEMORY* memory,CACHE* L1,CACHE* L2,int l1_size,int l2_size,int l1_assoc,int l2_assoc,int cpu);
#endif // WRT_THROUGH_H_INCLUDED
