#include "../modified_massdalsketches/frequent.h"
#include "../modified_massdalsketches/countmin.h"
typedef struct SketchFrequent_type
{
	char *a;
	char *b;
	long l;
	long w;	
	
	// uses 2/phi counters
	freq_type* T1;

	// uses 1/epsilon counters
	CM_type* T2;

	float phi;
	float epsilon;
	
}SketchFrequent_type;

SketchFrequent_type * SketchFrequent_Init(float phi, float epsilon, float delta);
void SketchFrequent_Insert(SketchFrequent_type* sfr, char * item);
void SketchFrequent_Report(SketchFrequent_type* sfr);
void SketchFrequent_Destroy(SketchFrequent_type *sfr);
