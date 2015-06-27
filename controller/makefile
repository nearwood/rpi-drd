#controller makefile
#export PATH=$PATH:/home/nick/x-tools/arm-rpi-linux-gnueabi/bin
PREFIX=arm-rpi-linux-gnueabi-
INCLUDE=/home/nick/dev/rpi/include
LIB=/home/nick/dev/rpi/lib
SRC=$(wildcard src/*.c)
OBJ=$(patsubst src/%.c,build/%.o,$(SRC))
TARGET=controller
LDFLAGS=-L $(LIB) -l bcm2835 -l ncurses -l rt
DRD=192.168.0.116

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
	scp $(TARGET) root@$(DRD):~/$(TARGET)

clean:
	rm -rvf $(TARGET) build
