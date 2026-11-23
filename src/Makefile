
CC        ?= gcc
MAX_TASKS ?= 20
CFLAGS    := -std=c11 -D_POSIX_C_SOURCE=200809L -O2 -g -pthread \
             -Wall -Wextra -Wpedantic -DMAX_TASKS=$(MAX_TASKS)
SRC_DIR   := ./
BUILD_DIR := build
LIB_SRC   := $(SRC_DIR)/task.c $(SRC_DIR)/fila.c $(SRC_DIR)/produtor.c $(SRC_DIR)/consumidor.c
APP_SRC   := $(LIB_SRC) $(SRC_DIR)/main.c
HEADERS   := $(wildcard $(SRC_DIR)/*.h)

.PHONY: all asan tsan clean

all: escalonador

escalonador: $(APP_SRC) $(HEADERS)
	$(CC) $(CFLAGS) $(APP_SRC) -o $@

# versao para achar vazamentos e acessos invalidos
asan: $(APP_SRC) $(HEADERS)
	$(CC) $(CFLAGS) -fsanitize=address,undefined -fno-omit-frame-pointer $(APP_SRC) -o escalonador

# versao para achar corridas de dados
tsan: $(APP_SRC) $(HEADERS)
	$(CC) $(CFLAGS) -fsanitize=thread $(APP_SRC) -o escalonador

clean:
	rm -rf escalonador $(BUILD_DIR)
