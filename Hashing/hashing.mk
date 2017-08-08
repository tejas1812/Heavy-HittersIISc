client_hashing: client_hashing.o hashing.o prng.o
	gcc -o client_hashing client_hashing.o hashing.o prng.o -Wall -lm 
client_hashing.o: client_hashing.c
	gcc -c client_hashing.c 
hashing.o: hashing.c hashing.h
	gcc -c hashing.c 
prng.o: ../massdalsketches/prng.c ../massdalsketches/prng.h
	gcc -c ../massdalsketches/prng.c 
