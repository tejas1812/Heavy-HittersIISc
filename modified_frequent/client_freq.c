/********************************************************************
Approximate frequent items in a data stream
G. Cormode 2002

Last modified: 2003-10

*********************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "../massdalsketches/prng.h"
#include "../massdalsketches/massdal.h"

/******************************************************************/

#include "frequent.h"

/******************************************************************/




int main(int argc, char **argv) 
{

	if(argc<3)
	{
		printf("Usage: pass value of phi and epsilon (./exename phi epsilon)\n");
		exit(0);	
	}
	float phi=atof(argv[1]);
	float eps=atof(argv[2]);
	freq_type *fr=Freq_Init(eps);
	
	printf("k: %d\n",fr->k);
	printf("tblsz: %d\n",fr->tblsz);
	printf("a: %lli b: %lli\n",fr->a,fr->b);

	//ShowGroups(fr);

	Freq_Update(fr,1);
	//printf("after inserting 1\n");	
	//ShowGroups(fr);

	Freq_Update(fr,2);
	//printf("after inserting 2\n");	
	//ShowGroups(fr);

	Freq_Update(fr,3);
	//printf("after inserting 3\n");	
	//ShowGroups(fr);

	Freq_Update(fr,1);
	//printf("after inserting 1\n");	
	//ShowGroups(fr);

	Freq_Update(fr,2);
	//printf("after inserting 2\n");	
	//ShowGroups(fr);

	Freq_Update(fr,2);
	//printf("after inserting 2\n");	
	//ShowGroups(fr);

	Freq_Update(fr,2);
	//printf("after inserting 2\n");	
	//ShowGroups(fr);

	Freq_Update(fr,3);
	//printf("after inserting 3\n");	
	//ShowGroups(fr);

	printf("Displaying all groups\n");
	ShowGroups(fr);
	unsigned int * items1=Freq_Output(fr,0);
	for(int i=1;i<=fr->tblsz;++i)
		printf("%u ",items1[i]);
	printf("\n\n");

	printf("Only displaying groups with count > (phi-epsilon)*N \n");
	ShowGroups_gt_than_phiN(fr,phi);
	unsigned int * items=Freq_Output_gt_than_phiN(fr,phi);
	for(int i=1;i<=fr->tblsz;++i)
		printf("%u ",items[i]);
	printf("\n\n");

	Freq_Destroy(fr);

	return 0;
}

