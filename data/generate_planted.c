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
	generate_data(N);
	return 0;
}
