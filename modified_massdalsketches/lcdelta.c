/********************************************************************
Implementation of Lossy Counting algorithm to Find Frequent Items
Based on the paper of Manku and Motwani, 2002
Implementation by G. Cormode 2002, 2003

Original Code: 2002-11
This version: 2003-10

This work is licensed under the Creative Commons
Attribution-NonCommercial License. To view a copy of this license,
visit http://creativecommons.org/licenses/by-nc/1.0/ or send a letter
to Creative Commons, 559 Nathan Abbott Way, Stanford, California
94305, USA.
*********************************************************************/
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "lcdelta.h"

#define LCDMULTIPLE 2

LCD_type * LCD_Init(float phi)
{
  LCD_type * result;

  result=(LCD_type *) calloc(1,sizeof(LCD_type));
  result->buckets=0;
  result->holdersize=0;
  result->epoch=0;
  result->epsilon = phi;
  
  result->window=1 + (int) 1.0/phi;
  result->maxholder=result->window*LCDMULTIPLE;
  result->bucket=(LCDCounter*) calloc(result->window+2,sizeof(LCDCounter));
  result->holder=(LCDCounter*) calloc(result->maxholder,sizeof(LCDCounter));
  result->newcount=(LCDCounter*) calloc(result->maxholder,sizeof(LCDCounter));
  return(result);
}

void LCD_Destroy(LCD_type * lc)
{
  /*  for(int i=0; i< lc->window+2;i++)
    free(lc->bucket[i].item);
  for(int i=0; i<lc->maxholder;i++)
    free(lc->holder[i].item);
  for(int i=0;i<lc->maxholder;i++)
    free(lc->newcount[i].item);
  free(lc->bucket);
  free(lc->holder);
  free(lc->newcount);
  free(lc);*/
}

void LCDShowCounters(LCDCounter * counts, int length)
{
  int i;

  for (i=0;i<length;i++)
    printf("  (item %s, count %d, delta %d)\n",
	   counts[i].item,counts[i].count,counts[i].delta);
}

int ccmp(const void *x, const void *y)
{
	//printf("in ccmp\n");
  // used in quicksort to sort lists to get the exact results for comparison
  const LCDCounter * h1=x; const LCDCounter * h2=y;

  if (strcmp((h1->item), (h2->item))<0) 
    return -1; 
  else if (strcmp((h1->item), (h2->item))>0)
    return 1;
  else return 0;
}


void lcdcountershell(unsigned long n, LCDCounter a[])
{
  unsigned long i,j,inc;
  LCDCounter v;
  inc=1;
  do {
    inc *= 3;
    inc++;
  } while (inc <= n);
  do {
    inc /= 3;
    for (i=inc+1;i<=n;i++) {
      v=a[i-1];
      j=i;
      while (strcmp(a[j-inc-1].item, v.item)>0) {
	a[j-1]=a[j-inc-1];
	j -= inc;
	if (j < inc) break;
      }
      a[j-1]=v;
    }
  } while (inc > 1);
}

int lcdcountermerge(LCDCounter *newcount, LCDCounter *left, LCDCounter *right,
		      int l, int r, int maxholder, int epoch)
{
	//printf("lcdcountermerge\n");
  // merge up two lists of counters. returns the size of the lists.
  int i,j,m;

  if (l+r>maxholder)
    { // a more advanced implementation would do a realloc here...
      printf("Out of memory -- trying to allocate %d counters\n",l+r);
      exit(1);
    }
  i=0;
  j=0;
  m=0;
  // interpretation: left is the new items, right is the existing list
  //  printf("l: %d, r: %d, maxholder: %d, epoch: %d\n", l, r, maxholder, epoch);
  while (i<l && j<r)
    { // merge two lists
      //      printf("merging with %s having count %d\n", right[j].item, right[j].count);
      if (strcmp(left[i].item, right[j].item)==0)
	{ 
	 // sum the counts of identical items
	  newcount[m].item=left[i].item;
	  newcount[m].count=right[j].count;
	  newcount[m].delta=right[j].delta;
	  while (i<l && j<r && strcmp(left[i].item, right[j].item)==0)
	    {  // merge in multiple copies of the same new item
	      newcount[m].count+=left[i].count;
	      i++;
	    }
	  j++;
	}
      else if (strcmp(left[i].item, right[j].item)<0)
	{	// else take the left item, creating counts appropriately
	  newcount[m].item=left[i].item;
	  newcount[m].count=0;
	  newcount[m].delta=left[i].delta;
	  while (i<l && strcmp(left[i].item, newcount[m].item)==0)
	    { // merge in multiple new copies of the same item
	      newcount[m].count+=left[i].count;
	      // need to update deltas here? 
	      i++;
	    }
	}
      else { 
	newcount[m].item=right[j].item;
	newcount[m].count=right[j].count;
	newcount[m].delta=right[j].delta;
	j++;
      }
      //newcount[m].count--;
      if (newcount[m].count+newcount[m].delta > epoch)
	//if (newcount[m].count>0) 
	m++;
      else
	{ // overwrite an item that is not worth keeping
	  newcount[m].item="";
	  newcount[m].count=0;
	  newcount[m].delta=0;
	}
    }
  //  printf("done with main loop, m =%d\n",m);
  // now that the main part of the merging has been done
  // need to copy over what remains of whichever list is not used up

  if (j<r)
    {
      while (j<r)
	{
	  if (right[j].count > 1)
	    {
	      newcount[m].item=right[j].item;
	      newcount[m].count=right[j].count;
	      newcount[m].delta=right[j].delta;
	      if (newcount[m].count+newcount[m].delta > epoch)
		//if (newcount[m].count>0) 
		m++;
	    }
	  j++;
	}
    }
  else
    if (i<l)
      {
	//	printf("hiya, l=%d!\n",l);
	while(i<l)
	    {
	      //	      printf("Value of m: %d\n", m);
	      newcount[m].item=left[i].item;
	      newcount[m].delta=left[i].delta;
	      newcount[m].count=0;
	      while ((i<l) && strcmp(newcount[m].item, left[i].item)==0)
		{
		  newcount[m].count+=left[i].count;
		  //if(newcount[m].count>1)
		    //		    printf("Incremented count of %s to %d\n", newcount[m].item, newcount[m].count);
		  //		  printf("Value of i = %d\n",i);
		  i++;
		}
	      //	      printf("Exited loop for %s\n", newcount[m].item);
	      if (newcount[m].count+newcount[m].delta > epoch)
		m++;
	      else
		{
		  newcount[m].item="";
		  newcount[m].count=0;
		  newcount[m].delta=0;
		}
	    }
	//	printf("Done with consolidation!");
      }
  return(m);
}


void LCD_Update(LCD_type * lc, char* val)
{
  LCDCounter *tmp;

  streamsize +=1;

  
  //printf(" {%d}, ",val);

  lc->bucket[lc->buckets].item = (char*)malloc(strlen(val));
  lc->bucket[lc->buckets].item=strdup(val);
  lc->bucket[lc->buckets].count=1;
  lc->bucket[lc->buckets].delta=lc->epoch;
  lc->buckets++;
  //  printf("buckets = %d, window=%d\n", lc->buckets, lc->window);

  if (lc->buckets==lc->window)
    {
      lc->epoch++;
      qsort(lc->bucket,lc->window,sizeof(LCDCounter),ccmp);
      lcdcountershell(lc->window,lc->bucket);

       // printf("Now going to merge\n");
      lc->holdersize=lcdcountermerge(lc->newcount,lc->bucket,lc->holder,
				    lc->window,
				    lc->holdersize,lc->maxholder,lc->epoch);
      //printf("Done with merge\n");
      tmp=lc->newcount;
      lc->newcount=lc->holder;
      lc->holder=tmp;
      lc->buckets=0;
    }
}

int LCD_Size(LCD_type * lc, int maxlength)
{

  return (lc->holdersize)*maxlength+(lc->maxholder+lc->window)*sizeof(LCDCounter)+sizeof(LCD_type);
}

int LCD_PointEst(LCD_type * lcd, char* item)
{
  int i;

  for (i=0;i<lcd->holdersize;i++)
    if (strcmp(lcd->holder[i].item, item) == 0)
      return(lcd->holder[i].count + lcd->holder[i].delta);
  return 0;
}


char** LCD_Report(LCD_type * lc, float phi, int maxlength){
  int i;
  LCDCounter *tmp;
  char** output = (char**)calloc((int)(1/(phi - lc->epsilon))+1, sizeof(char*));
	//if(lc->buckets==lc->window)
//{
	//qsort(lc->bucket,lc->window,sizeof(LCDCounter),ccmp);
  qsort(lc->bucket,lc->buckets,sizeof(LCDCounter),ccmp);
	//lcdcountershell(lc->window,lc->bucket);
  lcdcountershell(lc->buckets,lc->bucket);

  printf("Now going to merge\n");
	//lc->holdersize=lcdcountermerge(lc->newcount,lc->bucket,lc->holder,
		//		 lc->window,
			//	 lc->holdersize,lc->maxholder,lc->epoch);
  lc->holdersize=lcdcountermerge(lc->newcount,lc->bucket,lc->holder,
				 lc->buckets,
				 lc->holdersize,lc->maxholder,lc->epoch);
    //printf("Done with merge\n");
  tmp=lc->newcount;
  lc->newcount=lc->holder;
  lc->holder=tmp;
  lc->buckets=0;
//}


  //  printf("holdersize: %d\n", lc->maxholder);
  //  printf("threshold: %d\n", (int)((phi - lc->epsilon)*streamsize));
  int m=0;
  for(i=0; i<lc->maxholder;i++){
    if ((lc->holder[i].count + lc->holder[i].delta) > (phi - lc->epsilon)*streamsize){
      printf("%s: %d\n", lc->holder[i].item, lc->holder[i].count + lc->holder[i].delta);
      output[m] = lc->holder[i].item;
      m++;
    }
}
  return output;
}

/* unsigned int *  LCD_Output(LCD_type * lc, int thresh) */
/* { */
/*   //int correct=0; */
/*   //int claimed=0; */
/*   int i,point; */

/*   unsigned int * results; */

/*   results=(char**) calloc(lc->window, sizeof(unsigned int)); */
/*   point=1; */
/*   // should do a countermerge here. */

/*   for (i=0;i<lc->holdersize;i++) */
/*     { */
/*       if (lc->holder[i].count+lc->holder[i].delta>=thresh) */
/* 	results[point++]=lc->holder[i].item; */
/*     } */
/*   results[0]=point-1; */
/*   return results; */
/* } */
