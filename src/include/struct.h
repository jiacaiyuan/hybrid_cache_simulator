//***** ***** ***** ***** ***** ***** ***** ***** ***** ***** *****
//Author:   jcyuan
//E-Mail:   yuan861025184@163.com
//Address:  HIT_MEC
//Project:  hybrid cache simulator
/*Function: the struct.h is the base config of cache_simulator,
define the data struct of Cache,Memory,CMD and the input format in
different OS include the Linux and Windows,Also it define the Debug
base function about the project*/
//***** ***** ***** ***** ***** ***** ***** ***** ***** ***** *****


#ifndef STRUCT_H_INCLUDED
#define STRUCT_H_INCLUDED
#include <vector>
#include <list>
#include<iterator>
#include <iostream>
#include<stdlib.h>
#include<stdio.h>
#include<math.h>

#include "type.h"


#define WINDOWS
#ifdef WINDOWS
    #define llx I64x
    #define llu I64u
    #define lld I64d
#endif // WINDOWS

#ifdef LINUX
    #define llx  llx
    #define llu llu
    #define lld lld
#endif // LINUX

#define LOG2(N) (int)(log(N)/log(2))
#define u64 unsigned long long int
#define u32 unsigned int
#define u16 unsigned short
#define u8 char


#ifdef DEBUG
#define DPRINTF(fmt, ...) \
        do { fprintf(stdout, "DEBUG: [%24s:%5d] " fmt, __func__, __LINE__, \
                ## __VA_ARGS__); } while (0)
#else
#define DPRINTF(fmt, ...) do {} while (0)
#endif



#ifdef INFO
#define IPRINTF(fmt, ...) \
        do { fprintf(stdout, "INFO: " fmt,\
                ## __VA_ARGS__); } while (0)
#else
#define IPRINTF(fmt, ...) do {} while (0)
#endif


#ifdef ERROR
#define EPRINTF(fmt, ...) \
        do { fprintf(stderr, "ERROR: " fmt,\
                ## __VA_ARGS__); } while (0)
#else
#define EPRINTF(fmt, ...) do {} while (0)
#endif


typedef struct __mem_cell
{
    u64 addr;
    int lenth;
    u16 data[CACHELINE]; //the cell is one CacheLine
} MEM_CELL;

typedef std::vector<MEM_CELL> MEMORY;




typedef struct __cmd
{
    u64 cycle;
    bool op;//0 for read and 1 for write
    u64 addr;
    int lenth;
    u16 data[CMD_WIDTH];
    int owner;
    bool operator < (__cmd &b){return cycle<=b.cycle;}
}CMD;

typedef std::list<CMD> CMD_QUEUE;





typedef struct __cacheblock
{
    u32 flag;//for expand other functions
    u64 tagger;
    u64 index;
    u16 data[CACHELINE];
}CACHEBLCOK;

typedef std::list<CACHEBLCOK> CACHE;



#endif // STRUCT_H_INCLUDED
