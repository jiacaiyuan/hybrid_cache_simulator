//***** ***** ***** ***** ***** ***** ***** ***** ***** ***** *****
//Author:   jcyuan
//E-Mail:   yuan861025184@163.com
//Project:  hybrid cache simulator
/*Function: read,store and processing memory */
//***** ***** ***** ***** ***** ***** ***** ***** ***** ***** *****

#ifndef MEMORY_H_INCLUDED
#define MEMORY_H_INCLUDED

#include "../include/type.h"
#include "../include/struct.h"

#define MEM_THRESHOLD 100

/*demo about the read the memory file*/
int test_mem();

/*check the address in memory file if to overflow to the Memory Size setting in the type*/
int check_overflow(u64* addr);

/*decode the address to judge if it's PCM or DRAM*/
int decode(u64* addr);

/*read the Memory file to the memory vector in the format of
 addr lenth data0 data1 ......
addr: the command addr, maybe don't align the CacheLine
lenth: the data lenth in Byte
data0---datai: the data in each Byte
        the data0 is the MSB in addr+lenth-1
        the data... is the LSB in addr
*/
int mem_file(char* fil_name,MEMORY* memory);

/*align the address to the CacheLine Address to make the address in the CacheLine*/
u64 align_addr(u64 addr);

/*align the memory line to completion in a CacheLine, if no data using 0x0 to default it */
void align_mem(MEM_CELL* cell);

/*due to the align mem, some memory line may align same CacheLine with different data,
So need to merge them into one CacheLine and replace the default data 0x0 to the real value */
int merge_mem(MEM_CELL* cell,MEMORY* memory);

/*find if the addr that CMD need in the memory vector*/
MEM_CELL* find_mem(CMD* cmd,MEMORY* memory, int* in);

/*display the memory vector*/
void display(MEMORY* memory);



#endif // MEMORY_H_INCLUDED
