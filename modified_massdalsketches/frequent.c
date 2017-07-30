/********************************************************************
Implementation of Frequent algorithm to Find Frequent Items
Based on papers by:
  Misra and Gries, 1982
  Demaine, Lopez-Ortiz, Munroe, 2002
  Karp, Papadimitriou and Shenker, 2003
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
#include "frequent.h"
#include "../massdalsketches/prng.h"
#include "../Hashing/hashing.h"



//global variable assigned a value inside Freq_Init
float epsilon=0.0;

char *zero_string=NULL;

/*
unsigned int * Freq_Output_gt_than_phiN(freq_type * freq, float phi)
{
  GROUP *g;
  ITEMLIST *i,*first;
  int count=0;
  unsigned int * results;
  int point=1;

  results=(unsigned int *) calloc(1+freq->tblsz, sizeof(unsigned int));
  g=freq->groups->nextg;
  while (g!=NULL) 
    {
      count=count+g->diff;
      first=g->items;
      i=first;
      if (i!=NULL && count>(phi-epsilon)*stream_size)
	do 
	  {
	    //printf("Next item: %d \n",i->item);
	    results[point++]=i->item;
	    i=i->nexting;
	  }
	while (i!=first);
      g=g->nextg;
    }
  results[0]=point-1;
  //printf("I found %d items\n",point);
  return(results);
}
*/
void ShowGroups_gt_than_phiN(freq_type * freq, float phi) 
{
  GROUP *g;
  ITEMLIST *i,*first;
  int count;
  
  g=freq->groups;
  count=0;
  while (g!=NULL) 
    {
      count=count+g->diff;

			//print the group only if the estimated count is greater than (phi-epsilon)*N
			if(count>=(phi-epsilon)*stream_size)
			{		
		    printf("Group %d :",count);
		    first=g->items;
		    i=first;
		    if (i!=NULL)
					do 
					{
			  		printf("%s -> ",i->item);
			  		i=i->nexting;
					}
					while (i!=first);
		    else printf(" empty");
		    do 
				{
					printf("%s <- ",i->item);
					i=i->previousing;
				}
		    while (i!=first);
		    printf(")\n");
			}      
			g=g->nextg;
      if ((g!=NULL) && (g->previousg->nextg!=g))
				printf("Badly linked\n");
    }
}

void ShowGroups(freq_type * freq) 
{
  GROUP *g;
  ITEMLIST *i,*first;
  int count;
  
  g=freq->groups;
  count=0;
  while (g!=NULL) 
    {
      count=count+g->diff;
      printf("Group %d :",count);
      first=g->items;
      i=first;
      if (i!=NULL)
	do 
	  {
	    printf("%s -> ",i->item);
	    i=i->nexting;
	  }
	while (i!=first);
      else printf(" empty");
      do 
	{
	  printf("%s <- ",i->item);
	  i=i->previousing;
	}
      while (i!=first);
      printf(")");
      g=g->nextg;
      if ((g!=NULL) && (g->previousg->nextg!=g))
	printf("Badly linked");
      printf("\n");
    }
}
/*
unsigned int * Freq_Output(freq_type * freq, int thresh)
{
  GROUP *g;
  ITEMLIST *i,*first;
  int count=0;
  unsigned int * results;
  int point=1;

  results=(unsigned int *) calloc(1+freq->tblsz, sizeof(unsigned int));
  g=freq->groups->nextg;
  while (g!=NULL) 
    {
      count=count+g->diff;
      first=g->items;
      i=first;
      if (i!=NULL)
	do 
	  {
	    //printf("Next item: %d \n",i->item);
	    results[point++]=i->item;
	    i=i->nexting;
	  }
	while (i!=first);
      g=g->nextg;
    }
  results[0]=point-1;
  //printf("I found %d items\n",point);
  return(results);
}
*/

ITEMLIST * GetNewCounter(freq_type * freq, long long modval)
{
  ITEMLIST * newi;
  int j;
  
  newi=freq->groups->items;  // take a counter from the pool
  freq->groups->items=freq->groups->items->nexting; 
  
  newi->nexting->previousing=newi->previousing;
  newi->previousing->nexting=newi->nexting;
  // unhook the new item from the linked list	    
    
    // need to remove this item from the hashtable
	//change:
	char* hasheditem=get_string(hashval(newi->item,freq->a,freq->b,modval));
  j=mod_arrays2( hasheditem, freq->tblsz);
	free(hasheditem);
  if (freq->hashtable[j]==newi)
    freq->hashtable[j]=newi->nexti;
  
  if (newi->nexti!=NULL)
    newi->nexti->previousi=newi->previousi;
  if (newi->previousi!=NULL)
    newi->previousi->nexti=newi->nexti;

  return (newi);
}

void InsertIntoHashtable(freq_type * freq, ITEMLIST *newi, int i, char* newitem)
{
  newi->nexti=freq->hashtable[i];
	//if(newi->item!=zero_string) 
		{free(newi->item);}  
	newi->item=strdup(newitem);
  newi->previousi=NULL;
  // insert item into the hashtable
    
    if (freq->hashtable[i]!=NULL)
      freq->hashtable[i]->previousi=newi;
  freq->hashtable[i]=newi;
}

void InsertIntoFirstGroup(freq_type * freq, ITEMLIST *newi)
{
  GROUP * firstg;
  
  firstg=freq->groups->nextg;
  newi->parentg=firstg; 
  /* overwrite whatever was in the parent pointer */
  newi->nexting=firstg->items;
  newi->previousing=firstg->items->previousing;
  newi->previousing->nexting=newi;
  firstg->items->previousing=newi;
}

void CreateFirstGroup(freq_type * freq, ITEMLIST *newi) 
{
  GROUP *newgroup, *firstg;
  
  firstg=freq->groups->nextg;
  newgroup=malloc(sizeof(GROUP));
  newgroup->diff=1;
  newgroup->items=newi;
  newi->nexting=newi;
  newi->previousing=newi;
  newi->parentg=newgroup; 
  // overwrite whatever was there before
  newgroup->nextg=firstg;
  newgroup->previousg=freq->groups;
	//change
	//if(freq->groups->nextg!=NULL) free(freq->groups->nextg);
  freq->groups->nextg=newgroup;
  if (firstg!=NULL)
    {
      firstg->previousg=newgroup;	      
      firstg->diff--;
    }
}

void PutInNewGroup(ITEMLIST *newi, GROUP * tmpg)
{ 
  GROUP * oldgroup;

  oldgroup=newi->parentg;  
  // put item in the tmpg group
    newi->parentg=tmpg;

  if (newi->nexting!=newi) 
    { // remove the item from its current group
	newi->nexting->previousing=newi->previousing;
      newi->previousing->nexting=newi->nexting;
      oldgroup->items=oldgroup->items->nexting;
    }
  else { 
    if (oldgroup->diff!=0) 
      {
	if (oldgroup->nextg!=NULL)
	  {
	    oldgroup->nextg->diff+=oldgroup->diff;
	    oldgroup->nextg->previousg=oldgroup->previousg;
	  }
	oldgroup->previousg->nextg=oldgroup->nextg;
	free(oldgroup);      
	/* if we have created an empty group, remove it 
	   but avoid deleting the first group */
      }
  }	
  newi->nexting=tmpg->items;
  newi->previousing=tmpg->items->previousing;
  newi->previousing->nexting=newi;
  newi->nexting->previousing=newi;
}

void AddNewGroupAfter(ITEMLIST *newi, GROUP *oldgroup)
{
  GROUP *newgroup;
  
  // remove item from old group...
  newi->nexting->previousing=newi->previousing;
  newi->previousing->nexting=newi->nexting;
  oldgroup->items=newi->nexting;
  newgroup=malloc(sizeof(GROUP));	       
  newgroup->diff=1;
  newgroup->items=newi;
  newgroup->previousg=oldgroup;
  newgroup->nextg=oldgroup->nextg;
  oldgroup->nextg=newgroup;
  if (newgroup->nextg!=NULL) 
    {
      newgroup->nextg->diff--;
      newgroup->nextg->previousg=newgroup;
    }
  newi->parentg=newgroup;
  newi->nexting=newi;
  newi->previousing=newi;
}

void AddNewGroupBefore(ITEMLIST *newi, GROUP *oldgroup)
{
  GROUP *newgroup;
  
  // remove item from old group...
  newi->nexting->previousing=newi->previousing;
  newi->previousing->nexting=newi->nexting;
  oldgroup->items=newi->nexting;
  newgroup=malloc(sizeof(GROUP));	       
  newgroup->diff=oldgroup->diff-1;
  oldgroup->diff=1;
  
  newgroup->items=newi;
  newgroup->nextg=oldgroup;
  newgroup->previousg=oldgroup->previousg;
  oldgroup->previousg=newgroup;
  if (newgroup->previousg!=NULL) 
    {
      newgroup->previousg->nextg=newgroup;
    }
  newi->parentg=newgroup;
  newi->nexting=newi;
  newi->previousing=newi;
}


void DeleteFirstGroup(freq_type * freq)
{
  GROUP *tmpg;

  freq->groups->nextg->items->previousing->nexting=
    freq->groups->items->nexting;
  freq->groups->items->nexting->previousing=
    freq->groups->nextg->items->previousing;
  freq->groups->nextg->items->previousing=
    freq->groups->items;
  freq->groups->items->nexting=
    freq->groups->nextg->items;
  /* phew!  that has merged the two circular doubly linked lists */
  
  tmpg=freq->groups->nextg;
  freq->groups->nextg->diff=0;
  freq->groups->nextg=freq->groups->nextg->nextg;
  if (freq->groups->nextg!=NULL)
    freq->groups->nextg->previousg=freq->groups;
  tmpg->nextg=NULL;
  tmpg->previousg=NULL;
}

void IncrementCounter(ITEMLIST *newi)
{
  GROUP *oldgroup;
  
  oldgroup=newi->parentg;
  if ((oldgroup->nextg!=NULL) && (oldgroup->nextg->diff==1))
    PutInNewGroup(newi,oldgroup->nextg);
  // if the next group exists
  else 
    { 
      // need to create a new group with a differential of one
	if (newi->nexting==newi) 
	  {
	    newi->parentg->diff++;
	    if (newi->parentg->nextg!=NULL)
	      newi->parentg->nextg->diff--;
	  }
	else      
	  AddNewGroupAfter(newi,oldgroup);
    }
}

void SubtractCounter(ITEMLIST *newi)
{
  GROUP *oldgroup;

  oldgroup=newi->parentg;
  if ((oldgroup->previousg!=NULL) && (oldgroup->diff==1))
    PutInNewGroup(newi,oldgroup->previousg);
  else
    {
      if (newi->nexting==newi)
	{
	  newi->parentg->diff--;
	  if (newi->parentg->nextg!=NULL)
	    newi->parentg->nextg->diff++;
	}
      else
	AddNewGroupBefore(newi,oldgroup);
    }
}


void DecrementCounts(freq_type * freq)
{
  if ((freq->groups->nextg!=NULL) && (freq->groups->nextg->diff>0)) 
    {
      freq->groups->nextg->diff--;
      if (freq->groups->nextg->diff==0) 
	DeleteFirstGroup(freq);
      /* need to delete the first group... */
    }
}

void FirstGroup(freq_type * freq, ITEMLIST *newi)
{
  if ((freq->groups->nextg!=NULL) && (freq->groups->nextg->diff==1)) 
    InsertIntoFirstGroup(freq,newi);
  /* if the first group starts at 1... */
  else 
    CreateFirstGroup(freq,newi);
  /* need to create a new first group */
  /* and we are done, we don't need to decrement */
}

void RecycleCounter(freq_type * freq, ITEMLIST *il) 
{
  if (il->nexting==il)
    DecrementCounts(freq);
  else 
    {
      if (freq->groups->items==il)    
	freq->groups->items=freq->groups->items->nexting; 
      /* tidy up here in case we have emptied a defunct group? 
       need an item counter in order to do this */
      il->nexting->previousing=il->previousing;
      il->previousing->nexting=il->nexting;
      FirstGroup(freq,il);
      /* Needed to sort out what happens when we insert an item
	 which has a counter but its counter is zero
	 */
    }
}

//needs access to dfr->modval
void Freq_Update(freq_type * freq, char* newitem, long long modval) 
{
	
  int i;
  ITEMLIST *il;
  //int diff=1;
  stream_size+=1;	
	//change
	char* hasheditem=get_string(hashval(newitem,freq->a,freq->b,modval));
  i=mod_arrays2( hasheditem, freq->tblsz);
	free(hasheditem);
  il=freq->hashtable[i];
  while (il!=NULL) {
    if (strcmp(il->item,newitem)==0) 
      break;
    il=il->nexti;
  }
  if (il==NULL) 
    {
	  /* item is not monitored (not in hashtable) */
	  if (freq->groups->items->nexting!=freq->groups->items)
	    { 
	      /* if there is space for a new item */
	      il=GetNewCounter(freq,modval);
	      /* and put it into the hashtable for the new item */
	      InsertIntoHashtable(freq,il,i,newitem);
	      FirstGroup(freq,il);
	    }
         else
	  DecrementCounts(freq);
	
      /* else, delete an item that isnt there, ignore it */      
    }
  else 
      if (il->parentg->diff==0)
	RecycleCounter(freq,il);
      else
	IncrementCounter(il);
}
  
freq_type * Freq_Init(float eps, char *a, char *b)
{
	//setting a global variable epsilon to equal the value specified by the user.
	epsilon=eps;

  ITEMLIST *inititem;
  ITEMLIST *previtem;

  int i,k;
  //int hashspace,groupspace,itemspace;
  freq_type * result;

  k=(int) ceil(1.0/eps);
  if (k<1) k=1;
  result=calloc(1,sizeof(freq_type));

	//shallow copy okay? check. otherwise use strdup
  result->a=a;
 	result->b=b;

  result->k=k;
  result->tblsz=2*k;  
  result->hashtable=calloc(2*k+2,sizeof(ITEMLIST *));
  //hashspace=(2*k+2)*sizeof(ITEMLIST *);
  for (i=0; i<2*k;i++) 
    result->hashtable[i]=NULL;
  
  result->groups=malloc(sizeof(GROUP));
  result->groups->diff=0;
  result->groups->nextg=NULL;
  result->groups->previousg=NULL;
  previtem=malloc(sizeof(ITEMLIST));
  result->groups->items=previtem;
  previtem->nexti=NULL;
  previtem->previousi=NULL;
  previtem->parentg=result->groups;
  previtem->nexting=previtem;
  previtem->previousing=previtem;
	
	//using string containing zeroes instead of 0
	if(zero_string==NULL)
	{
		zero_string=malloc(num_of_elements);
		memset(zero_string,'0',num_of_elements);
		zero_string[num_of_elements-1]='\0';
	}
	//previtem->item=zero_string;
  previtem->item=strdup(zero_string);
  
	//groupspace=k*sizeof(GROUP);

  for (i=1;i<=k;i++) 
    {
      inititem=malloc(sizeof(ITEMLIST));
	//	inititem->item=zero_string;      
	inititem->item=strdup(zero_string);
      inititem->parentg=result->groups;
      inititem->nexti=NULL;
      inititem->previousi=NULL;
      inititem->nexting=previtem;
      inititem->previousing=previtem->previousing;
      previtem->previousing->nexting=inititem;
      previtem->previousing=inititem;      
    }

  //itemspace=(k+1)*sizeof(ITEMLIST);
  return(result);
}  

int Freq_Size(freq_type * freq)
{
  int size;

  size=2*(freq->tblsz)*sizeof(ITEMLIST) + (freq->k + 1)*sizeof(ITEMLIST) + 
    (freq->k)*sizeof(GROUP);
  return size;

}
void Freq_Destroy(freq_type * freq)
{
  // placeholder implementation: need to go through and free 
  // all memory associated with the data structure explicitly
	if(zero_string!=NULL) {free(zero_string); zero_string=NULL;}
	GROUP *g=freq->groups;
	ITEMLIST *i,*first;
	while (g!=NULL) 
	{
		first=g->items;
		i=first;
		if (i!=NULL)
			do 
			{
				ITEMLIST * temp=i;
				i=i->nexting;
				//if(g!=freq->groups) 
				free(temp->item);
				free(temp);
			}
			while (i!=first);
		GROUP *tempg=g;		
		g=g->nextg;
		free(tempg);
    }
	free(freq->hashtable);	 
	free (freq);
}  
