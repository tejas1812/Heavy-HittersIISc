client_doublelc: client_doubleLC.o 
	gcc -o client_doubleLC client_doubleLC.o DoubleLC.o countmin.o lcdelta.o massdal.o hashing.o prng.o -lm
client_doubleLC.o: client_doubleLC.c DoubleLC.c DoubleLC.o
	gcc -c -g client_doubleLC.c -Wall
DoubleLC.o: DoubleLC.c lcdelta.o DoubleLC.h countmin.o
	gcc -c -g DoubleLC.c -Wall
lcdelta.o: ../modified_massdalsketches/lcdelta.c ../modified_massdalsketches/lcdelta.h
	gcc -c -g ../modified_massdalsketches/lcdelta.c -Wall
countmin.o: ../modified_massdalsketches/countmin.c ../modified_massdalsketches/countmin.h hashing.o massdal.o
	gcc -c -g ../modified_massdalsketches/countmin.c -Wall
massdal.o: ../modified_massdalsketches/massdal.h ../modified_massdalsketches/massdal.c
	gcc -c -g ../modified_massdalsketches/massdal.c -Wall
hashing.o: ../Hashing/hashing.c prng.o ../Hashing/hashing.h
	gcc -c -g ../Hashing/hashing.c -Wall
prng.o: ../massdalsketches/prng.c ../massdalsketches/prng.h
	gcc -c -g ../massdalsketches/prng.c -Wall 

clean:
	rm *.o
