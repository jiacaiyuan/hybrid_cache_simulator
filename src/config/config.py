#***** ***** ***** ***** ***** ***** ***** ***** ***** ***** *****
#Author:   jcyuan
#E-Mail:   yuan861025184@163.com
#Project:  hybrid cache simulator
#Function: config the Hybrid Cache Simulator
#***** ***** ***** ***** ***** ***** ***** ***** ***** ***** *****
import os
import sys
import re
from optparse import OptionParser
from cfg_process import process_size
from cfg_process import config_fil
print("\n\n\n\n")
print(sys.version)
os.system("python -V")
usage="%prog [options]"
version="1.0"
info="""
Author:   jcyuan        
E-Mail:   yuan861025184@163.com        
Project:  hybrid cache simulator        
Function: Config the simulator by the file or manual   
"""
parser=OptionParser(usage,description=info,version="%prog "+version)

#---------------------------------------------------------------------------------
#-------------------------------------Config the Hybrid Cache Simulator-----------
#---------------------------------------------------------------------------------
#can add the config file 
parser.add_option("-f","--file",dest="cfg_fil",help="input the config file of Hybrid Cache Simulator",type='string',default="config.cfg")
parser.add_option("-F","--fil_mode",dest="fil_mode",help="read the config file",action="store_true")
#config the memory size
parser.add_option("-m","--mem",dest="mem_size",help="memory size in the format 4GB, 1024MB, 512KB",type="string",default="4GB")
#config the cache line size
parser.add_option("-L","--line",dest="cache_line",help="CacheLine Size the format is byte 64,32",type="int",default=64)
#config the max command size
parser.add_option("-c","--cmd",dest="cmd_size",help="max command width x Bytes",type="int",default=64)
#config the number of cpu
parser.add_option("-n","--cpu",dest="cpu_num",help="the number of CPU",type="int",default=4)
#config the cache level one level or two level
parser.add_option("-l","--level",dest="cache_level",help="input cache level 1 or 2",type="int",default=2)
#config the L1 Cache Size
parser.add_option("-s","--l1_size",dest="l1_size",help="input L1 Cache Size 1024MB, 512KB, 256B",type="string",default="256B")
#config the L2 Cache Size
parser.add_option("-S","--l2_size",dest="l2_size",help="input L2 Cache Size 1024MB, 512KB, 256B",type="string",default="1024B")
#config the L1 Cache Assoc
parser.add_option("-a","--l1_assoc",dest="l1_assoc",help="input L1 Cache Assoc 1,2,4,8...and all",type="string",default="4")
#config the L2 Cache Assoc
parser.add_option("-A","--l2_assoc",dest="l2_assoc",help="input L2 Cache Assoc 1,2,4,8...and all",type="string",default="4")
#config the write through of write assign
parser.add_option("-w","--wrt_ass",dest="wrt_ass",help="write assign or not write assign",action="store_true")
#config the write through of write update or write invalid
parser.add_option("-u","--wrt_upd",dest="wrt_upd",help="write update or write invalid",action="store_true")
#config the consistency protocol of write through or MESI
parser.add_option("-p","--protocol",dest="protocol",help="the protocol of simulator MESI or WRT_THROUGH",type="string",default="MESI")
#config the replace protocol of LRU
parser.add_option("-r","--replace",dest="replace",help="Cache replace policy",type="string",default="LRU")
#config the debug info
parser.add_option("-d","--debug",dest="debug",help="simulator info of debug",action="store_true")
parser.add_option("-i","--info",dest="info",help="simulator info of simple information",action="store_true")
parser.add_option("-e","--error",dest="error",help="simulator info of error",action="store_true")
#config windows mode
parser.add_option("-W","--windows",dest="windows",help="simulator in the OS of windows",action="store_true")
(options,args)=parser.parse_args()

c_file_info="""
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
"""

if(options.fil_mode):
	print("Config File Mode\n")
	#os.system("python config_fil.py -f "+options.cfg_fil)
	if(not options.windows):
		os.mknod("type.h")
	out=open("type.h","w")
	out.write(c_file_info+"\n\n")
	out.write("#ifndef TYPE_H_INCLUDED\n")
	out.write("#define TYPE_H_INCLUDED\n\n")
	config_fil(options.cfg_fil,out)
else:
	if(not options.windows):
		os.mknod("type.h")
	out=open("type.h","w")
	out.write(c_file_info+"\n\n")
	out.write("#ifndef TYPE_H_INCLUDED\n")
	out.write("#define TYPE_H_INCLUDED\n\n")
	if(options.debug):
		out.write("#define DEBUG\n")
	if(options.info):
		out.write("#define INFO\n")
	if(options.error):
		out.write("#define ERROR\n\n")

	out.write("#define MEM_SIZE "+str(int(process_size(options.mem_size)/(options.cache_line))*(options.cache_line))+"\n")
	out.write("#define CACHELINE "+str(options.cache_line)+"\n")
	out.write("#define CMD_WIDTH "+str(options.cmd_size)+"\n")
	out.write("#define CPU "+str(options.cpu_num)+"\n")
	out.write("\n\n")

	#size calculate
	l1_size=int(process_size(options.l1_size)/(options.cache_line))*(options.cache_line)
	l2_size=int(process_size(options.l2_size)/(options.cache_line))*(options.cache_line)
	l1_all_assoc=int(process_size(options.l1_size)/(options.cache_line))
	l2_all_assoc=int(process_size(options.l2_size)/(options.cache_line))

	if(options.protocol=="MESI"):
		out.write("#define MESI\n")
		out.write("#define LEVEL_TWO\n")
		out.write("#define L1SIZE "+str(l1_size)+"\n")
		out.write("#define L2SIZE "+str(l2_size)+"\n")
		if(re.search("all",options.l1_assoc,re.IGNORECASE)):
			out.write("#define L1_ASSOC "+str(l1_all_assoc)+"\n")
		else:
			out.write("#define L1_ASSOC "+options.l1_assoc+"\n")
		if(re.search("all",options.l2_assoc,re.IGNORECASE)):
			out.write("#define L2_ASSOC "+str(l2_all_assoc)+"\n")
		else:
			out.write("#define L2_ASSOC "+options.l2_assoc+"\n")


	elif(options.protocol=="WRT_THROUGH"):
		out.write("#define WRT_THROUGH\n")
		if(options.wrt_ass):
			out.write("#define WRT_ASSIGN\n")
		else:
			out.write("#define N_WRT_ASSIGN\n")
		if(options.wrt_upd):
			out.write("#define WRT_UPDATE\n")
		else:
			out.write("#define WRT_INVALID\n")
		if(options.cache_level==1):
			out.write("#define L1SIZE "+str(l1_size)+"\n")
			if(re.search("all",options.l1_assoc,re.IGNORECASE)):
				out.write("#define L1_ASSOC "+str(l1_all_assoc)+"\n")
			else:
				out.write("#define L1_ASSOC "+options.l1_assoc+"\n")
		elif(options.cache_level==2):
			out.write("#define LEVEL_TWO\n")
			out.write("#define L1SIZE "+str(l1_size)+"\n")
			out.write("#define L2SIZE "+str(l2_size)+"\n")
			if(re.search("all",options.l1_assoc,re.IGNORECASE)):
				out.write("#define L1_ASSOC "+str(l1_all_assoc)+"\n")
			else:
				out.write("#define L1_ASSOC "+options.l1_assoc+"\n")
			if(re.search("all",options.l2_assoc,re.IGNORECASE)):
				out.write("#define L2_ASSOC "+str(l2_all_assoc)+"\n")
			else:
				out.write("#define L2_ASSOC "+options.l2_assoc+"\n")
	else:
		print("WARNING: UNKNOWN PROTOCOL\n USING MESI DEFAULT\n")
		out.write("#define MESI\n")
		out.write("#define LEVEL_TWO\n")
		out.write("#define L1SIZE "+str(l1_size)+"\n")
		out.write("#define L2SIZE "+str(l2_size)+"\n")
		if(re.search("all",options.l1_assoc,re.IGNORECASE)):
			out.write("#define L1_ASSOC "+str(l1_all_assoc)+"\n")
		else:
			out.write("#define L1_ASSOC "+options.l1_assoc+"\n")
		if(re.search("all",options.l2_assoc,re.IGNORECASE)):
			out.write("#define L2_ASSOC "+str(l2_all_assoc)+"\n")
		else:
			out.write("#define L2_ASSOC "+options.l2_assoc+"\n")
		
	if(re.search("lru",options.replace,re.IGNORECASE)):
		out.write("#define LRU\n")
	else:
		print("no other replace policy just LRU\n")
		out.write("#define LRU\n")
out.write("#endif // TYPE_H_INCLUDED\n")
out.close()
