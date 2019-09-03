#include "../Hashing/hashing.h"
#include <time.h>
#include <stdlib.h>
#include <stdio.h>
int main()
{
	unsigned int N;
	srand(time(NULL));
	scanf("%u",&N);
	printf("%u\n",N);
	zipf(2.0,N);
	return 0;
}
