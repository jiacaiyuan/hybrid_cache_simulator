//***** ***** ***** ***** ***** ***** ***** ***** ***** ***** *****
//Author:   jcyuan
//E-Mail:   yuan861025184@163.com
//Project:  hybrid cache simulator
/*Function: Main */
//***** ***** ***** ***** ***** ***** ***** ***** ***** ***** *****

#include <iostream>
#include<stdio.h>
#include<list>
#include<iterator>
#include<string.h>
#include<stdlib.h>
#include<math.h>

#include "mem/memory.h"
#include "include/type.h"
#include "cmd/command.h"
#include "cache/cache.h"
#include "stats/stats.h"
#include "policy/consistency/wrt_through.h"
#include "policy/replace/lru.h"
#include "policy/consistency/mesi.h"


int main(int argc,char* argv[])
{
    std::cout << "PROCESS STRAT" << std::endl;
    MEMORY memory;
    CMD_QUEUE cmd_queue;
    char* mem_fil="memory_all.txt";
    char* cmd_fil="cmd.txt";
    char* stats_fil="stats.txt";
   // char* mem_fil=argv[1];
   // char* cmd_fil=argv[2];
   // char* stats_fil=argv[3];
    //initial------------------------------------
    std::cout << "PROCESS MEMORY" << std::endl;
    mem_file(mem_fil,&memory);
    std::cout << "PROCESS COMMAND" << std::endl;
    cmd_file(cmd_fil,&cmd_queue);
    #ifdef LEVEL_TWO
        CACHE L2;
        CACHE L1[CPU];
    #else
        CACHE L1[1];//one share
    #endif // LEVEL_TWO
    std::cout<<"INITIAL SUCCESS\n"<<std::endl;
    //----------------------------initial ok-----------
    std::list<CMD>::iterator iter;
    #ifdef WRT_THROUGH
        for(iter=cmd_queue.begin();iter!=cmd_queue.end();iter++)
        {
            extern u64 cmd_counter;
            cmd_counter=cmd_counter+1;
            if(cmd_counter%CMD_THRESHOLD==0){IPRINTF("EXE %llu\n",cmd_counter);}
            #ifdef LEVEL_TWO
                wrt_through(&(*iter),&memory,L1,&L2, L1SIZE, L2SIZE, L1_ASSOC, L2_ASSOC,CPU);
            #else
                wrt_through(&(*iter),&memory,&L1[0],L1SIZE,L1_ASSOC);
            #endif // LEVEL_TWO
        }
    #endif // WRT_THROUGH
    #ifdef MESI
        for(iter=cmd_queue.begin();iter!=cmd_queue.end();iter++)
        {
            extern u64 cmd_counter;
            cmd_counter=cmd_counter+1;
            if(cmd_counter%CMD_THRESHOLD==0){IPRINTF("EXE %llu\n",cmd_counter);}
            mesi(&(*iter),&memory,L1,&L2,L1SIZE,L2SIZE,L1_ASSOC,L2_ASSOC,CPU);
        }
    #endif // MESI
    std::cout<<"PROCESS FINISH\n"<<std::endl;
    get_stats(stats_fil);
    std::cout<<"STATS FINISH\n"<<std::endl;
//    display(&L1[0],L1SIZE,L1_ASSOC);
//    display(&L2,L2SIZE,L2_ASSOC);
//    display(&memory);
    return 0;
}








