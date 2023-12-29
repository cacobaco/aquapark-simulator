msrc = monitor/
ssrc = simulador/

build:
	+$(MAKE) -C $(msrc) build
	+$(MAKE) -C $(ssrc) build
	+$(MAKE) copy

clean:
	+$(MAKE) -C $(msrc) clean
	+$(MAKE) -C $(ssrc) clean

copy: build
	cp $(msrc)/monitor .
	cp $(ssrc)/simulador .
	cp $(ssrc)/config.json .
