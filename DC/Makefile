./bin/DC: ./obj/DC.o ./../common/obj/shared_mem.o
	cc ./obj/DC.o ./../common/obj/shared_mem.o -o ./bin/DC -pthread

./obj/DC.o: ./src/DC.c ./../common/inc/shared_mem.h
	cc -c ./src/DC.c -o ./obj/DC.o -pthread

./../common/obj/shared_mem.o: ./../common/src/shared_mem.c ./../common/inc/shared_mem.h
	cc -c ./../common/src/shared_mem.c -o ./../common/obj/shared_mem.o -pthread

all: ./bin/DC

clean:
	rm -f ./bin.DC
	rm -f ./../common/obj/*.o
	rm -f ./obj/*.o
