CC=g++
PARA= -Wall -fexceptions -g -std=c++11 
SRC_DIR=./src
OBJ_DIR=./obj
BIN_DIR=./bin
TARGET=hc_simu

#MEM_FILE=./demo/memory_all.txt
#CMD_FILE= ./demo/cmd_all.txt
MEM_FILE=./demo/memory.txt
CMD_FILE= ./demo/cmd.txt
STAT_FILE=./stats/stats.txt

.PHONY:default
default: compile demo



   .PHONY:BASERA) 
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

.PHONY:compile
compile:BASE LRU WRT_THROUGH
	$(CC) -o $(BIN_DIR)/$(TARGET) $(OBJ_DIR)/*.o
		
.PHONY:demo
demo:
	$(BIN_DIR)/$(TARGET) $(MEM_FILE) $(CMD_FILE) $(STAT_FILE)

clean:
	-rm $(OBJ_DIR)/* $(BIN_DIR)/*
#NOTE: don't update for makefile ,you can just runing the consistency of wrt_through and change the define in type.h to use the Makefile

