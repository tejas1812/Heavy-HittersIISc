#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

int main()
{
	srand(time(NULL));

	prng_type *prng= prng_Init(time(NULL),3);
	
	char *a = allocate_array(sizeofdata);
	char *b = allocate_array(sizeofdata);
	char *x = allocate_array(sizeofdata);
	
	generate_rand_using_prng(a,prng);
	generate_rand_using_prng(b,prng);
	generate_rand_using_prng(x,prng);
	
	//char *x=add_arrays(a,b,sizeofdata,sizeofdata);
	
	printf("a: %s\n",a);
	printf("b: %s\n",b);
	printf("x: %s\n",x);
	printf("hashval of %s: %lld\n",x,hashval(x,a,b,2917));
	
	deallocate_array(a);
	deallocate_array(b);
	deallocate_array(x);

	return 0;
}