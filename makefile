
PREFIX=arm-unknown-linux-gnueabi-
INCLUDE=/home/nick/x-tools/arm-unknown-linux-gnueabi/include
LIB=/home/nick/x-tools/arm-unknown-linux-gnueabi/lib
SRC=$(wildcard src/*.c)
OBJ=$(patsubst src/%.c,build/%.o,$(SRC))
TARGET=bin/drd
LDFLAGS=-I $(LIB)

.PHONY: all clean upload

all: $(TARGET)
	$(PREFIX)strip $(TARGET)
	@$(PREFIX)size -A $(TARGET) | grep "Total"

$(TARGET): $(OBJ) | bin
	$(PREFIX)gcc $(LDFLAGS) $(OBJ) -o $@

$(OBJ): | build

bin:
	mkdir -p bin

build:
	mkdir -p build

build/%.o: src/%.c
	$(PREFIX)gcc $(CFLAGS) -I $(INCLUDE) -c $< -o $@

upload: $(TARGET)
	#scp

clean:
	rm -rvf $(TARGET) build
