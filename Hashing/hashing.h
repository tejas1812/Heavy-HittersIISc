#include "../massdalsketches/prng.h"

#ifndef sizeofdata
	#define sizeofdata 100
#endif

#ifndef num_of_elements
	#define num_of_elements sizeofdata/sizeof(char)
#endif

char * allocate_array(int size);
void deallocate_array(char * a);
long long hashval(char *num, char *a,char *b, long long modval);
void generate_rand_using_prng(char* a,prng_type * prng, int len);
char * get_string(long long val);
long long mod_arrays2(char *a, long long b);
char * mul_arrays(char*, char*, int ,int );
void zipf(double alpha,unsigned int m);
void generate_data(unsigned int N);
