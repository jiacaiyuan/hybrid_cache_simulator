//***** ***** ***** ***** ***** ***** ***** ***** ***** ***** *****
//Author:   jcyuan
//E-Mail:   yuan861025184@163.com
//Address:  HIT_MEC
//Project:  hybrid cache simulator
/*Function: the stats.s is the functions of add statistics
The variable need to be added in the head of this file and when using it
The variable need to be extern by user,and user need to add the variable's behavior in right place
and at last need to add the name and the variable in the function of get_stats() */
//***** ***** ***** ***** ***** ***** ***** ***** ***** ***** *****


#include "stats.h"



u64 cmd_counter;


FILE* ini_stats(char* fil_name)
{
    FILE* fp_stats=fopen(fil_name,"a+");
    if(fp_stats==NULL)
    {
        DPRINTF("NO stats FILE\n");
        printf("Error: No stats File\n");
        exit(0);
    }
    return fp_stats;
}



 int add_stats(FILE* fp_stats,u64 stats,char* name)
{
    fprintf(fp_stats,"%s %llu\n",name,stats);
    return 0;
}


int get_stats(char* fil_name)
{
    FILE* fp_stats=ini_stats(fil_name);
    add_stats(fp_stats,cmd_counter,"CMD_COUNTER");
    fclose(fp_stats);
    return 0;
}
