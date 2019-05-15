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
    CACHEBLCOK new_block;
    int i;
    std::list<CACHEBLCOK>::iterator iter;
    for(iter = (*cache).begin(); iter != (*cache).end(); iter++)
    {
        if(((*iter).tagger==(*cacheblock).tagger)&((*iter).index==(*cacheblock).index))
        {
            new_block.flag=cacheblock->flag;
            new_block.index=cacheblock->index;
            new_block.tagger=cacheblock->tagger;
            for(i=0;i<CACHELINE;i++) { new_block.data[i]=cacheblock->data[i];}
            (*cache).erase(iter);//find the block,and erase it
            (*cache).push_back(new_block);//and push the blcok back in the LRU List
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
    {   //head is the oldest
        if(index==(*iter).index)
        {
            u64 evict_addr= decode_cache((*iter),cache_size,assoc);
            (*cache).erase(iter);//due to the write_through we don't using it again,so can erase it easily
            return evict_addr;
        }
    }
    EPRINTF("NO CACHE BLOCK EVICT LRU\n");
    return -1;
}


CACHEBLCOK evict_wb (u64 addr,CACHE* cache,int cache_size,int assoc)
{
    u64 index,tagger;
    int i;
    CACHEBLCOK evict_block;
    decode_addr(addr,&tagger,&index,cache_size,assoc);
    std::list<CACHEBLCOK>::iterator iter;
    for(iter = (*cache).begin(); iter != (*cache).end(); iter++)
    {
        if(index==(*iter).index)
        {
            evict_block.flag=(*iter).flag;
            evict_block.index=(*iter).index;
            evict_block.tagger=(*iter).tagger;
            for(i=0;i<CACHELINE;i++){evict_block.data[i]=(*iter).data[i];}
            (*cache).erase(iter);//due to the write back,if evict it ,we need to get the block to write back
            return evict_block;
        }
    }
    evict_block.flag=-1;
    evict_block.index=-1;
    evict_block.tagger=-1;
    for(i=0;i<CACHELINE;i++){evict_block.data[i]=-1;}
    EPRINTF("NO CACHE BLOCK EVICT LRU\n");
    return evict_block;
}
