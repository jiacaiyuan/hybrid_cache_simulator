//***** ***** ***** ***** ***** ***** ***** ***** ***** ***** *****
//Author:   jcyuan
//E-Mail:   yuan861025184@163.com
//Project:  hybrid cache simulator
/*Function: read,store and processing Cache the protocol is MESI write back */
//Reference: https://en.wikipedia.org/wiki/MESI_protocol
//***** ***** ***** ***** ***** ***** ***** ***** ***** ***** *****
#include"mesi.h"

void set_flag(CACHEBLCOK* block,u32 flag)
{
    block->flag=(((block->flag)>>MESI_BIT)<<MESI_BIT)|((flag)&MESI_MASK);
    //right shift and left shift to clear the status flag,and write other flag to the block
}

void set_flag(CMD* cmd,CACHE* cache,int cache_size,int assoc,u32 flag)
{
    u64 index,tagger;
    std::list<CACHEBLCOK>::iterator iter;
    decode_addr(cmd->addr,&tagger,&index,cache_size,assoc);
    for(iter = (*cache).begin(); iter != (*cache).end(); iter++)
    {
        //find the block according the cmd's addr and set the flag of the block
        if((tagger==(*iter).tagger)&(index==(*iter).index)) { set_flag(&(*iter),flag); }
    }
}



u32 get_flag(CACHEBLCOK* block)
{
    if (block==NULL) {return -1;}
    else {return (block->flag)&MESI_MASK;}
}

int wrt_back(CACHEBLCOK* block,MEMORY* memory,int cache_size,int assoc)
{
    int i;
    std::vector<MEM_CELL>::iterator iter;
    if((get_flag(block))==INVALID) {return 0;}
    else
    {
        u64 addr=decode_cache((*block),cache_size,assoc);
        for(iter=(*memory).begin();iter!=(*memory).end();iter++)
        {
            if(addr==(*iter).addr)
            {
                for(i=0;i<CACHELINE;i++) {(*iter).data[i]=block->data[i];}
                return 0;
            }

        }
    }
    EPRINTF("NO MEMORY CELL\n");
    return -1;
}

//write the L1 block to L2 cache
int wrt_back(CACHEBLCOK* block,CACHE* cache,int l1_size,int l2_size,int l1_assoc,int l2_assoc)
{
    int i;
    u64 index,tagger;
    std::list<CACHEBLCOK>::iterator iter;
    if((get_flag(block))==INVALID) {return 0;}
    else
    {
        u64 addr=decode_cache(*block,l1_size,l1_assoc);// the same addr's block  l1 index and tagger is different in L2
        decode_addr(addr,&tagger,&index,l2_size,l2_assoc);
        for(iter=(*cache).begin();iter!=(*cache).end();iter++)
        {
            if((index==(*iter).index)&(tagger==(*iter).tagger))
            {   //find the block evict from L1 cache and in L2 Cache must
                for(i=0;i<CACHELINE;i++) {(*iter).data[i]=block->data[i];}
                (*iter).flag=block->flag;
                return 0;
            }
        }
    }
    EPRINTF("NO CACHE BLOCK\n");
    return -1;
}


int evict_wrt_back(CMD* cmd,CACHE* L1,CACHE* L2,int l1_size,int l2_size,int l1_assoc,int l2_assoc)//evict l1 and write back to L2
{
    CACHEBLCOK e_block;
    if(check_full(cmd->addr,&L1[cmd->owner],l1_size,l1_assoc))
    {
        e_block=evict_wb(cmd->addr,&L1[cmd->owner],l1_size,l1_assoc);//get the evict block
        wrt_back(&e_block,L2,l1_size,l2_size,l1_assoc,l2_assoc);
    }
    return 0;
}

int evict_wrt_back(CMD* cmd,MEMORY* memory,CACHE* L1,CACHE* L2,int l1_size,int l2_size,int l1_assoc,int l2_assoc,int cpu)//evict l2 and write back to memory
{
    CACHEBLCOK e_block;
    u64 addr,index,tagger;
    int i;
    if(check_full(cmd->addr,L2,l2_size,l2_assoc))
    {
        e_block=evict_wb(cmd->addr,L2,l2_size,l2_assoc);
        addr=decode_cache(e_block,l2_size,l2_assoc);
        decode_addr(addr,&tagger,&index,l1_size,l1_assoc);
        std::list<CACHEBLCOK>::iterator iter;
        for(i=0;i<cpu;i++)
        {
            for(iter = L1[i].begin();iter != L1[i].end(); iter++)
            {
                if(((*iter).index==index)&((*iter).tagger==tagger)) { set_flag(&(*iter),INVALID);}
            }
        }
        wrt_back(&e_block,memory,l2_size,l2_assoc);
    }
    //there may be one condition that the block evict from L2 cache is in the state of MES, and L1 has the same block,
    //when the block in L2 is evicted to Memory if the same block in L1 need to write back to L2 cache, it's not in L2
    // and may some error generate, so when the block in L2 is evicted, the same block in L2 need to be set INVALID, in
    // this way the block in L1 can't write back to L2
    return 0;
}


int BusUpgr(CMD* cmd,MEMORY* memory,CACHE* L1,CACHE* L2,int l1_size,int l2_size,int l1_assoc,int l2_assoc,int cpu)
{
    int i;
    u64 index,tagger;
    std::list<CACHEBLCOK>::iterator iter;
    decode_addr(cmd->addr,&tagger,&index,l1_size,l1_assoc);
    for(i=0;i<cpu;i++)
    {
        if(i==cmd->owner) {continue;}
        else
        {
            for(iter = L1[i].begin(); iter != L1[i].end(); iter++)
            {
                if((tagger==(*iter).tagger)&(index==(*iter).index)) { set_flag(&(*iter),INVALID); }
            }
        }
    }
    decode_addr(cmd->addr,&tagger,&index,l2_size,l2_assoc);
    for(iter = (*L2).begin(); iter != (*L2).end(); iter++)
    {
        if((tagger==(*iter).tagger)&(index==(*iter).index)) { set_flag(&(*iter),INVALID); }
    }
    return 0;
    //when to write the block in L1 if the state of the block is SHARED, it means that other Cache has same block
    //if write to the block ,other block is the old value, which may cause the inconsistency so the bus send the busupgr
    // to tell other Cache that the block in their own Cache is INVALID
}

CACHEBLCOK BusRd(CMD* cmd,MEMORY* memory,CACHE* L1,CACHE* L2,int l1_size,int l2_size,int l1_assoc,int l2_assoc,int cpu,int* get)
{
    int i,j,in;
    CACHEBLCOK new_block;
    CACHEBLCOK* block;
    decode_addr(cmd->addr,&(new_block.tagger),&(new_block.index),l1_size,l1_assoc);//get the base information of new block
    for(i=0;i<cpu;i++)
    {
        //find in other L1 Cache
        if(i==cmd->owner){continue;}
        block=find_cache(cmd->addr,&L1[i],l1_size,l1_assoc,&in);
        if((in==1)&(((get_flag(block))==EXCLUSIVE)|((get_flag(block))==MODIFIED)|((get_flag(block))==SHARED)))
        {
            set_flag(block,SHARED);
            set_flag(&new_block,SHARED);
            for(j=0;j<CACHELINE;j++) { new_block.data[j]=block->data[j];}
            if((get_flag(block))==MODIFIED){wrt_back(block,memory,l1_size,l1_assoc);}//modify need to write to the memory,bus action required
            *get=1;
            return new_block;
        }
    }
    //find in the L2 cache
    block=find_cache(cmd->addr,L2,l2_size,l2_assoc,&in);
    if((in==1)&(((get_flag(block))==EXCLUSIVE)|((get_flag(block))==MODIFIED)|((get_flag(block))==SHARED)))
    {
        set_flag(block,SHARED);
        set_flag(&new_block,SHARED);
        for(j=0;j<CACHELINE;j++) { new_block.data[j]=block->data[j];}
        if((get_flag(block))==MODIFIED){wrt_back(block,memory,l1_size,l1_assoc);}//modify need to write to the memory,bus action required
        *get=1;
        return new_block;
    }
    *get=0;//get is 0 and we don't use the new_block
    new_block.flag=-1;
    new_block.index=-1;
    new_block.tagger=-1;
    for(i=0;i<CACHELINE;i++){new_block.data[i]=-1;}
    return new_block;
}

CACHEBLCOK BusRdx(CMD* cmd,MEMORY* memory,CACHE* L1,CACHE* L2,int l1_size,int l2_size,int l1_assoc,int l2_assoc,int cpu,int* get)
{
    int i,j,in;
    CACHEBLCOK* block;
    CACHEBLCOK new_block;
    decode_addr(cmd->addr,&(new_block.tagger),&(new_block.index),l1_size,l1_assoc);
    for(i=0;i<cpu;i++)
    {
        //find in L1 Cache
        if(i==cmd->owner){continue;}
        block=find_cache(cmd->addr,&L1[i],l1_size,l1_assoc,&in);
        if((in==1)&(((get_flag(block))==EXCLUSIVE)|((get_flag(block))==MODIFIED)|((get_flag(block))==SHARED)))
        {
            set_flag(block,INVALID);
            set_flag(&new_block,MODIFIED);
            for(j=0;j<CACHELINE;j++) { new_block.data[j]=block->data[j];}
            if((get_flag(block))==MODIFIED){wrt_back(block,memory,l1_size,l1_assoc);}//modify need to write to the memory,bus action required
            *get=1;
            return new_block;
        }
    }
    //find in L2 Cache
    block=find_cache(cmd->addr,L2,l2_size,l2_assoc,&in);
    if((in==1)&(((get_flag(block))==EXCLUSIVE)|((get_flag(block))==MODIFIED)|((get_flag(block))==SHARED)))
    {
        set_flag(block,INVALID);
        set_flag(&new_block,MODIFIED);
        for(j=0;j<CACHELINE;j++) { new_block.data[j]=block->data[j];}
        if((get_flag(block))==MODIFIED){wrt_back(block,memory,l1_size,l1_assoc);}//modify need to write to the memory
        *get=1;
        return new_block;
    }
    *get=0;//get is 0 and we don't use the new_block
    new_block.flag=-1;
    new_block.index=-1;
    new_block.tagger=-1;
    for(i=0;i<CACHELINE;i++){new_block.data[i]=-1;}
    return new_block;
}




int mesi(CMD* cmd,MEMORY* memory,CACHE* L1,CACHE* L2,int l1_size,int l2_size,int l1_assoc,int l2_assoc,int cpu)
{
    int in,get;
    CACHEBLCOK* block=find_cache(cmd->addr,&L1[cmd->owner],l1_size,l1_assoc,&in);
    CACHEBLCOK l_block;
    if((cmd->op)==0)//read
    {
        if((in==1)&(((get_flag(block))==MODIFIED)|((get_flag(block))==EXCLUSIVE)|((get_flag(block))==SHARED)))
        {
            read(cmd,&L1[cmd->owner],l1_size,l1_assoc);
            update_lru(&L1[cmd->owner],block);
        }
        else
        {
            //evict the useless block in owner L1 cache
            if((in==1)&((get_flag(block))==INVALID)){evict_block(cmd->addr,&L1[cmd->owner],l1_size,l1_assoc);}
            else { evict_wrt_back(cmd,L1,L2,l1_size,l2_size,l1_assoc,l2_assoc);}//not in evict and write back
            //load in
            l_block=BusRd(cmd,memory,L1,L2,l1_size,l2_size,l1_assoc,l2_assoc,cpu,&get);
            if(get){L1[cmd->owner].push_back(l_block);}
            else
            {
                avoid_null(cmd,memory);
                evict_wrt_back(cmd,memory,L1,L2,l1_size,l2_size,l1_assoc,l2_assoc,cpu);
                load(cmd,memory,L2,l2_size,l2_assoc);
                set_flag(cmd,L2,l2_size,l2_assoc,EXCLUSIVE);//to ensure the block when load in has flag
                load(cmd,&L1[cmd->owner],L2,l1_size,l2_size,l1_assoc,l2_assoc);
                set_flag(cmd,&L1[cmd->owner],l1_size,l1_assoc,EXCLUSIVE);//to ensure the block when load in has flag
            }
            read(cmd,&L1[cmd->owner],l1_size,l1_assoc);
        }
    }
    else if((cmd->op)==1)//write
    {
        if((in==1)&(((get_flag(block))==EXCLUSIVE)|((get_flag(block))==MODIFIED)|((get_flag(block))==SHARED)))
        {
            write(cmd,&L1[cmd->owner],l1_size,l1_assoc);
            update_lru(&L1[cmd->owner],block);
            set_flag(block,MODIFIED);
            if((get_flag(block))==SHARED) { BusUpgr(cmd,memory,L1,L2,l1_size,l2_size,l1_assoc,l2_assoc,cpu);}
        }
        else
        {
            if((in==1)&((get_flag(block))==INVALID)){evict_block(cmd->addr,&L1[cmd->owner],l1_size,l1_assoc);}
            else { evict_wrt_back(cmd,L1,L2,l1_size,l2_size,l1_assoc,l2_assoc);}//not in evict and write back
            l_block=BusRdx(cmd,memory,L1,L2,l1_size,l2_size,l1_assoc,l2_assoc,cpu,&get);
            if(get){L1[cmd->owner].push_back(l_block);}
            else
            {
                avoid_null(cmd,memory);
                evict_wrt_back(cmd,memory,L1,L2,l1_size,l2_size,l1_assoc,l2_assoc,cpu);
                load(cmd,memory,L2,l2_size,l2_assoc);
                set_flag(cmd,L2,l2_size,l2_assoc,MODIFIED);//to ensure the block when load in has flag
                load(cmd,&L1[cmd->owner],L2,l1_size,l2_size,l1_assoc,l2_assoc);
                set_flag(cmd,&L1[cmd->owner],l1_size,l1_assoc,MODIFIED);//to ensure the block when load in has flag
            }
        }
        write(cmd,&L1[cmd->owner],l1_size,l1_assoc);
    }
    else { EPRINTF("UNKNOWN CMD\n"); }
    return 0;
}



