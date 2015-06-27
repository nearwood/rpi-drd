#main makefile

#export PATH=$PATH:/home/nick/x-tools/arm-rpi-linux-gnueabi/bin
PREFIX=arm-rpi-linux-gnueabi-
INCLUDE=/home/nick/dev/rpi/include
LIB=/home/nick/dev/rpi/lib
WEB=$(wildcard web/*)
TARGET=bin/uh
LDFLAGS=-L $(LIB) -l bcm2835 -l ncurses -l rt
DRD=192.168.0.116

SUBDIRS = controller server

#ifeq (, $(shell which $(PREFIX)gcc))
# $(error "No $(PREFIX)gcc in $(PATH).")
#endif

.PHONY: subdirs $(SUBDIRS) all web clean

subdirs: $(SUBDIRS)

$(SUBDIRS):
	$(MAKE) -C $@

all: subdirs

#upload any changed files under web/
web: $(WEB)
	scp $< root@$(DRD):/srv/http/

clean:
	rm -rvf build
