#include "../modified_massdalsketches/frequent.h"
#include "DoubleFrequent.h"
#include "../Hashing/hashing.h"
#include "../massdalsketches/prng.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
int DoubleFrequent_Size(DoubleFrequent_type *dfr)
{
	return Freq_Size(dfr->T1)+Freq_Size(dfr->T2);
}
DoubleFrequent_type * DoubleFrequent_Init(float phi, float epsilon, float delta)
{
	//for generating random values. uses prng.c
	prng_type *prng= prng_Init(time(NULL),3);

	DoubleFrequent_type * dfr= malloc(sizeof(DoubleFrequent_type));

	dfr->a=allocate_array(sizeofdata);
	dfr->b=allocate_array(sizeofdata); 
	generate_rand_using_prng(dfr->a,prng);
	generate_rand_using_prng(dfr->b,prng);
	//change
	prng_Destroy(prng);
	//hash function : [n]->[4/(delta*epsilon^2)]
	dfr->modval=(long long)4.0/(delta*epsilon*epsilon);

	dfr->T1 = Freq_Init(phi/2.0, dfr->a, dfr->b);
	dfr->T2 = Freq_Init(epsilon, dfr->a, dfr->b);

	dfr->phi=phi;
	dfr->epsilon=epsilon;

	return dfr;
}

void DoubleFrequent_Insert(DoubleFrequent_type* dfr, char * item)
{
	Freq_Update(dfr->T1, item, dfr->modval);
	//change:
	char * hasheditem=get_string(hashval(item, dfr->a, dfr->b, dfr->modval));
	Freq_Update(dfr->T2,hasheditem ,dfr->modval);
	--stream_size;
	free(hasheditem);
}

int freq_get_count(freq_type *fr,char *item)
{
	GROUP *g;
	ITEMLIST *i,*first;
	int count=0;
  
	int point=1;
	g=fr->groups->nextg;
	while (g!=NULL) 
	{
		count=count+g->diff;
		first=g->items;
		i=first;
		if (i!=NULL)
			do 
			{
				if(strcmp(item,i->item)==0)
					return count;
				i=i->nexting;
			}
			while (i!=first);
		g=g->nextg;
    }
	return -1;
}

void DoubleFrequent_Report(DoubleFrequent_type* dfr)
{
	GROUP *g;
	ITEMLIST *i,*first;
	int count=0;
	g=dfr->T1->groups;
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
				//printf("item: %s count: %d\n",i->item,count);
				if(count>=(dfr->phi *stream_size)/2)
				{
					//if T2[h(x)]>=(phi-epsilon)m
				
						//change
					char * hasheditem=get_string(hashval(i->item, dfr->a, dfr->b, dfr->modval));
					int count_in_T2= freq_get_count(dfr->T2,hasheditem);
					free(hasheditem);

					if(count_in_T2>=(dfr->phi-dfr->epsilon)*stream_size)
					{
						printf("%s:%d\n",i->item,count_in_T2);
					}								
				}
				i=i->nexting;
			}
			while (i!=first);
		g=g->nextg;
    }
}
void DoubleFrequent_Destroy(DoubleFrequent_type* dfr)
{
	Freq_Destroy(dfr->T1);
	Freq_Destroy(dfr->T2);
	free(dfr->a);
	free(dfr->b);
	free(dfr);
}
