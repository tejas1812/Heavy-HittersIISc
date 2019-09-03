#include "DoubleLC.h"
#include "../modified_massdalsketches/lcdelta.h"
#include "../Hashing/hashing.h"
#include "../massdalsketches/prng.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

int stream_sze=0;

int DoubleLC_Size(DoubleLC_type *lcfr, int maxlength)
{
  return LCD_Size((lcfr->T1), maxlength)+LCD_Size(lcfr->T2,maxlength);
}
DoubleLC_type * DoubleLC_Init(float phi, float epsilon, float delta)
{
	//for generating random values. uses prng.c
  	prng_type *prng= prng_Init(time(NULL),3);

	DoubleLC_type * lcfr= malloc(sizeof(DoubleLC_type));
	lcfr->a=allocate_array(sizeofdata);
	lcfr->b=allocate_array(sizeofdata); 
	generate_rand_using_prng(lcfr->a,prng,sizeofdata);
	generate_rand_using_prng(lcfr->b,prng,sizeofdata);
	prng_Destroy(prng);
	lcfr->modval=(long long)4.0/(delta*epsilon*epsilon);
	//	sfr->a=allocate_array(sizeofdata);
	//	sfr->b=allocate_array(sizeofdata); 
	//	generate_rand_using_prng(sfr->a,prng);
	//	generate_rand_using_prng(sfr->b,prng);
	//	prng_Destroy(prng);
	//hash function : [n]->[4/(delta*epsilon^2)]
	lcfr->l=(long)2.0*log10l(1/(phi*delta));
	lcfr->w=(long)2.0/(epsilon);


	lcfr->T1 = LCD_Init(phi/2.0);
	lcfr->T2 = LCD_Init(epsilon);

	lcfr->phi=phi;
	lcfr->epsilon=epsilon;

	
	return lcfr;
}
/*
int LCD_get_count(LCD_type *fr,char *item)
{
	GROUP *g;
	ITEMLIST *i,*first;
	int count=0;
  
	int point=1;
	g=fr->groups->nextg;
	//printf("444\n");
	while (g!=NULL) 
	{
		count=count+g->diff;
		//printf("count - %d\n", count);
		first=g->items;
		i=first;
		//printf("555\n");
		if (i!=NULL)
			do 
			{
				//printf("6666\n");
				//printf("item - %s -> %d\n",item, strlen(item));
				//printf("i->item - %s ->%d\n",i-> item, strlen(i->item));
				if(strcmp(item,i->item)==0)
				{
					//printf("787");
					return count;
				}
				//printf("???");
				i=i->nexting;
			}
			while (i!=first);
		g=g->nextg;
    }
	return -1;
}*/
void DoubleLC_Insert(DoubleLC_type* lcfr, char * item)
{
  stream_sze++;
	//printf("before %s\n",item);
	LCD_Update(lcfr->T1, item);
	//printf("after\n");
	char * hasheditem=get_string(hashval(item, lcfr->a, lcfr->b, lcfr->modval));
	LCD_Update(lcfr->T2, hasheditem);
	//printf("after !!cm\n");
}
void DoubleLC_Report(DoubleLC_type* lcfr)
{
	//printf("Working");
  char** potentials = LCD_Report(lcfr->T1, lcfr->phi, sizeofdata);
  printf("\n\n\n\t\t\t\tDoubleLC output\n\n\n");
  int m=0, count;
  //printf("lcfr->phi * stream_sze - %lf\n" ,(lcfr->phi)*stream_sze);
  while(potentials[m]!=0){
    count = LCD_PointEst(lcfr->T1, potentials[m]);
    //printf("count -%d\n",count);
    //printf("lcfr->phi * stream_sze - %lf\n" ,(lcfr->phi)*stream_sze);
    if(count>(lcfr->phi)*stream_sze/2)
    {
    	char * hasheditem=get_string(hashval(potentials[m],lcfr->a, lcfr->b, lcfr->modval));
    	//printf("");
		int count_in_T2= LCD_PointEst(lcfr->T2,hasheditem);
		//printf("count_in_T2 -%d\n",count_in_T2);
		free(hasheditem);
		if(count_in_T2>=(lcfr->phi- lcfr->epsilon)*stream_sze)
		{
			printf("%s:%d\n", potentials[m], count);
		}		
    }
    //printf("done");
    free(potentials[m]);
    m++;
  }
  free(potentials);
  
}

void DoubleLC_Destroy(DoubleLC_type *lcfr)
{
  //	free(sfr->a);
  //	free(sfr->b);
	//LCD_Destroy(lcfr->T1);
	//LCD_Destroy(lcfr->T2);
	
	//free(lcfr);
}
