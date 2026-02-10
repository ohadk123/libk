SRC_DIR=./src
BUILD_DIR=./build

CC=gcc
CFLAGS=-Wall -Wextra -pedantic -Werror -g -I./include

SRCS := $(wildcard $(SRC_DIR)/*.c)
OBJS := $(patsubst $(SRC_DIR)/%.c,$(BUILD_DIR)/%.o,$(SRCS))

all:
	compiledb make compile

compile: build $(BUILD_DIR)/libk.so

build:
	mkdir -p $(BUILD_DIR)

$(BUILD_DIR)/libk.so: $(OBJS)
	$(CC) -shared -o $@ $^

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf $(BUILD_DIR)

install: compile
	sudo mkdir -p /usr/local/include/libk
	sudo cp $(BUILD_DIR)/libk.so /usr/local/lib/libk.so
	sudo cp ./include/* /usr/local/include/libk/

uninstall:
	sudo rm -f /usr/local/lib/libk.so
	sudo rm -rf /usr/local/include/libk
