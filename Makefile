msrc = src_monitor
ssrc = src_simulador

build:
	+$(MAKE) -C $(msrc) build
	+$(MAKE) -C $(ssrc) build
	+$(MAKE) copy

clean:
	+$(MAKE) -C $(msrc) clean
	+$(MAKE) -C $(ssrc) clean

copy:
	cp $(msrc)/monitor .
	cp $(ssrc)/simulador .
	cp $(ssrc)/config.json .
