/********************************************************************
Implementation of Lazy Lossy Counting algorithm to Find Frequent Items
Based on the paper of Manku and Motwani, 2002
And Metwally, Agrwawal and El Abbadi, 2005
Implementation by G. Cormode 2002, 2003, 2005

Original Code: 2002-11
This version: 2005-08

This work is licensed under the Creative Commons
Attribution-NonCommercial License. To view a copy of this license,
visit http://creativecommons.org/licenses/by-nc/1.0/ or send a letter
to Creative Commons, 559 Nathan Abbott Way, Stanford, California
94305, USA.
*********************************************************************/
#include <stdlib.h>
#include <stdio.h>
#include "lclazy.h"
#include "prng.h"
//#include <values.h>

//#define NULLITEM MAXINT
#define NULLITEM MOD // borrow value of 2^31 -1 from prng.h
#define HASHMULT 3

LCL_type * LCL_Init(float phi)
{
  LCL_type * result;
  int i;

  result=(LCL_type *) calloc(1,sizeof(LCL_type));
  result->hasha=151261303;
  result->hashb=6722461; // hard coded constants for the hash table, 
  //should really generate these randomly

  result->n=0;
  result->size = (1 + (int) (1.0/phi)) | 1; // ensure that size is odd
  result->hashsize = HASHMULT*result->size;
  result->hashtable=(int *) calloc(result->hashsize,sizeof(int));
  result->counters=(LCLCounter*) calloc(1+result->size,sizeof(LCLCounter));
  for (i=1; i<=result->size;i++)
    {
      result->counters[i].next=0;
      result->counters[i].prev=0;
      result->counters[i].item=NULLITEM;
    }
  return(result);
}

void LCL_Destroy(LCL_type * lcl)
{
  free(lcl->hashtable);
  free(lcl->counters);
  free(lcl);
}

void CheckHash(LCL_type * lcl, int item, int hash)
{
  int i, hashptr, prev;

  for (i=0; i<lcl->hashsize;i++)
    {
      prev=0;
      hashptr=lcl->hashtable[i];
      while (hashptr) {
	if (lcl->counters[hashptr].hash!=i)
	  {
	    printf("\n Hash violation! hash = %d, should be %d \n", 
		   lcl->counters[hashptr].hash,i);
	    printf("after inserting item %d with hash %d\n", item, hash);
	  }
	if (lcl->counters[hashptr].prev!=prev)
	  {
	    printf("\n Previous violation! prev = %d, should be %d\n",
		   lcl->counters[hashptr].prev, prev);
	    printf("after inserting item %d with hash %d\n",item, hash);
	    exit(EXIT_FAILURE);
	  }
	prev=hashptr;
	hashptr=lcl->counters[hashptr].next;
      }
    }
}

void ShowHash(LCL_type * lcl)
{
  int i, hashptr, prev;

  for (i=0; i<lcl->hashsize;i++)
    {
      prev=0;
      printf("%d:",i);
      hashptr=lcl->hashtable[i];
      while (hashptr) {
	printf(" %d [h(%u) = %d, prev = %d] ---> ",hashptr,
	       (unsigned int) lcl->counters[hashptr].item,
	       lcl->counters[hashptr].hash,
	       lcl->counters[hashptr].prev);
	prev=hashptr;
	hashptr=lcl->counters[hashptr].next;
      }
      printf(" *** \n");
    }
}
  
void ShowHeap(LCL_type * lcl)
{
  int i, j;

  j=1;
  for (i=1; i<=lcl->size; i++)
    {
      printf("%d ",lcl->counters[i].count);
      if (i==j) 
	{ 
	  printf("\n");
	  j=2*j+1;
	}
    }
  printf("\n\n");
}

void Heapify(LCL_type * lcl, int ptr)
{
  int minchild;
  LCLCounter tmp;

  while(1)
    {
      if (2*ptr + 1>lcl->size) return;
      // if the current node has no children
      minchild=2*ptr+
	((lcl->counters[2*ptr].count<lcl->counters[2*ptr+1].count)? 0 : 1);
      // compute which child is the lesser of the two

      if (lcl->counters[ptr].count < lcl->counters[minchild].count) return;
      // if the parent is less than the smallest child, we can stop

      tmp=lcl->counters[ptr];
      lcl->counters[ptr]=lcl->counters[minchild];
      lcl->counters[minchild]=tmp;
      // else, swap the parent and child

      if (lcl->counters[ptr].hash==lcl->counters[minchild].hash)
	{ 
	  // swap the prev and next pointers back. 
	  // if the two items are in the same linked list
	  // this avoids some nasty buggy behaviour
	  lcl->counters[minchild].prev=lcl->counters[ptr].prev;
	  lcl->counters[ptr].prev=tmp.prev;
	  lcl->counters[minchild].next=lcl->counters[ptr].next;
	  lcl->counters[ptr].next=tmp.next;
	}
      else
	{ // ensure that the poiners in the linked list ae set correctly
	  // check: hashtable has correct pointer (if prev ==0)
	  if (lcl->counters[ptr].prev==0)
	    {
	      if (lcl->counters[ptr].item!=NULLITEM)
		lcl->hashtable[lcl->counters[ptr].hash]=ptr;
	    }
	  else
	    lcl->counters[lcl->counters[ptr].prev].next=ptr;
	  if (lcl->counters[ptr].next)
	    lcl->counters[lcl->counters[ptr].next].prev=ptr;
	  
	  if (lcl->counters[minchild].prev==0)
	    lcl->hashtable[lcl->counters[minchild].hash]=minchild;
	  else
	    lcl->counters[lcl->counters[minchild].prev].next=minchild;
	  if (lcl->counters[minchild].next)
	    lcl->counters[lcl->counters[minchild].next].prev=minchild;
	}
      ptr=minchild;
      // continue on with the heapify from the child position
    }  
}

int LCL_Update(LCL_type * lcl, LCLitem_t item, int value)
{
  int hashptr, hashval;
  //uint32_t olditem;

  // find whether new item is already stored, if so store it and add one
  // update heap property if necessary

  lcl->n+=value;

  hashval=(int) hash31(lcl->hasha, lcl->hashb,item) % lcl->hashsize;
  hashptr=lcl->hashtable[hashval];
  // compute the hash value of the item, and begin to look for it in 
  // the hash table

  while (hashptr) {
    if (lcl->counters[hashptr].item==item)
      break;
    else hashptr=lcl->counters[hashptr].next;
  }
  if (hashptr)
    {
      lcl->counters[hashptr].count+=value; // increment the count of the item
      Heapify(lcl,hashptr); // and fix up the heap
      return(lcl->counters[hashptr].count);
    }
  else
  // else, overwrite smallest heap item and reheapify if necessary
    // fix up linked list from hashtable
    {
      if (lcl->counters[1].prev==0) // if it is first in the list
	lcl->hashtable[lcl->counters[1].hash]=lcl->counters[1].next;
      else
	lcl->counters[lcl->counters[1].prev].next=lcl->counters[1].next;
      if (lcl->counters[1].next) // if it is not last in the list
	lcl->counters[lcl->counters[1].next].prev=lcl->counters[1].prev;
      // update the hash table appropriately to remove the old item

      // slot new item into hashtable
      lcl->counters[1].next=lcl->hashtable[hashval];
      if (lcl->hashtable[hashval])
	lcl->counters[lcl->hashtable[hashval]].prev=1;
      lcl->hashtable[hashval]=1;
      // this is the smallest heap item, so we are looking in 
      //slot 1 all the time
      lcl->counters[1].prev=0;
      lcl->counters[1].item=item;
      lcl->counters[1].hash=hashval;
      lcl->counters[1].delta=lcl->counters[1].count;
      value+=lcl->counters[1].delta;
      lcl->counters[1].count=value;
      Heapify(lcl,1);
      return value;
    }
}

int LCL_Size(LCL_type * lcl)
{
  int size;
  size=sizeof(LCL_type) + (lcl->hashsize * sizeof(int)) + 
    (lcl->size*sizeof(LCLCounter));
  return size;
}

int LCL_PointEst(LCL_type * lcl, LCLitem_t item)
{
  int i;

  for (i=1;i<=lcl->size;i++)
    if (lcl->counters[i].item==item)
      return(lcl->counters[i].count);
  return 0;
}

int LCL_PointErr(LCL_type * lcl, LCLitem_t item)
{
  int i;

  for (i=1;i<=lcl->size;i++)
    if (lcl->counters[i].item==item)
      return(lcl->counters[i].delta);
  return lcl->counters[1].delta;
}


LCLitem_t *  LCL_Output(LCL_type * lcl, int thresh)
{
  int i,point;
  LCLitem_t * results;
  //uint32_t * results;

  results=(LCLitem_t *) calloc(lcl->size, sizeof(LCLitem_t));
  //results=(uint32_t *) calloc(lcl->size, sizeof(uint32_t));
  point=1;

  for (i=1;i<=lcl->size;i++)
    {
      if (lcl->counters[i].count>=thresh)
	results[point++]=lcl->counters[i].item;
    }
  results[0]=(LCLitem_t) point-1;
  return results;
}
