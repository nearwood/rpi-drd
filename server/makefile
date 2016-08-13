#server makefile
#export PATH=$PATH:/home/nick/x-tools/arm-rpi-linux-gnueabi/bin
#./configure arm-linux --host=arm-rpi-linux-gnueabi --prefix=/home/nick/dev/rpi
#ncurses <6:
#./configure arm-linux --host=arm-rpi-linux-gnueabi --prefix=/home/nick/dev/rpi --disable-big-core --enable-termcap --with-shared --without-manpages

#ncursues 6:
#./configure arm-linux --host=arm-rpi-linux-gnueabi --prefix=/home/nick/dev/rpi --enable-widec --disable-big-core --enable-termcap --with-shared --without-manpages
PREFIX=arm-rpi-linux-gnueabi-
INCLUDE=/home/nick/dev/rpi/include
LIB=/home/nick/dev/rpi/lib
SRC=$(wildcard src/*.c)
OBJ=$(patsubst src/%.c,build/%.o,$(SRC))
TARGET=server
LDFLAGS=-L $(LIB) -l fcgi
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
	scp $(TARGET) alarm@$(DRD):~/$(TARGET)

clean:
	rm -rvf $(TARGET) build
