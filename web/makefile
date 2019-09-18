#web makefile

DRD=drd

.PHONY: upload all clean

all: upload

upload:
	rsync -r . root@$(DRD):/srv/http/ --exclude makefile
