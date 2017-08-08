genplanted: generate_planted.o hashing.o prng.o
	gcc -o genplanted generate_planted.o hashing.o prng.o -lm
generate_planted.o: generate_planted.c hashing.o
	gcc -c -g generate_planted.c -Wall
hashing.o: ../Hashing/hashing.c ../Hashing/hashing.h prng.o
	gcc -c -g ../Hashing/hashing.c -Wall
prng.o: ../massdalsketches/prng.c ../massdalsketches/prng.h
	gcc -c -g ../massdalsketches/prng.c -Wall

clean:
	rm *.o
