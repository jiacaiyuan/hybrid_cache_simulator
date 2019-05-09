//***** ***** ***** ***** ***** ***** ***** ***** ***** ***** *****
//Author:   jcyuan
//E-Mail:   yuan861025184@163.com
//Project:  hybrid cache simulator
/*Function: read,store and processing memory */
//***** ***** ***** ***** ***** ***** ***** ***** ***** ***** *****

#include "memory.h"


//memory format
// addr lenth data0 data1 ......
int test_mem()
{
	u64 a[10];
	u64 b[10];
	char c;
	FILE *fpRead=fopen("test.txt","r");
	if(fpRead==NULL)
	{
		return 0;
	}
	for(int i=0;!feof(fpRead);i++)
    {
        fscanf(fpRead,"%llx %llx ",&a[0],&a[1]);
        printf("%llx %llx\n",a[0],a[1]);
 //       std::cout<<a[0]<<" "<<a[1]<<std::endl;
        for(int i=0;i<a[1];i++)
        {
            if(i==a[1]-1)
            {
                fscanf(fpRead,"%llx\n",&b[i]);
                 c= (char)(b[i]&0xff);
            }
            else
            {
                fscanf(fpRead,"%llx ",&b[i]);
                 c= (char)(b[i]&0xff);
            }
            printf("%x ",c);
        }
        printf("\n");
    }
	return 1;
}

int check_overflow(u64* addr)
{
    if((*addr)>MEM_SIZE)
    {
        EPRINTF("MEMORY ADDR OVERFLOW\n");
        return 1;
    }
    return 0;
}

int decode(u64* addr)
{
    return 0;
}

u64 align_addr(u64 addr)
{
    u32 lsb_bit=LOG2(CACHELINE);
    return (addr>>lsb_bit)<<lsb_bit;
}

void align_mem(MEM_CELL* cell)
{
    u64 a_addr=align_addr(cell->addr);
    u16 temp [CACHELINE];
    unsigned int i=0;
    if((cell->addr==a_addr)&(cell->lenth==CACHELINE))
    {
        return;
    }
    else
    {
        if((cell->lenth>CACHELINE)|((cell->addr)<a_addr)|(((cell->addr)+(cell->lenth))>(a_addr+CACHELINE)))
        {
            EPRINTF("MEMORY CELL ILLEGAL\n");
            return;
        }
        else
        {
            //data[] |0------------------------------------------------CACHELINE-1|
            //       |a_addr+CACHELINE-1------------------------------------a_addr|
            //       |------------addr+lenth-1------addr--------------------------|
            //       |----------------0-------------lenth-1-----------------------|
            for(i=0;i<CACHELINE;i++)//follow the addr to align data
            {
                if(i<a_addr+CACHELINE-1-((cell->addr)+(cell->lenth)-1))
                {
                    temp[i]=0x0;
                }
                else if((i>=a_addr+CACHELINE-1-((cell->addr)+(cell->lenth)-1))&(i<=a_addr+CACHELINE-1-cell->addr))
                {
                    temp[i]=cell->data[i-(a_addr+CACHELINE-1-((cell->addr)+(cell->lenth)-1))];
                }
                else
                {
                    temp[i]=0x0;
                }
            }
            cell->lenth=CACHELINE;
            cell->addr=a_addr;
            for(i=0;i<CACHELINE;i++)//get the data
            {
                cell->data[i]=temp[i];
            }
        }
    }
    return;
}

int merge_mem(MEM_CELL* cell,MEMORY* memory)
{
    std::vector<MEM_CELL>::iterator iter;
    int i=0;
    for(iter=(*memory).begin();iter!=(*memory).end();iter++)
    {
        if((*iter).addr==(cell->addr))
        {
            for(i=0;i<CACHELINE;i++)
            {
                if((*iter).data[i]!=cell->data[i])
                {
                    if(((*iter).data[i]!=0)&(cell->data[i]!=0)){IPRINTF("WRANING:MEMORY DATA MERGE\n");}
                    //two different data in same addr follow the first data
                    else{(*iter).data[i]=(*iter).data[i]|(cell->data[i]);}//the default data is 0x0
                }
            }
            return 1;
        }
    }
    return 0;
}



int mem_file(char* fil_name,MEMORY* memory)
{
    int i,j=0;
    MEM_CELL cell;
    FILE* fpread=fopen(fil_name,"r");
    if(fpread==NULL)
    {
        EPRINTF("NO MEMORY FILE\n");
        exit(0);
    }
    for(i=0;!feof(fpread);i++)
    {
        if(i%MEM_THRESHOLD==0){IPRINTF("MEM %d\n",i);}
        fscanf(fpread,"%llx ",&cell.addr);//the 64bit is llu,llx or lld
        if(check_overflow(&cell.addr))
        {
            EPRINTF("MEMORY ADDR OVERFLOW\n");
            std::cout<<cell.addr<<std::endl;
            return 1;
        }
        else
        {
            fscanf(fpread,"%d ",&cell.lenth);//32 bit is d,x,u
            //std::cout<<cell.addr<<"  "<<cell.lenth<<std::endl;
            for(j=0;j<cell.lenth;j++)
            {
                if(j==(cell.lenth-1)){fscanf(fpread,"%hx\n",&cell.data[j]);}//16 bit is hx,hd,hu
                else              {fscanf(fpread,"%hx ",&cell.data[j]);}
            }
            align_mem(&cell);
            if(!merge_mem(&cell,memory)) { (*memory).push_back(cell); }
            //if no same CacheLine then push a new cell ,else merge the exist CacheLine
        }
    }
    fclose(fpread);
    #ifdef DEBUG
        int k;
        std::vector<MEM_CELL>::iterator iter;
        for(iter=(*memory).begin();iter!=(*memory).end();iter++)
        {
            printf("ADDR:%llx,LEN:%d\n",(*iter).addr,(*iter).lenth);
            for(k=0;k<(*iter).lenth;k++) { printf("%hx ",(*iter).data[k]);}
            printf("\n");
        }
    #endif // DEBUG
    return 0;
}

MEM_CELL* find_mem(CMD* cmd,MEMORY* memory, int* in)
{
    u64 ca_addr=align_addr(cmd->addr);
    std::vector<MEM_CELL>::iterator iter;
    for(iter=(*memory).begin();iter!=(*memory).end();iter++)
    {
        u64 ma_addr=align_addr((*iter).addr);
        if(ma_addr==ca_addr)
        {
            //DPRINTF("CMD IN MEMORY\n");
            *in=1;
            return &(*iter);
        }

    }
    *in=0;
    return NULL;
}



void display(MEMORY* memory)
{
    int k;
    std::vector<MEM_CELL>::iterator iter;
    for(iter=(*memory).begin();iter!=(*memory).end();iter++)
    {
        printf("ADDR:%llx,LEN:%d\n",(*iter).addr,(*iter).lenth);
        for(k=0;k<(*iter).lenth;k++) { printf("%hx ",(*iter).data[k]);}
        printf("\n");
    }
    return;
}





