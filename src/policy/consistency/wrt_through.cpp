//***** ***** ***** ***** ***** ***** ***** ***** ***** ***** *****
//Author:   jcyuan
//E-Mail:   yuan861025184@163.com
//Project:  hybrid cache simulator
/*Function: read,store and processing Cache */
//***** ***** ***** ***** ***** ***** ***** ***** ***** ***** *****

#include "wrt_through.h"

int check_data(CMD* cmd, CACHEBLCOK* block,int cache_size,int assoc)
{
    int i=0;
    int counter=0;
    u64 addr=align_addr(cmd->addr);
    for(i=0;i<(cmd->lenth);i++)
    {
        if((cmd->data[i])==block->data[(addr+CACHELINE-1)-(cmd->addr+cmd->lenth-1)+i])
        {
            counter=counter+1;
        }
        else{counter=counter;}
    }
    if(counter==cmd->lenth) { return 1;}
    else{
/*            printf("counter=%d\n",counter);
            printf("cmd=%llx ",cmd->addr);
            for(i=0;i<(cmd->lenth);i++){printf("%hx ",cmd->data[i]);}
            printf("\n");

            printf("cache=%llx ",decode_cache(*block,cache_size,assoc));
            for(i=0;i<(CACHELINE);i++){printf("%hx ",block->data[i]);}
            printf("\n");*/
            return 0;
    }
}

int read(CMD* cmd,CACHE* cache,int cache_size,int assoc)
{
    int in;
    int i=0;
    DPRINTF("READ IN CACHE\n");
    CACHEBLCOK* block=find_cache(cmd->addr,cache,cache_size,assoc,&in);
    if(in==1)
    {
        u64 addr=decode_cache(*block,cache_size,assoc);
        if(!check_data(cmd,block,cache_size,assoc))
        {
            IPRINTF("WARNING:READ DATA DIFF\n");
            write(cmd,cache,cache_size,assoc);
            //how to process the different data between cmd and memory in the trace,maybe comment it is better
        }
        for(i=0;i<(cmd->lenth);i++) { cmd->data[i]=block->data[(addr+CACHELINE-1)-(cmd->addr+cmd->lenth-1)+i];}//read the data
    }//note to align the address of command and CacheLine
    else{EPRINTF("READ NO DATA\n");}
    return 0;
}


int write(CMD* cmd,CACHE* cache,int cache_size,int assoc)
{
    int in;
    int i=0;
    DPRINTF("WRITE IN CACHE\n");
    CACHEBLCOK* block=find_cache(cmd->addr,cache,cache_size,assoc,&in);
    if(in==1)
    {
        u64 addr=decode_cache(*block,cache_size,assoc);
        for(i=0;i<(cmd->lenth);i++) { block->data[(addr+CACHELINE-1)-(cmd->addr+cmd->lenth-1)+i]=cmd->data[i];}//write the data
    }
    else{EPRINTF("WRITE NO DATA\n");}
    return 0;
}

int write(CMD* cmd, CACHE* L1, CACHE* L2, int l1_size, int l2_size, int l1_assoc, int l2_assoc,int cpu)
{
    int in;
    int i=0;
    DPRINTF("WRITE IN CACHE\n");
    if((cmd->owner)>cpu){EPRINTF("CPU OVER\n");return 0;}
    for(i=0;i<cpu;i++)
    {
        //CACHEBLCOK* block=find_cache(cmd->addr,&L1[i],l1_size,l1_assoc,&in);//&L1[cpu]=(L1+i)
        find_cache(cmd->addr,&L1[i],l1_size,l1_assoc,&in);
        if((i==(cmd->owner))&(in!=1)){ EPRINTF("WRITE NO DATA\n"); return -1;}//write the owner L1 Cache
        if(in==1)
        {   //Other Level One Cache may has the same Block data
            //if config WRT_UPDATE,other Level one should update the data to keep consistency
            //if config WRT_INVALID,other Level one evict the same block data,to avoid un-consistency
            if(i==(cmd->owner)){write(cmd,&L1[cmd->owner],l1_size,l1_assoc);}
            else
            {
                #ifdef WRT_UPDATE
                    write(cmd,&L1[i],l1_size,l1_assoc);
                #endif // WRT_UPDATE
                #ifdef WRT_INVALID
                    evict_block(cmd->addr,&L1[cmd->owner],l1_size,l1_assoc);
                #endif // WRT_INVALID
            }
        }
    }
    //write through need to update the L2
    write(cmd,L2,l2_size,l2_assoc);
    return 0;
}






int store(CMD* cmd,MEMORY* memory)
{
    std::vector<MEM_CELL>::iterator iter;
    int i=0;
    DPRINTF("STORE TO MEMORY\n");
    for(iter=(*memory).begin();iter!=(*memory).end();iter++)
    {
        u64 a_addr=align_addr(cmd->addr);
        if(a_addr==(*iter).addr)
        {
            for(i=0;i<(cmd->lenth);i++) { (*iter).data[(a_addr+CACHELINE-1)-(cmd->addr+cmd->lenth-1)+i]=cmd->data[i];}
            return 1;
        }
    }
    return 0;
}



int load(CMD* cmd,MEMORY* memory,CACHE* cache,int cache_size,int assoc)
{
    int in;
    DPRINTF("LOAD MEMORY TO L1 CACHE\n");
    if(check_full(cmd->addr,cache,cache_size,assoc)) { evict(cmd->addr,cache,cache_size,assoc);}
    MEM_CELL* cell=find_mem(cmd,memory, &in);
    load_cache(cache,cell,cache_size,assoc);
    return 1;
}


int load(CMD* cmd, CACHE* L1, CACHE* L2, int l1_size, int l2_size, int l1_assoc, int l2_assoc)
{
    int in;
    DPRINTF("LOAD L2 CACHE TO L1 CACHE\n");
    if(check_full(cmd->addr,L1,l1_size,l1_assoc)) { evict(cmd->addr,L1,l1_size,l1_assoc);}
    CACHEBLCOK* block=find_cache(cmd->addr,L2,l2_size,l2_assoc,&in);
    load_cache(L1,block,l1_size,l2_size,l1_assoc,l2_assoc);
    return 1;
}

int load(CMD* cmd,MEMORY* memory,CACHE* L1, CACHE* L2, int l1_size, int l2_size, int l1_assoc, int l2_assoc,int cpu)
{
    int in;
    int i;
    //due to L2 has all L1 data,so if we evict the block in L2 due to full ,
    //if L1 has the same block,we also,need to evict the block in L1 no matter it's full or not
    //and the block that evicted in L1 don't care about the policy of eviction,just the specific Block
    DPRINTF("LOAD MEMORY TO L1&L2 CACHE\n");
    if(check_full(cmd->addr,L2,l2_size,l2_assoc))
    {
        u64 evic_addr=evict(cmd->addr,L2,l2_size,l2_assoc);//follow the LRU
        for(i=0;i<cpu;i++)
        {
            //CACHEBLCOK* block=find_cache(evic_addr,&L1[i],l1_size,l1_assoc,&in);
            find_cache(evic_addr,&L1[i],l1_size,l1_assoc,&in);
            if(in==1){ evict_block(evic_addr,&L1[i],l1_size,l1_assoc); }
        }
    }
    MEM_CELL* cell=find_mem(cmd,memory, &in);
    load_cache(L2,cell,l2_size,l2_assoc);//load the data into L2 cache
    load(cmd,&L1[cmd->owner], L2,l1_size,l2_size,l1_assoc,l2_assoc);//load the L2 to the owner L1
    return 1;
}




int avoid_null(CMD* cmd,MEMORY* memory)
{
    int in;
    int i=0;
    u64 addr=align_addr(cmd->addr);
    //MEM_CELL* cell=find_mem(cmd,memory, &in);
    find_mem(cmd,memory, &in);
    if(in==0)
    {   //to avoid the bug of trace that command may read and write the data that not in the memory
        //so if command data not in memory,we need to create a memory cell into the memory
        MEM_CELL new_cell;
        new_cell.addr=align_addr(cmd->addr);
        new_cell.lenth=CACHELINE;
        for(i=0;i<CACHELINE;i++) { new_cell.data[i]=0;}
        for(i=0;i<(cmd->lenth);i++) { new_cell.data[(addr+CACHELINE-1)-(cmd->addr+cmd->lenth-1)+i]=cmd->data[i];}
        (*memory).push_back(new_cell);
        store(cmd,memory);
        IPRINTF("WARNING:MEMORY MISS\n");
    }
    return 0;
}




int wrt_through(CMD* cmd,MEMORY* memory,CACHE* cache,int cache_size,int assoc)
{
    int in;
    CACHEBLCOK* block=find_cache(cmd->addr,cache,cache_size,assoc,&in);
    if(in==1)//in the L1 Cache
    {
        if(cmd->op==0)//operation is read
        {
            read(cmd,cache,cache_size,assoc);
        }
        else if(cmd->op==1)//operation is write
        {
            //write through
            write(cmd,cache,cache_size,assoc);
            store(cmd,memory);
        }
        else{EPRINTF("UNKNOWN CMD\n");}
        update_lru(cache,block);//the policy of LRU
    }
    else//not in the Cache
    {
        avoid_null(cmd,memory);
        if(cmd->op==0)//the operation is read
        {
            load(cmd,memory,cache,cache_size,assoc);
            read(cmd,cache,cache_size,assoc);
        }
        else if(cmd->op==1)//the operation is write
        {//the action is different when to config the WRT_ASSIGN or N_WRT_ASSIGN
         //if config WRT_ASSIGN we need to load to cache and write and store to memory
         //if config N_WRT_ASSIGN we just write the memory is ok
            #ifdef WRT_ASSIGN
                load(cmd,memory,cache,cache_size,assoc);
                write(cmd,cache,cache_size,assoc);
                store(cmd,memory);
            #endif // WRT_ASSIGN
            #ifdef N_WRT_ASSIGN
                store(cmd,memory);
            #endif // N_WRT_ASSIGN
        }
        else{EPRINTF("UNKNOWN CMD\n");}
    }
    return 0;
}


int wrt_through(CMD* cmd,MEMORY* memory,CACHE* L1,CACHE* L2,int l1_size,int l2_size,int l1_assoc,int l2_assoc,int cpu)
{
    int in;
    int i;
    CACHEBLCOK* block=find_cache(cmd->addr,&L1[cmd->owner],l1_size,l1_assoc,&in);
    if(in==1)//in the L1 CACHE
    {
        if(cmd->op==0)//the operation is read
        {
            read(cmd,&L1[cmd->owner],l1_size,l1_assoc);
        }
        else if(cmd->op==1)//the operation is write
        {
            //write through
            write(cmd,L1,L2,l1_size,l2_size,l1_assoc,l2_assoc,cpu);
            store(cmd,memory);
        }
        else{EPRINTF("UNKNOWN CMD\n");}
        for(i=0;i<cpu;i++)
        {
            block=find_cache(cmd->addr,&L1[i],l1_size,l1_assoc,&in);
            if(in==1) { update_lru(&L1[i],block); }
        }
        block=find_cache(cmd->addr,L2,l2_size,l2_assoc,&in);
        if(in){update_lru(L2,block);}
        else{EPRINTF("L2 CACHE LOSS DATA\n");}
    }
    else//don't in the L1 Cache
    {
        block=find_cache(cmd->addr,L2,l2_size,l2_assoc,&in);
        if(in==1)//in l2_cache
        {
            load(cmd,&L1[cmd->owner],L2,l1_size,l2_size,l1_assoc,l2_assoc);//load the data from L2 to L1
            if(cmd->op==0){ read(cmd,&L1[cmd->owner],l1_size,l1_assoc); }//the operation is read
            else if(cmd->op==1)//the operation is write
            {
                write(cmd,L1,L2,l1_size,l2_size,l1_assoc,l2_assoc,cpu);
                store(cmd,memory);
            }
            else{EPRINTF("UNKNOWN CMD\n");}
            update_lru(L2,block);
        }
        else
        {
            avoid_null(cmd,memory);
            if(cmd->op==0)//the operation is read
            {
                load(cmd,memory,L1,L2,l1_size,l2_size,l1_assoc,l2_assoc,cpu);
                read(cmd,&L1[cmd->owner],l1_size,l1_assoc);
            }
            else if(cmd->op==1)//the operation is write
            {
                #ifdef WRT_ASSIGN
                load(cmd,memory,L1,L2,l1_size,l2_size,l1_assoc,l2_assoc,cpu);
                write(cmd,L1,L2,l1_size,l2_size,l1_assoc,l2_assoc,cpu);
                store(cmd,memory);
                #endif // WRT_ASSIGN
                #ifdef N_WRT_ASSIGN
                    store(cmd,memory);
                #endif // N_WRT_ASSIGN
            }
            else{EPRINTF("UNKNOWN CMD\n");}
        }
    }
    return 0;
}
