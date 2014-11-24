
SRC=$(wildcard src/*.c)
OBJ=$(patsubst src/%.c,build/%.o,$(SRC))
TARGET=bin/drd

.PHONY: all clean upload

all: $(TARGET)
	strip $(TARGET)
	@size -A $(TARGET) | grep "Total"

$(TARGET): $(OBJ) | bin
	gcc $(LDFLAGS) $(OBJ) -o $@

$(OBJ): | build

bin:
	mkdir -p bin

build:
	mkdir -p build

build/%.o: src/%.c
	gcc $(CFLAGS) -c $< -o $@

upload: $(TARGET)
	#scp

clean:
	rm -rvf $(TARGET) build
