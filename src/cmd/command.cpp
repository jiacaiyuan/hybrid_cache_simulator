//***** ***** ***** ***** ***** ***** ***** ***** ***** ***** *****
//Author:   jcyuan
//E-Mail:   yuan861025184@163.com
//Project:  hybrid cache simulator
/*Function: read,store and processing command */
//***** ***** ***** ***** ***** ***** ***** ***** ***** ***** *****

#include "command.h"
/*cmd format
 cycle operation addr lenth data0 data1 ...... owner*/


int test_cmd()
{
    FILE* fp_cmd=fopen("cmd.txt","r");
    CMD cmd;
    char a;
    fscanf(fp_cmd,"%llu ",&cmd.cycle);
    fscanf(fp_cmd,"%c ",&a);
    printf("%llu %c\n",cmd.cycle,a);
    if(a=='R'){printf("read\n");}
    else if(a=='W'){printf("write\n");}
    return 0;
}



int cmd_file(char* fil_name,CMD_QUEUE* cmd_queue)
{
    CMD cmd;
    int i=0,j=0;
    FILE* fp_cmd=fopen(fil_name,"r");
    if(fp_cmd==NULL)
    {
        EPRINTF("NO CMD FILE\n");
        exit(0);
    }
    for(i=0;!feof(fp_cmd);i++)//each line is one command
    {
        if(i%CMD_THRESHOLD==0){IPRINTF("CMD %d\n",i);}
        char operation;
        fscanf(fp_cmd,"%llu %c %llx ",&cmd.cycle,&operation,&cmd.addr);
        if(operation=='R'){cmd.op=0;}//the cmd.op 0 is read and 1 is for write
        else if(operation=='W'){cmd.op=1;}
        else{EPRINTF("CMD OPERATION\n");}
        if(check_overflow(&cmd.addr))
        {
            EPRINTF("CMD ADDR OVERFLOW\n");
            std::cout<<cmd.addr<<std::endl;
            return 1;
        }
        else
        {
            fscanf(fp_cmd,"%d ",&cmd.lenth);
            if(cmd.lenth>CMD_WIDTH)
            {
                    EPRINTF("CMD LENTH OVER\n");
                    return 1;
            }
            for(j=0;j<cmd.lenth;j++) { fscanf(fp_cmd,"%hx ",&cmd.data[j]);}
            fscanf(fp_cmd,"%d\n",&cmd.owner);
            (*cmd_queue).push_back(cmd);
        }
    }
    fclose(fp_cmd);//finish the command read
    (*cmd_queue).sort();//sort by the cycle from little to big
    #ifdef DEBUG//for display
        int k;
        std::list<CMD>::iterator iter;
        for(iter=(*cmd_queue).begin();iter!=(*cmd_queue).end();iter++)
        {
            std::cout<<"cycle:"<<(*iter).cycle<<",operation:"<<(*iter).op<<",owner:"<<(*iter).owner<<std::endl;
            printf("ADDR:%llx,LEN:%d\n",(*iter).addr,(*iter).lenth);
            for(k=0;k<(*iter).lenth;k++) { printf("%x ",(*iter).data[k]);}
            printf("\n");
        }
    #endif // DEBUG

    return 0;
}



void display(CMD_QUEUE* cmd_queue)
{
    int k;
    std::list<CMD>::iterator iter;
    for(iter=(*cmd_queue).begin();iter!=(*cmd_queue).end();iter++)
    {
        std::cout<<"cycle:"<<(*iter).cycle<<",operation:"<<(*iter).op<<",owner:"<<(*iter).owner<<std::endl;
        printf("ADDR:%llx,LEN:%d\n",(*iter).addr,(*iter).lenth);
        for(k=0;k<(*iter).lenth;k++) { printf("%hx ",(*iter).data[k]);}
        printf("\n");
    }
    return;
}
