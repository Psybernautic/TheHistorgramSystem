CC = gcc
CFLAGS = -Wall -Wextra -g -pthread
LDFLAGS = -lrt -pthread

BIN_DIR = bin
OBJ_DIR = obj
INC_DIR = inc
SRC_DIR = src

SRCS = $(wildcard $(SRC_DIR)/*.c)
OBJS = $(patsubst $(SRC_DIR)/%.c,$(OBJ_DIR)/%.o,$(SRCS))

DP1_BIN = $(BIN_DIR)/dp1
DP2_BIN = $(BIN_DIR)/dp2
DC_BIN = $(BIN_DIR)/dc

all: $(DP1_BIN) $(DP2_BIN) $(DC_BIN)

$(DP1_BIN): $(OBJ_DIR)/dp1.o $(OBJ_DIR)/shared_mem.o
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

$(DP2_BIN): $(OBJ_DIR)/dp2.o $(OBJ_DIR)/shared_mem.o
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)


$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c | $(OBJ_DIR)
	$(CC) $(CFLAGS) -c -o $@ $< -I$(INC_DIR)

$(OBJ_DIR):
	mkdir $(OBJ_DIR)

clean:
	rm -rf $(BIN_DIR)/* $(OBJ_DIR)/*

.PHONY: all clean

