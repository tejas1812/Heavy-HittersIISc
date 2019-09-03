#include "../modified_massdalsketches/lcdelta.h"
#include "../modified_massdalsketches/countmin.h"

#ifndef SKETCHLC_H
#define SKETCHLC_H
typedef struct SketchLC_type
{
  //	char *a;
  //	char *b;
	long l;
	long w;	
	
	// uses 2/phi counters
	LCD_type* T1;

	// uses 1/epsilon counters
	CM_type* T2;

	float phi;
	float epsilon;
	
}SketchLC_type;

SketchLC_type * SketchLC_Init(float phi, float epsilon, float delta);
void SketchLC_Insert(SketchLC_type* sfr, char * item);
void SketchLC_Report(SketchLC_type* sfr);
void SketchLC_Destroy(SketchLC_type *sfr);
int SketchLC_Size(SketchLC_type *dfr, int maxlength);

#endif
