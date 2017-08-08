genzipf: generatezipfian.o hashing.o prng.o
	gcc -o genzipf generatezipfian.o hashing.o prng.o -lm
generatezipfian.o: generatezipfian.c hashing.o
	gcc -c -g generatezipfian.c -Wall
hashing.o: ../Hashing/hashing.c ../Hashing/hashing.h prng.o
	gcc -c -g ../Hashing/hashing.c -Wall
prng.o: ../massdalsketches/prng.c ../massdalsketches/prng.h
	gcc -c -g ../massdalsketches/prng.c -Wall

clean:
	rm *.o
