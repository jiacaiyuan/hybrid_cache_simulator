//***** ***** ***** ***** ***** ***** ***** ***** ***** ***** *****
//Author:   jcyuan
//E-Mail:   yuan861025184@163.com
//Address:  HIT_MEC
//Project:  hybrid cache simulator
/*Function: the type.h is user config about cache_simulator,including
the number of CPU,the Memory Size,the Cache Level,Size,Assoc,CacheLine Size,
the MAX CMD WIDTH,the Policy of Cache-Replace and the Policy of Cache-Consistency
AlSO you can config the DEBUG MODE about it */
//***** ***** ***** ***** ***** ***** ***** ***** ***** ***** *****

#ifndef TYPE_H_INCLUDED
#define TYPE_H_INCLUDED

#define MEM_SIZE ((u64)(0x1)<<32)
#define CACHELINE 64
#define CMD_WIDTH 64 //64 bytes

#define CPU 4

//#define DEBUG
//#define INFO
#define ERROR

#define LEVEL_TWO
//#define L1SIZE 1024
//#define L2SIZE 4096

#define L1SIZE 256
#define L2SIZE 1024


#define  L1_ASSOC 4
#define L2_ASSOC 4


#define WRT_ASSIGN
//#define N_WRT_ASSIGN


#define WRT_UPDATE
//#define WRT_INVALID


//#define WRT_THROUGH
#define LRU


#define MESI



#endif // TYPE_H_INCLUDED
