client_sketchfrequent: client_sketchfreq.o SketchFrequent.o
	gcc -o client_sketchfrequent client_sketchfreq.o DoubleFrequent.o DoubleLC.o SketchFrequent.o SketchLC.o countmin.o frequent.o lcdelta.o massdal.o hashing.o prng.o -lm
client_sketchfreq.o: client_sketchfreq.c SketchLC.c DoubleFrequent.o DoubleLC.o SketchFrequent.o SketchLC.o
	gcc -c -g client_sketchfreq.c -Wall
DoubleFrequent.o: ../DoubleFrequent/DoubleFrequent.c ../DoubleFrequent/DoubleFrequent.h	frequent.o
	gcc -c -g ../DoubleFrequent/DoubleFrequent.c -Wall
DoubleLC.o: ../DoubleFrequent/DoubleLC.c lcdelta.o ../DoubleFrequent/DoubleLC.h
	gcc -c -g ../DoubleFrequent/DoubleLC.c -Wall
SketchLC.o: SketchLC.c lcdelta.o SketchLC.h countmin.o
	gcc -c -g SketchLC.c -Wall
SketchFrequent.o: SketchFrequent.c frequent.o SketchFrequent.h countmin.o
	gcc -c -g SketchFrequent.c -Wall
lcdelta.o: ../modified_massdalsketches/lcdelta.c ../modified_massdalsketches/lcdelta.h
	gcc -c -g ../modified_massdalsketches/lcdelta.c -Wall
countmin.o: ../modified_massdalsketches/countmin.c ../modified_massdalsketches/countmin.h hashing.o massdal.o
	gcc -c -g ../modified_massdalsketches/countmin.c -Wall
frequent.o: ../modified_massdalsketches/frequent.c hashing.o ../modified_massdalsketches/frequent.h
	gcc -c -g ../modified_massdalsketches/frequent.c -Wall
massdal.o: ../modified_massdalsketches/massdal.h ../modified_massdalsketches/massdal.c
	gcc -c -g ../modified_massdalsketches/massdal.c -Wall
hashing.o: ../Hashing/hashing.c prng.o ../Hashing/hashing.h
	gcc -c -g ../Hashing/hashing.c -Wall
prng.o: ../massdalsketches/prng.c ../massdalsketches/prng.h
	gcc -c -g ../massdalsketches/prng.c -Wall 

clean:
	rm *.o
