//***** ***** ***** ***** ***** ***** ***** ***** ***** ***** *****
//Author:   jcyuan
//E-Mail:   yuan861025184@163.com
//Project:  hybrid cache simulator
/*Function: lru is Cache Replace Policy*/
//***** ***** ***** ***** ***** ***** ***** ***** ***** ***** *****

#include "lru.h"
/*the LRU is only one List,in theory the LRU is one List each Assoc Group,but one List in all
Cache is OK due to the fact that we evict the Block in the Tail of List in the index about Assoc Group.
So, if All Block in one LRU List, then each Assoc Group is in the order of LRU,too  */
int update_lru(CACHE* cache,CACHEBLCOK* cacheblock)
{
    //first oldest,tail new
    std::list<CACHEBLCOK>::iterator iter;
    for(iter = (*cache).begin(); iter != (*cache).end(); iter++)
    {
        if(((*iter).tagger==(*cacheblock).tagger)&((*iter).index==(*cacheblock).index))
        {
            (*cache).erase(iter);//find the block,and erase it
            (*cache).push_back(*cacheblock);//and push the blcok back in the LRU List
            //DPRINTF("UPDATE LRU\n");
            return 0;
        }
    }
    EPRINTF("UPDATE LRU NO FIND\n");
    return 1;
}

/*due to the Assoc of Cache, when need to evict, we need to evict the oldest block
in the same Assoc Group, in one Group,their index is same and different in tagger*/
u64 evict (u64 addr,CACHE* cache,int cache_size,int assoc)
{
    u64 index,tagger;
    decode_addr(addr,&tagger,&index,cache_size,assoc);
    std::list<CACHEBLCOK>::iterator iter;
    for(iter = (*cache).begin(); iter != (*cache).end(); iter++)
    {
        if(index==(*iter).index)
        {
            u64 evict_addr= decode_cache((*iter),cache_size,assoc);
            (*cache).erase(iter);
            return evict_addr;
        }
    }
    EPRINTF("NO CACHE BLOCK EVICT LRU\n");
    return -1;
}



