//***** ***** ***** ***** ***** ***** ***** ***** ***** ***** *****
//Author:   jcyuan
//E-Mail:   yuan861025184@163.com
//Address:  HIT_MEC
//Project:  hybrid cache simulator
/*Function: the stats.h is the functions of add statistics */
//***** ***** ***** ***** ***** ***** ***** ***** ***** ***** *****

#ifndef STATS_H_INCLUDED
#define STATS_H_INCLUDED

#include "../include/struct.h"
#include "../include/type.h"

/*initial the statistics file*/
FILE* ini_stats(char* fil_name);

/*add the variable of stats and define the variable name */
int add_stats(FILE* fp_stats,u64 stats,char* name);

/*do collect the stats variable*/
int get_stats(char* fil_name);

/*auto gen the system config information*/
void config_out(char* fil_name);
#endif // STATS_H_INCLUDED
