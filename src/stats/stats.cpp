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
    FILE* fp_stats=fopen(fil_name,"w+");
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




void config_out(char* fil_name)
{
    FILE* fp=fopen(fil_name,"w+");
    if(fp==NULL)
    {
        DPRINTF("NO CONFIG FILE\n");
        printf("Error: No config File\n");
        exit(0);
    }
    fprintf(fp,"****************************************\n");
    fprintf(fp,"---------Hybrid Cache Simulator---------\n");
    fprintf(fp,"---------     System Config    ---------\n");
    fprintf(fp,"---------    System Auto Gen   ---------\n");
    fprintf(fp,"****************************************\n");
    fprintf(fp,"\n\n\n\n");
    fprintf(fp,"MEM_SIZE %llu\n",MEM_SIZE);
    fprintf(fp,"CACHELINE_SIZE %d\n",CACHELINE);
    fprintf(fp,"CMD_WIDTH %d\n",CMD_WIDTH);
    fprintf(fp,"CPU_NUM %d\n",CPU);
    #ifdef WRT_THROUGH
        #ifdef LEVEL_TWO
            fprintf(fp,"L2SIZE %d\n",L2SIZE);
            fprintf(fp,"L2_ASSOC %d\n",L2_ASSOC);
        #endif // LEVEL_TWO
        fprintf(fp,"CONSISTNECY WRT_THROUGH\t\t");
        #ifdef WRT_ASSIGN
            fprintf(fp,"WRITE_ASSIGN\t\t");
        #endif // WRT_ASSIGN
        #ifdef N_WRT_ASSIGN
            fprintf(fp,"NO_WRITE_ASSIGN\t\t");
        #endif // N_WRT_ASSIGN
        #ifdef WRT_UPDATE
            fprintf(fp,"WRITE_UPDATE\t\t");
        #endif // WRT_UPDATE
        #ifdef WRT_INVALID
            fprintf(fp,"WRITE_INVALID\t\t");
        #endif // WRT_INVALID
        fprintf(fp,"\n");
        fprintf(fp,"L1SIZE %d\n",L1SIZE);
        fprintf(fp,"L1_ASSOC %d\n",L1_ASSOC);
    #endif // WRT_THROUGH
    #ifdef MESI
        fprintf(fp,"CONSISTNECY MESI\n");
        fprintf(fp,"L1SIZE %d\n",L1SIZE);
        fprintf(fp,"L1_ASSOC %d\n",L1_ASSOC);
        fprintf(fp,"L2SIZE %d\n",L2SIZE);
        fprintf(fp,"L2_ASSOC %d\n",L2_ASSOC);
    #endif // MESI
    #ifdef LRU
        fprintf(fp,"REPLACE LRU\n");
    #endif // LRU
    fclose(fp);
    IPRINTF("WRITE CONFIG FILE\n");
    return;
}
