//***** ***** ***** ***** ***** ***** ***** ***** ***** ***** *****
//Author:   jcyuan
//E-Mail:   yuan861025184@163.com
//Project:  hybrid cache simulator
/*Function: read,store and processing Cache */
//***** ***** ***** ***** ***** ***** ***** ***** ***** ***** *****

#ifndef CACHE_H_INCLUDED
#define CACHE_H_INCLUDED

#include "../include/type.h"
#include "../include/struct.h"
#include "../mem/memory.h"



/*calculate the tagger and index and lsb bit in different CacheSize and ASSOC Group*/
int calc_bit(u32* tagger_bit,u32*index_bit,u32*lsb_bit,int cache_size,int assoc);

/*for a Cache Block, get the begin addr=align addr of Cache Block*/
u64 decode_cache(CACHEBLCOK block,int cache_size,int assoc);

/*find the CacheBlock in the List of Cache follow the addr*/
CACHEBLCOK* find_cache(u64 addr,CACHE *cache,int cache_size,int assoc,int* in);

/*get the tagger and index of a CacheBlock follow the given addr*/
int decode_addr(u64 addr,u64* tagger,u64* index,int cache_size,int assoc);

/*check the Cache if is full in the ASSOC GROUP follow the addr*/
int check_full(u64 addr,CACHE* cache,int cache_size,int assoc);

/*load the CacheBlock from Memory*/
void load_cache(CACHE* cache,MEM_CELL* cell,int cache_size,int assoc);

/*load the First Level Cache from LLC*/
void load_cache(CACHE* cache,CACHEBLCOK* block,int l2_size,int l1_size,int l1_assoc,int l2_assoc);

/*Evict the block of specific in the Cache*/
u64 evict_block (u64 addr,CACHE* cache,int cache_size,int assoc);

/*display the Cache List*/
void display(CACHE* cache,int cache_size,int assoc);









#endif // CACHE_H_INCLUDED
