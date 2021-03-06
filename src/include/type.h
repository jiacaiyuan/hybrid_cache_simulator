
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

#define CACHELINE 64
#define CMD_WIDTH 64
#define CPU 4
#define MEM_SIZE 4294967296

#define MESI 
#define LEVEL_TWO 
#define L2SIZE 1024
#define L2_ASSOC 4
#define L1SIZE 256
#define L1_ASSOC 4

#define LRU 
#define INFO
#define DEBUG
#define ERROR
#endif // TYPE_H_INCLUDED
