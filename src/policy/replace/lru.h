//***** ***** ***** ***** ***** ***** ***** ***** ***** ***** *****
//Author:   jcyuan
//E-Mail:   yuan861025184@163.com
//Project:  hybrid cache simulator
/*Function: lru is Cache Replace Policy*/
//***** ***** ***** ***** ***** ***** ***** ***** ***** ***** *****

#ifndef LRU_H_INCLUDED
#define LRU_H_INCLUDED

#include "../../cache/cache.h"
#include "../../include/struct.h"
#include "../../include/type.h"
#include <vector>
#include <iterator>
#include <list>



/*Updating the LRU list in the Cache,
the Newest CacheBlock in the tail of the list and
the Oldest CacheBlock in the head of the list*/
int update_lru(CACHE* cache,CACHEBLCOK* cacheblock);

/*Evict the CacheBlock in the Assoc Group about the same index*/
u64 evict (u64 addr,CACHE* cache,int cache_size,int assoc);

#endif // LRU_H_INCLUDED
