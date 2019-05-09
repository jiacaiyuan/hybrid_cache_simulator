import os
import sys
import re
from optparse import OptionParser
print("\n\n\n\n")
print(sys.version)
os.system("python -V")
usage="%prog [options]"
version="1.0"
parser=OptionParser(usage,description="Get INFO about CMD/MEMORY",version="%prog "+version)
parser.add_option("-f","--format",dest="format",help="cmd/memory format",type='string',default="memory")
parser.add_option("-i","--input",dest="inp_file",help="input the file of memory of command",type='string',default="inp.txt")
parser.add_option("-o","--output",dest="out_file",help="output the file of memory of command",type='string',default="out.txt")
(options,args)=parser.parse_args()
inp=open(options.inp_file,'r')
out=open(options.out_file,'w')
owner=[]
if options.format=="memory":
	format='0 W 0x\w{0,9} +'
elif options.format=="cmd":
	format='\d{1,20} (W|R) 0x\w{0,9} +'
else:
	print("the Format is only the memory or cmd")
	exit()
lines=inp.readlines()
for line in lines:
	data=''
	if re.match(format,line):
		line=(line.strip()).split(" ")
		if len(line)!=5:
			print("Error Memory/Command info")
			exit()
		if options.format=="cmd" and line[0]=='0' and line[1]=='W':
			print("Ignore memory")
			continue
		cycle=line[0]
		operation=line[1]
		addr=line[2][2:] #don't using 0x
		lenth=str(int(len(line[3])/2))

		if line[4] not in owner:
			owner.append(line[4])
		own=str(owner.index(line[4]))
		for i in range(0,len(line[3]),2):
			data=data+line[3][i:i+2]+' '
		if options.format=="memory":
			out.write(addr+' '+str(int(len(line[3])/2))+" "+data.strip()+'\n')
		elif options.format=="cmd":
			#out.write(cycle+" "+operation+" "+addr+" "+lenth+" "+data.strip()+" "+own+"\n")
			out.write(cycle+" "+operation+" "+addr+" "+lenth+" "+data.strip()+" "+'0'+"\n")
print(owner)
