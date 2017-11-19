#include "../modified_massdalsketches/lcdelta.h"
#include "../modified_massdalsketches/frequent.h"

typedef struct FreqLC_type
{
  	char *a;
  	char *b;
	long l;
	long w;	
	long long modval;
	// uses 2/phi counters
	LCD_type* T1;

	// uses 1/epsilon counters
	freq_type* T2;

	float phi;
	float epsilon;
	
}FreqLC_type;

FreqLC_type * FreqLC_Init(float phi, float epsilon, float delta);
void FreqLC_Insert(FreqLC_type* lcfr, char * item);
void FreqLC_Report(FreqLC_type* lcfr);
void FreqLC_Destroy(FreqLC_type *lcfr);
int FreqLC_Size(FreqLC_type *lcfr, int maxlength);
