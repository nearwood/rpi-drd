#main makefile

#export PATH=$PATH:/home/nick/x-tools/arm-rpi-linux-gnueabi/bin
DRD=192.168.0.116

SUBDIRS = controller server

#ifeq (, $(shell which $(PREFIX)gcc))
# $(error "No $(PREFIX)gcc in $(PATH).")
#endif

.PHONY: subdirs $(SUBDIRS) all clean

subdirs: $(SUBDIRS)

$(SUBDIRS):
	$(MAKE) -C $@

all: subdirs

clean:
	rm -rvf build
