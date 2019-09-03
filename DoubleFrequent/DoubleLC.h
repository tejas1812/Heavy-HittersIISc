#include "../modified_massdalsketches/lcdelta.h"
#ifndef DOUBLELC_H
#define DOUBLELC_H


typedef struct DoubleLC_type
{
  	char *a;
  	char *b;
	long l;
	long w;	
	long long modval;
	// uses 2/phi counters
	LCD_type* T1;

	// uses 1/epsilon counters
	LCD_type* T2;

	float phi;
	float epsilon;
	
}DoubleLC_type;

DoubleLC_type * DoubleLC_Init(float phi, float epsilon, float delta);
void DoubleLC_Insert(DoubleLC_type* lcfr, char * item);
void DoubleLC_Report(DoubleLC_type* lcfr);
void DoubleLC_Destroy(DoubleLC_type *lcfr);
int DoubleLC_Size(DoubleLC_type *lcfr, int maxlength);
#endif