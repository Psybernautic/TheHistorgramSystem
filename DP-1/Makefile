./bin/DP-1: ./obj/DP-1.o ./../common/obj/shared_mem.o
	cc ./obj/DP-1.o ./../common/obj/shared_mem.o -o ./bin/DP-1 -pthread

./obj/DP-1.o: ./src/DP-1.c ./../common/inc/shared_mem.h
	cc -c ./src/DP-1.c -o ./obj/DP-1.o -pthread

./../common/obj/shared_mem.o: ./../common/src/shared_mem.c ./../common/inc/shared_mem.h
	cc -c ./../common/src/shared_mem.c -o ./../common/obj/shared_mem.o -pthread

all: ./bin/DP-1

clean:
	rm -f ./bin.DP-1
	rm -f ./../common/obj/*.o
	rm -f ./obj/*.o
