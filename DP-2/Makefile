./bin/DP-2: ./obj/DP-2.o ./../common/obj/shared_mem.o
	cc ./obj/DP-2.o ./../common/obj/shared_mem.o -o ./bin/DP-2

./obj/DP-2.o: ./src/DP-2.c ./../common/inc/shared_mem.h
	cc -c ./src/DP-2.c -o ./obj/DP-2.o

./../common/obj/shared_mem.o: ./../common/src/shared_mem.c ./../common/inc/shared_mem.h
	cc -c ./../common/src/shared_mem.c -o ./../common/obj/shared_mem.o

all: ./bin/DP-2

clean:
	rm -f ./bin.DP-2
	rm -f ./../common/obj/*.o
	rm -f ./obj/*.o