#web makefile

DRD=drd
SRC=src

.PHONY: all prod upload clean

all: yarn.lock

#TODO Fix so it runs as needed.
prod: build/
	yarn build

node_modules:
	mkdir -p $@

yarn.lock: node_modules package.json
	$(MAKE clean)
	yarn install

upload: prod
	rsync -rv build/* alarm@$(DRD):/srv/http/ --exclude makefile

clean:
	rm -rf node_modules build
