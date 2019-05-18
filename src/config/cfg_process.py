#***** ***** ***** ***** ***** ***** ***** ***** ***** ***** *****
#Author:   jcyuan
#E-Mail:   yuan861025184@163.com
#Project:  hybrid cache simulator
#Function: base func of config the Hybrid Cache Simulator
#***** ***** ***** ***** ***** ***** ***** ***** ***** ***** *****
import os
import sys
import re
def process_size(parameter):
	if(("GB" in parameter) or ("G" in parameter)):#re.search("GB",parameter,re.IGNORECASE)
		outp=re.sub("\D","",parameter)
		finial=int(int(outp)*1024*1024*1024)
	elif(("MB" in parameter) or ("M" in parameter)):
		outp=re.sub("\D","",parameter)
		finial=int(int(outp)*1024*1024)
	elif(("KB" in parameter) or ("K" in parameter)):
		outp=re.sub("\D","",parameter)
		finial=int(int(outp)*1024)
	else:
		print("the default unit is Byte\n")
		outp=re.sub("\D","",parameter)
		finial=int(outp)
	return finial


def key_word(cfg_fil,key):
	lines=open(cfg_fil,"r").readlines()
	for line in lines:
		if ((line=="") or ("#" in line)):
			continue
		if(re.search(key,line,re.IGNORECASE)):
			outp=re.sub(key,"",line)
			return outp
	return -1

	



def config_fil(cfg_file,out):
	#cfg cacheline
	info=key_word(cfg_file,"CACHELINE")
	if(info==-1):
		print("DEFAULT CACHELINE SIZE: 64Byte")
		out.write("#define CACHELINE "+str(64)+"\n")
		cacheline=64
	else:
		out.write("#define CACHELINE "+re.sub("\D","",info)+"\n")
		cacheline=int(re.sub("\D","",info))
	#cfg cmdwidth
	info=key_word(cfg_file,"CMD_WIDTH")
	if(info==-1):
		print("DEFAULT CMD SIZE: 64Byte")
		out.write("#define CMD_WIDTH "+str(64)+"\n")
	else:
		out.write("#define CMD_WIDTH "+re.sub("\D","",info)+"\n")
	#cfg cpu
	info=key_word(cfg_file,"CPU")
	if(info==-1):
		print("DEFAULT CPU NUM: 4")
		out.write("#define CPU "+str(4)+"\n")
	else:
		out.write("#define CPU "+re.sub("\D","",info)+"\n")
	#cfg memory size
	info=key_word(cfg_file,"MEM_SIZE")
	if(info==-1):
		print("DEFAULT MEM_SIZE: 4GB")
		out.write("#define MEM_SIZE "+"((u64)(0x1)<<32)"+"\n")
	else:
		mem_size=int(int(process_size(info)/cacheline)*cacheline)
		out.write("#define MEM_SIZE "+str(mem_size)+"\n")
	out.write("\n")
	#cache config size/assoc/level
	info=key_word(cfg_file,"L1SIZE")
	if(info==-1):
		l1_size=256
	else:
		l1_size=int(int(process_size(info)/cacheline)*cacheline)
	l1_all_assoc=int(l1_size/cacheline)
		
	info=key_word(cfg_file,"L2SIZE")
	if(info==-1):
		l2_size=1024
	else:
		l2_size=int(int(process_size(info)/cacheline)*cacheline)
	l2_all_assoc=int(l2_size/cacheline)
	
	info=key_word(cfg_file,"L1ASSOC")
	if(info==-1):
		l1_assoc=4
	elif(re.search("all",info,re.IGNORECASE)):
		l1_assoc=l1_all_assoc
	else:
		l1_assoc=int(process_size(info))
			
	info=key_word(cfg_file,"L2ASSOC")
	if(info==-1):
		l2_assoc=4
	elif(re.search("all",info,re.IGNORECASE)):
		l2_assoc=l2_all_assoc
	else:
		l2_assoc=int(process_size(info))
	
	#cfg consistency
	info=key_word(cfg_file,"CONSISTENCY")
	if(info==-1):
		print("DEFAULT CONSISTENCY: MESI")
		out.write("#define MESI "+"\n")
	else:
		#WRT_THROUGH or MESI
		if (re.search("MESI",info,re.IGNORECASE)):
			out.write("#define MESI "+"\n")
			out.write("#define LEVEL_TWO "+"\n")
			out.write("#define L2SIZE "+str(l2_size)+"\n")
			out.write("#define L2_ASSOC "+str(l2_assoc)+"\n")
			
		elif(re.search("WRT_THROUGH",info,re.IGNORECASE)):
			out.write("#define WRT_THROUGH "+"\n")
			#other config
			b_info=key_word(cfg_file,"EXTRA")
			if(b_info==-1):
				print("DEFAULT CONFIG is WRT_ASSIGN and WRT_UPDATE\n")
				out.write("#define WRT_ASSIGN "+"\n")
				out.write("#define WRT_UPDATE "+"\n")
			else:
				b_info=b_info.strip().split(',')
				if(len(b_info)!=2):
					print("ERROR CONFIG")
				else:
					out.write("#define "+b_info[0].strip('[]')+"\n")
					out.write("#define "+b_info[1].strip('[]')+"\n")
			#cache level
			c_info=key_word(cfg_file,"LEVEL")
			if(c_info==-1):
				print("DEFAULT LEVEL 1\n")
			elif(process_size(c_info)==2):
				out.write("#define LEVEL_TWO "+"\n")
				out.write("#define L2SIZE "+str(l2_size)+"\n")
				out.write("#define L2_ASSOC "+str(l2_assoc)+"\n")
			else:
				print("DEFAULT LEVEL 1\n")
		else:
			print("DEFAULT CONSISTENCY: MESI")
			out.write("#define MESI "+"\n")
			out.write("#define LEVEL_TWO "+"\n")
			out.write("#define L2SIZE "+str(l2_size)+"\n")
			out.write("#define L2_ASSOC "+str(l2_assoc)+"\n")		
		
		out.write("#define L1SIZE "+str(l1_size)+"\n")
		out.write("#define L1_ASSOC "+str(l1_assoc)+"\n")
	#replace policy
	out.write("\n")
	info=key_word(cfg_file,"REPLACE")
	if(info==-1):
		print("DEFAULT REPLACE POLICY: LRU\n")
		out.write("#define LRU "+"\n")
	elif(re.search("LRU",info,re.IGNORECASE)):
		out.write("#define LRU "+"\n")
	else:
		print("DEFAULT REPLACE POLICY: LRU\n")
		out.write("#define LRU "+"\n")
	#debug
	info=key_word(cfg_file,"PRINT")
	if(info==-1):
		print("DEFAULT PRINT INFO")
		out.write("#define INFO "+"\n")
	else:
		info=info.strip().split(',')
		for i in range(len(info)):
			info[i]=info[i].strip('[]').upper()
			out.write("#define "+info[i]+"\n")
