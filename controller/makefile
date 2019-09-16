#controller makefile

export PATH := $(PATH):/home/nick/dev/x-tools6h/arm-unknown-linux-gnueabihf/bin
PREFIX=arm-unknown-linux-gnueabihf-
INCLUDE=/home/nick/dev/rpi/include
LIB=/home/nick/dev/rpi/lib
SRC=$(wildcard src/*.c)
OBJ=$(patsubst src/%.c,build/%.o,$(SRC))
TARGET=controller
LDFLAGS=-L $(LIB) -l bcm2835 -l ncursesw -l rt
DRD=drd#192.168.2.134

#ifeq (, $(shell which $(PREFIX)gcc))
# $(error "No $(PREFIX)gcc in $(PATH).")
#endif

.PHONY: all clean upload

all: $(TARGET)

$(TARGET): $(OBJ) | bin
	$(PREFIX)gcc -o $@ $(OBJ) $(LDFLAGS)
	$(PREFIX)strip $(TARGET)
	@$(PREFIX)size -A $(TARGET) | grep "Total"

$(OBJ): | build

bin:
	mkdir -p bin

build:
	mkdir -p build

build/%.o: src/%.c
	$(PREFIX)gcc $(CFLAGS) -I $(INCLUDE) -c $< -o $@

upload: $(TARGET)
	scp $(TARGET) alarm@$(DRD):~/$(TARGET)

clean:
	rm -rvf $(TARGET) build
