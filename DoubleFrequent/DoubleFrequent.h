#include "../modified_massdalsketches/frequent.h"
typedef struct DoubleFrequent_type
{
	char *a;
	char *b;
	long long modval;	
	
	// uses 2/phi counters
	freq_type* T1;

	// uses 1/epsilon counters
	freq_type* T2;

	float phi;
	float epsilon;
	
}DoubleFrequent_type;

DoubleFrequent_type * DoubleFrequent_Init(float phi, float epsilon, float delta);
void DoubleFrequent_Insert(DoubleFrequent_type* dfr, char * item);
void DoubleFrequent_Report(DoubleFrequent_type* dfr);
int DoubleFrequent_Size(DoubleFrequent_type *dfr);
