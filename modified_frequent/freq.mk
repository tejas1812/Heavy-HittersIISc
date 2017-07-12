client_freq: client_freq.c frequent.c
	gcc -o client_freq client_freq.c frequent.c ../massdalsketches/prng.c ../massdalsketches/massdal.c -lm -Wall -std=c99
