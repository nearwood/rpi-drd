#main makefile

#export PATH=$PATH:/home/nick/x-tools/arm-rpi-linux-gnueabi/bin
WEB=$(wildcard web/*)
DRD=192.168.0.116

SUBDIRS = controller server

#ifeq (, $(shell which $(PREFIX)gcc))
# $(error "No $(PREFIX)gcc in $(PATH).")
#endif

.PHONY: subdirs $(SUBDIRS) all web $(WEB) clean

subdirs: $(SUBDIRS)

$(SUBDIRS):
	$(MAKE) -C $@

all: subdirs

#upload any changed files under web/
web: $(WEB)

$(WEB):
	scp -r $@ root@$(DRD):/srv/http/

clean:
	rm -rvf build
