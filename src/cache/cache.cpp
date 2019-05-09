//***** ***** ***** ***** ***** ***** ***** ***** ***** ***** *****
//Author:   jcyuan
//E-Mail:   yuan861025184@163.com
//Project:  hybrid cache simulator
/*Function: read,store and processing Cache */
//***** ***** ***** ***** ***** ***** ***** ***** ***** ***** *****

#include "cache.h"



int calc_bit(u32* tagger_bit,u32*index_bit,u32*lsb_bit,int cache_size,int assoc)
{
    u32 addr_bit=LOG2(MEM_SIZE);
    *lsb_bit=LOG2(CACHELINE);
    u32 cache_line_cnt;
    u32 index_cnt=0;
    cache_line_cnt=(u32)(cache_size/CACHELINE);
    index_cnt=(u32)(cache_line_cnt/assoc);
    *index_bit=LOG2(index_cnt);
    *tagger_bit=addr_bit-*lsb_bit-*index_bit;
    return 0;
}


u64 decode_cache(CACHEBLCOK block,int cache_size,int assoc)
{
    u32 tagger,index,lsb;
    calc_bit(&tagger,&index,&lsb,cache_size,assoc);
    return ((block.tagger<<(index+lsb))|(block.index<<(lsb)));
}



CACHEBLCOK* find_cache(u64 addr,CACHE *cache,int cache_size,int assoc,int* in)
{
    u64 cmd_addr=align_addr(addr);
    std::list<CACHEBLCOK>::iterator iter;
    if((*cache).empty()){ *in=0; return NULL;}
    else
    {
        for(iter=(*cache).begin();iter!=(*cache).end();iter++)
        {
            if(cmd_addr==decode_cache((*iter),cache_size,assoc))
            {
                *in=1;
                return &(*iter);
            }
        }
        *in=0;
        return NULL;
        //if run this and you need to get the block that returned back
        //you need to confirm the block in the Cache ---- in=1,otherwise,there maybe something wrong
    }
}


int decode_addr(u64 addr,u64* tagger,u64* index,int cache_size,int assoc)
{
    u64 a_addr=align_addr(addr);
    u32 tagger_bit,index_bit,lsb_bit;
    calc_bit(&tagger_bit,&index_bit,&lsb_bit,cache_size,assoc);
    u64 index_mask=(u64)(((0x1)<<index_bit)-0x1);
    u64 tagger_mask=(u64)(((0x1)<<tagger_bit)-0x1);
    a_addr=a_addr>>lsb_bit;
    *index=a_addr&index_mask;
    a_addr=a_addr>>index_bit;
    *tagger=a_addr&tagger_mask;
    return 0;
}



int check_full(u64 addr,CACHE* cache,int cache_size,int assoc)//watch the index
{
    int full_flag=0;
    int counter=0;
    std::list<CACHEBLCOK>::iterator iter;
    u64 a_addr=align_addr(addr);
    u64 tagger,index;
    decode_addr(a_addr,&tagger,&index,cache_size,assoc);
    if((*cache).empty()){return 0;}//if the cache empty it's unfull
    for(iter=(*cache).begin();iter!=(*cache).end();iter++)
    {
        if((*iter).index==index) { counter=counter+1;}
        else { counter=counter;}
        if(counter==assoc){full_flag=1;}
        else if(counter>assoc){EPRINTF("CACHE ASSOC OVER\n");}
        else{full_flag=0;}
    }//if the counter is the same as the ASSOC so the addr in the Cache is Full
    if((*cache).size()>((u32)(cache_size/CACHELINE))) { EPRINTF("CACHE OVERFLOW\n");}
    //although the Cache List don't limit the Size,but following the ASSOC condition we can get the
    //information of Full,so we don't need to all List Cache Size,but we need to avoid to come out something error
    //so,we check all the size if to over the CacheLine number
    return full_flag;
}

void load_cache(CACHE* cache,MEM_CELL* cell,int cache_size,int assoc)
{
    CACHEBLCOK block;
    int i=0;
    block.flag=0x0;//don't use
    decode_addr(cell->addr,&block.tagger,&block.index,cache_size,assoc);
    for(i=0;i<CACHELINE;i++) { block.data[i]=cell->data[i];}
    (*cache).push_back(block);
    return;
}

void load_cache(CACHE* cache,CACHEBLCOK* block,int l1_size,int l2_size,int l1_assoc,int l2_assoc)
{
    CACHEBLCOK new_block;
    int i=0;
    new_block.flag=block->flag;
    u64 addr=decode_cache(*block,l2_size,l2_assoc);
    decode_addr(addr,&(new_block.tagger),&(new_block.index),l1_size,l1_assoc);
    for(i=0;i<CACHELINE;i++) { new_block.data[i]=block->data[i];}
    (*cache).push_back(new_block);
    return;
}


u64 evict_block (u64 addr,CACHE* cache,int cache_size,int assoc)
{
    u64 index,tagger;
    decode_addr(addr,&tagger,&index,cache_size,assoc);
    std::list<CACHEBLCOK>::iterator iter;
    for(iter = (*cache).begin(); iter != (*cache).end(); iter++)
    {
        if((index==(*iter).index)&(tagger==(*iter).tagger))
        {
            (*cache).erase(iter);
            return addr;
        }
    }
    EPRINTF("NO CACHE BLOCK\n");
    return -1;
}



void display(CACHE* cache,int cache_size,int assoc)
{
    int i;
    std::list<CACHEBLCOK>::iterator iter;
    for(iter=(*cache).begin();iter!=(*cache).end();iter++)
    {
        printf("FLAG: %x\tTAGGER: %llx\tINDEX: %llx\n",(*iter).flag,(*iter).tagger,(*iter).index);
        u64 addr=decode_cache(*(&(*iter)),cache_size,assoc);
        printf("ADDR: %llx\n",addr);
        for(i=0;i<CACHELINE;i++) { printf("%hx ",(*iter).data[i]);}
        printf("\n");
    }
    return;
}



