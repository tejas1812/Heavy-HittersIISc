client_doublefrequent: client_double_freq.o DoubleFrequent.o frequent.o hashing.o prng.o
	gcc -o client_doublefrequent client_double_freq.o DoubleFrequent.o frequent.o hashing.o prng.o -lm
client_double_freq.o: client_double_freq.c DoubleFrequent.o
	gcc -c -g client_double_freq.c
DoubleFrequent.o: DoubleFrequent.c frequent.o
	gcc -c -g DoubleFrequent.c
frequent.o: ../modified_massdalsketches/frequent.c hashing.o
	gcc -c -g ../modified_massdalsketches/frequent.c -Wall
hashing.o: ../Hashing/hashing.c prng.o
	gcc -c -g ../Hashing/hashing.c -Wall
prng.o: ../massdalsketches/prng.c
	gcc -c -g ../massdalsketches/prng.c -Wall 

clean:
	rm *.o
