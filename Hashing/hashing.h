#include "../massdalsketches/prng.h"

#ifndef sizeofdata
	#define sizeofdata 128
#endif

#ifndef num_of_elements
	#define num_of_elements sizeofdata/sizeof(char)
#endif

char * allocate_array(int size);
void deallocate_array(char * a);
long long hashval(char *num, char *a,char *b, long long modval);
void generate_rand_using_prng(char* a,prng_type * prng);