#!/usr/bin/make -f
## boostrap.mk (for myenv)
## Copyright 2017 Mac Radigan
## All Rights Reserved

.PHONY: bootstrap update packages-apt install
.DEFAULT_GOAL := bootstrap

bootstrap:
	autoreconf --force --install

install: bootstrap
	./configure --prefix=/opt/local && make && sudo make install

update:
	$(MAKE) -C ./submodules update

packages-apt:
	sudo apt-get install -y autoconf
	sudo apt-get install -y automake
	sudo apt-get install -y libtool
	sudo apt-get install -y build-essential

## *EOF*
