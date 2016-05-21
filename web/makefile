#web makefile

#export PATH=$PATH:/home/nick/x-tools/arm-rpi-linux-gnueabi/bin
DRD=192.168.0.116

.PHONY: upload all clean

all: upload

#upload any changed files
upload:
	#scp web root@$(DRD):/srv/http/
	#touch web
	rsync -r . root@$(DRD):/srv/http/ --exclude makefile
