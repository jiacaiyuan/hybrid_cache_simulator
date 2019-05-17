//***** ***** ***** ***** ***** ***** ***** ***** ***** ***** *****
//Author:   jcyuan
//E-Mail:   yuan861025184@163.com
//Project:  hybrid cache simulator
/*Function: read,store and processing Cache the protocol is MESI write back */
//Reference: https://en.wikipedia.org/wiki/MESI_protocol
//***** ***** ***** ***** ***** ***** ***** ***** ***** ***** *****

#ifndef MESI_H_INCLUDED
#define MESI_H_INCLUDED

#include<list>
#include<iterator>
#include "../../cache/cache.h"
#include "../../include/type.h"
#include "../../include/struct.h"
#include "wrt_through.h"
#ifdef LRU
    #include "../replace/lru.h"
#endif // LRU

#define MODIFIED  ((u32)0x0)
#define EXCLUSIVE ((u32)0x1)
#define SHARED    ((u32)0x2)
#define INVALID   ((u32)0x3)



#define MESI_MASK ((u32)0x3)
#define MESI_BIT   2

/*Set the flag of the specific block */
void set_flag(CACHEBLCOK* block,u32 flag);

/*set the flag of the block in the Cache according to the command's address*/
void set_flag(CMD* cmd,CACHE* cache,int cache_size,int assoc,u32 flag);

/*get the flag of a cache block*/
u32 get_flag(CACHEBLCOK* block);

/*write the block back to memory from cache*/
int wrt_back(CACHEBLCOK* block,MEMORY* memory,int cache_size,int assoc);

/*write the block back to L2 cache from L1 cache*/
int wrt_back(CACHEBLCOK* block,CACHE* cache,int l1_size,int l2_size,int l1_assoc,int l2_assoc);

/*evict a block and write the block back from L1 Cache to L2 Cache */
int evict_wrt_back(CMD* cmd,CACHE* L1,CACHE* L2,int l1_size,int l2_size,int l1_assoc,int l2_assoc);

/*evict a block and write the block back from L2 Cache to Memory but L1 may has same block, need to set Invalid*/
int evict_wrt_back(CMD* cmd,MEMORY* memory,CACHE* L1,CACHE* L2,int l1_size,int l2_size,int l1_assoc,int l2_assoc,int cpu);

/*request to write the block that the owner Cache has*/
int BusUpgr(CMD* cmd,MEMORY* memory,CACHE* L1,CACHE* L2,int l1_size,int l2_size,int l1_assoc,int l2_assoc,int cpu);

/*request to read the block that the owner Cache not has
search the other Cache to find the block that the command need and load the block to the own Cache*/
CACHEBLCOK BusRd(CMD* cmd,MEMORY* memory,CACHE* L1,CACHE* L2,int l1_size,int l2_size,int l1_assoc,int l2_assoc,int cpu,int* get);

/*request to write the block that the owner Cache not has
search the other Cache to find the block that the command need and load the block to the own Cache*/
CACHEBLCOK BusRdx(CMD* cmd,MEMORY* memory,CACHE* L1,CACHE* L2,int l1_size,int l2_size,int l1_assoc,int l2_assoc,int cpu,int* get);

/*the MESI protocol of Two Level Cache*/
int mesi(CMD* cmd,MEMORY* memory,CACHE* L1,CACHE* L2,int l1_size,int l2_size,int l1_assoc,int l2_assoc,int cpu);



#endif // MESI_H_INCLUDED
