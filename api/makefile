#Golang API server makefile

export PATH := $(PATH):/home/nick/dev/x-tools6h/arm-unknown-linux-gnueabihf/bin
PREFIX=arm-unknown-linux-gnueabihf-
TARGET=drdapi
DRD=drd#192.168.2.134

.PHONY: all clean upload

all: bin/$(TARGET)

bin/$(TARGET): $(OBJ) | bin
	env GOOS=linux GOARCH=arm GOARM=5 go build -o bin/$(TARGET)
	$(PREFIX)size -A $@ | grep "Total"
	$(PREFIX)strip $@
	$(PREFIX)size -A $@ | grep "Total"

bin:
	mkdir -p bin

upload: bin/$(TARGET)
	scp -p bin/$(TARGET) alarm@$(DRD):~/$(TARGET)

clean:
	rm -rvf bin

