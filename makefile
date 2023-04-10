#
# This makefile will compile all sources listed for all.
# =======================================================


all :
	+$(MAKE) -C DP-1
	+$(MAKE) -C DP-2
	+$(MAKE) -C DC



clean:
	rm -rf */bin/*
	rm -rf */obj/*.o
	rm -rf */inc/*.h~
	rm -rf */src/*.c~
	rm -rf */lib/*.a
	rm -rf */bin/*.so


