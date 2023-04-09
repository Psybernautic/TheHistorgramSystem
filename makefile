#
# This makefile will compile all sources listed for all.
# =======================================================


all :
	+$(MAKE) -C DP-1
	+$(MAKE) -C DP-2
	+$(MAKE) -C DC

	rm -rf ./program

	mkdir program
	mkdir program/bin

	cp DP-1/bin/DP-1 program/bin
	cp DP-2/bin/DP-2 program/bin
	cp DC/bin/DC program/bin/

clean:
	rm -rf */bin/*
	rm -rf */obj/*.o
	rm -rf */inc/*.h~
	rm -rf */src/*.c~
	rm -rf */lib/*.a
	rm -rf */bin/*.so
	rm -rf ./program

