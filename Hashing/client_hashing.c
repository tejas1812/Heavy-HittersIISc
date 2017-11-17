#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include "hashing.h"
double diff(struct timespec* s,struct timespec* e)
{
  double temp;
  temp = (e->tv_sec - s->tv_sec)*1000;
  temp = temp + (e->tv_nsec - s->tv_nsec)*0.000001;
  return temp;
}
int main()
{
	srand(time(NULL));

	prng_type *prng= prng_Init(time(NULL),3);
	
	char *a = allocate_array(sizeofdata);
	char *b = allocate_array(sizeofdata);
	char *x = allocate_array(sizeofdata);
	
	generate_rand_using_prng(a,prng,sizeofdata);
	generate_rand_using_prng(b,prng,sizeofdata);
	generate_rand_using_prng(x,prng,sizeofdata);
	
	printf("a: %s\n",a);
	printf("b: %s\n",b);
	printf("x: %s\n",x);
	
	struct timespec s,e;	
    char *line =allocate_array(sizeofdata+1);
	int count;
	scanf("%d",&count);
	printf("count: %d\n", count);

	clock_gettime(CLOCK_REALTIME,&s);
	while(count>0)
	{
		scanf("%s",line);				
		printf("hash value: %llu\n", hashval(line,a,b,2917));
		free(line);
		    
	    line=allocate_array(sizeofdata+1);
		--count;
    }
	free(line);
	clock_gettime(CLOCK_REALTIME,&e);
	printf("%f milliseconds\n",diff(&s,&e));
	
	deallocate_array(a);
	deallocate_array(b);
	deallocate_array(x);

	return 0;
}
