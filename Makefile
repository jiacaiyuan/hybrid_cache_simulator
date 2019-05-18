#***** ***** ***** ***** ***** ***** ***** ***** ***** ***** *****
#Author:   jcyuan
#E-Mail:   yuan861025184@163.com
#Project:  hybrid cache simulator
#Function: Makefile
#***** ***** ***** ***** ***** ***** ***** ***** ***** ***** *****
CC=g++
PARA= -Wall -fexceptions -g -std=c++11 
PY=python
SRC_DIR=./src
OBJ_DIR=./obj
BIN_DIR=./bin
CFG_DIR=./config
STATS_DIR=./stats
UTIL_DIR=./utils
TARGET=hc_simu

TRACE_FILE=./demo/test_1_nvmain.txt
MEM_FILE=./demo/memory_all.txt
CMD_FILE= ./demo/cmd_all.txt
#MEM_FILE=./demo/memory.txt
#CMD_FILE= ./demo/cmd.txt
STAT_FILE=$(STATS_DIR)/stats.txt
CFG_OUT=$(STATS_DIR)/system.cfg
CFG_FILE=$(CFG_DIR)/sys_cfg.cfg


.PHONY:default
default: get_trace config compile demo


#-----------------------------C++ Compile----------------------------------------------------
.PHONY:BASE 
BASE:
	$(CC) $(PARA) -c $(SRC_DIR)/cache/cache.cpp -o $(OBJ_DIR)/cache.o
	$(CC) $(PARA) -c $(SRC_DIR)/cmd/command.cpp -o $(OBJ_DIR)/command.o
	$(CC) $(PARA) -c $(SRC_DIR)/mem/memory.cpp -o $(OBJ_DIR)/memroy.o
	$(CC) $(PARA) -c $(SRC_DIR)/stats/stats.cpp -o $(OBJ_DIR)/stats.o
	$(CC) $(PARA) -c $(SRC_DIR)/main.cpp  -o $(OBJ_DIR)/main.o

.PHONY:LRU
LRU:
	$(CC) $(PARA) -c $(SRC_DIR)/policy/replace/lru.cpp -o $(OBJ_DIR)/replace.o

.PHONY:WRT_THROUGH
WRT_THROUGH:
	$(CC) $(PARA) -c $(SRC_DIR)/policy/consistency/wrt_through.cpp -o $(OBJ_DIR)/consistency.o

.PHONY:MESI
MESI:
	$(CC) $(PARA) -c $(SRC_DIR)/policy/consistency/mesi.cpp -o $(OBJ_DIR)/mesi.o

.PHONY:compile
compile:BASE LRU WRT_THROUGH MESI
	$(CC) -o $(BIN_DIR)/$(TARGET) $(OBJ_DIR)/*.o
#------------------------------------------------------------------------------------------


#-----------------------------------------Python Config-----------------------------------
.PHONY:help
help:
	$(PY) $(SRC_DIR)/config/config.py -h

.PHONY:config
config:
	chmod 775 $(SRC_DIR)/config/*
	$(PY) $(SRC_DIR)/config/config.py -F -f $(CFG_FILE)	
	mv type.h $(SRC_DIR)/include/
.PHONY:get_trace
get_trace:
	$(PY) $(UTIL_DIR)/get_info.py -i $(TRACE_FILE) -f cmd -o $(CMD_FILE)
	$(PY) $(UTIL_DIR)/get_info.py -i $(TRACE_FILE) -f memory -o $(MEM_FILE)
#-----------------------------------------------------------------------------------------


	
.PHONY:demo
demo:
	$(BIN_DIR)/$(TARGET) $(MEM_FILE) $(CMD_FILE) $(STAT_FILE) $(CFG_OUT)

clean:
	-rm $(OBJ_DIR)/* $(BIN_DIR)/* $(STATS_DIR)/*

