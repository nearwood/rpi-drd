#web makefile

DRD=drd

.PHONY: upload all clean

all: upload

upload:
	rsync -rv build/* alarm@$(DRD):/srv/http/ --exclude makefile
