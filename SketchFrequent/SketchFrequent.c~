#include "../modified_massdalsketches/frequent.h"
#include "SketchFrequent.h"
#include "../modified_massdalsketches/countmin.h"
#include "../Hashing/hashing.h"
#include "../massdalsketches/prng.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
SketchFrequent_type * SketchFrequent_Init(float phi, float epsilon, float delta)
{
	//for generating random values. uses prng.c
	prng_type *prng= prng_Init(time(NULL),3);

	SketchFrequent_type * sfr= malloc(sizeof(SketchFrequent_type));

	sfr->a=allocate_array(sizeofdata);
	sfr->b=allocate_array(sizeofdata); 
	generate_rand_using_prng(sfr->a,prng);
	generate_rand_using_prng(sfr->b,prng);
	prng_Destroy(prng);
	//hash function : [n]->[4/(delta*epsilon^2)]
	sfr->l=(long)2.0*log10l(1/(phi*delta));
	sfr->w=(long)2.0/(epsilon);


	sfr->T1 = Freq_Init(phi/2.0, sfr->a, sfr->b);
	sfr->T2 = CM_Init(sfr->w,sfr->l);

	sfr->phi=phi;
	sfr->epsilon=epsilon;

	
	return sfr;
}
void SketchFrequent_Insert(SketchFrequent_type* sfr, char * item)
{
	//printf("before\n");
	Freq_Update(sfr->T1, item, sfr->w);
	//printf("after\n");
	CM_Update(sfr->T2, item);
	//printf("after cm\n");
}
void SketchFrequent_Report(SketchFrequent_type* sfr)
{
	GROUP *g;
	ITEMLIST *i,*first;
	int count=0;
	g=sfr->T1->groups;
	while (g!=NULL) 
	{
		count=count+g->diff;
		first=g->items;
		i=first;
		if (i!=NULL)
			do 
			{
				//x: i->item
				//if T1(x)>(phi*m/2)

				long c =CM_PointEst(sfr->T2, i->item);
				//printf("item: %s count: %d\n",i->item,count);
				if(c>=(sfr->phi *stream_size))
				{
					//if T2[h(x)]>=(phi-epsilon)m
					/*int count_in_T2= freq_get_count(dfr->T2,get_string(hashval(i->item, dfr->a, dfr->b, dfr->modval)));
					if(count_in_T2>=(dfr->phi-dfr->epsilon)*stream_size)
					{
						//printf("item: %s count: %d\n",i->item,count_in_T2);
					}*/
					printf("item: %s count: %li\n",i->item,c);								
				}
				i=i->nexting;
			}
			while (i!=first);
		g=g->nextg;
    }
}

void SketchFrequent_Destroy(SketchFrequent_type *sfr)
{
	free(sfr->a);
	free(sfr->b);
	Freq_Destroy(sfr->T1);
	CM_Destroy(sfr->T2);
	
	free(sfr);
}
