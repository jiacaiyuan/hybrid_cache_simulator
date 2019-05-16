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


void set_flag(CACHEBLCOK* block,u32 flag);

void set_flag(CMD* cmd,CACHE* cache,int cache_size,int assoc,u32 flag);

u32 get_flag(CACHEBLCOK* block);

int wrt_back(CACHEBLCOK* block,MEMORY* memory,int cache_size,int assoc);

int wrt_back(CACHEBLCOK* block,CACHE* cache,int l1_size,int l2_size,int l1_assoc,int l2_assoc);

int evict_wrt_back(CMD* cmd,CACHE* L1,CACHE* L2,int l1_size,int l2_size,int l1_assoc,int l2_assoc);

int evict_wrt_back(CMD* cmd,MEMORY* memory,CACHE* L1,CACHE* L2,int l1_size,int l2_size,int l1_assoc,int l2_assoc,int cpu);

int BusUpgr(CMD* cmd,MEMORY* memory,CACHE* L1,CACHE* L2,int l1_size,int l2_size,int l1_assoc,int l2_assoc,int cpu);

CACHEBLCOK BusRd(CMD* cmd,MEMORY* memory,CACHE* L1,CACHE* L2,int l1_size,int l2_size,int l1_assoc,int l2_assoc,int cpu,int* get);

CACHEBLCOK BusRdx(CMD* cmd,MEMORY* memory,CACHE* L1,CACHE* L2,int l1_size,int l2_size,int l1_assoc,int l2_assoc,int cpu,int* get);

int mesi(CMD* cmd,MEMORY* memory,CACHE* L1,CACHE* L2,int l1_size,int l2_size,int l1_assoc,int l2_assoc,int cpu);









#endif // MESI_H_INCLUDED
