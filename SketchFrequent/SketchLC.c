#include "../modified_massdalsketches/lcdelta.h"
#include "SketchLC.h"
#include "../modified_massdalsketches/countmin.h"
#include "../Hashing/hashing.h"
#include "../massdalsketches/prng.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

int stream_size=0;
//char emptystring[sizeofdata];
int SketchLC_Size(SketchLC_type *dfr, int maxlength)
{
	//changed this
  return LCD_Size((dfr->T1), maxlength)+CM_Size(dfr->T2);
}
SketchLC_type * SketchLC_Init(float phi, float epsilon, float delta)
{
	//memset(emptystring,'0',sizeofdata);
	//emptystring[sizeofdata-1]='\0';
	//for generating random values. uses prng.c
  //	prng_type *prng= prng_Init(time(NULL),3);

	SketchLC_type * sfr= malloc(sizeof(SketchLC_type));

	//	sfr->a=allocate_array(sizeofdata);
	//	sfr->b=allocate_array(sizeofdata); 
	//	generate_rand_using_prng(sfr->a,prng);
	//	generate_rand_using_prng(sfr->b,prng);
	//	prng_Destroy(prng);
	//hash function : [n]->[4/(delta*epsilon^2)]
	sfr->l=(long)2.0*log10l(1/(phi*delta));
	sfr->w=(long)2.0/(epsilon);


	sfr->T1 = LCD_Init(phi/2.0);
	sfr->T2 = CM_Init(sfr->w,sfr->l,sizeofdata);

	sfr->phi=phi;
	sfr->epsilon=epsilon;

	
	return sfr;
}
void SketchLC_Insert(SketchLC_type* sfr, char * item)
{
  stream_size++;
	//printf("before\n");
	LCD_Update(sfr->T1, item);
	//printf("after\n");
	CM_Update(sfr->T2, item);
	//printf("after cm\n");
}
void SketchLC_Report(SketchLC_type* sfr)
{
  char** potentials = LCD_Report(sfr->T1, sfr->phi, sizeofdata);
  int m=0, count;
  printf("\n\n\n\t\t\t\tSketchLC Output\n\n");
	while(potentials[m]!=0){    
	count = LCD_PointEst(sfr->T1, potentials[m]);
    if(count>(sfr->phi)*stream_size/2 && CM_PointEst(sfr->T2, potentials[m])>(sfr->phi)*stream_size)
      
      printf("%s:%d\n", potentials[m], count);
		++m;
  }
  
}

void SketchLC_Destroy(SketchLC_type *sfr)
{
  //	free(sfr->a);
  //	free(sfr->b);
	LCD_Destroy(sfr->T1);
	CM_Destroy(sfr->T2);
	
	free(sfr);
}
