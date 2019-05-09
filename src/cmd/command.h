//***** ***** ***** ***** ***** ***** ***** ***** ***** ***** *****
//Author:   jcyuan
//E-Mail:   yuan861025184@163.com
//Project:  hybrid cache simulator
/*Function: read,store and processing command */
//***** ***** ***** ***** ***** ***** ***** ***** ***** ***** *****

#ifndef COMMAND_H_INCLUDED
#define COMMAND_H_INCLUDED

#include "../include/type.h"
#include "../include/struct.h"
#include "../mem/memory.h"

#define CMD_THRESHOLD 100

/*demo about the read the command file*/
int test_cmd();

/*read the CMD file to the CMD queue in the format of
cycle operation addr lenth data0 data1 ...... owner
cycle: the time of simulator time
operation: R/W read or write
addr: the command addr, maybe don't align the CacheLine
lenth: the data lenth in Byte
data0---datai: the data in each Byte
        the data0 is the MSB in addr+lenth-1
        the data... is the LSB in addr
owner: the CPU of command send
*/
int cmd_file(char* fil_name,CMD_QUEUE* cmd_queue);

/*display the command queue*/
void display(CMD_QUEUE* cmd_queue);

#endif // COMMAND_H_INCLUDED
