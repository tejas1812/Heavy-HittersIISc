#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "DoubleFrequent.h"
#include "../modified_massdalsketches/frequent.h"
#include "../massdalsketches/prng.h"
#include "../Hashing/hashing.h"


int main(int argc, char **argv) 
{

	stream_size=0;
	DoubleFrequent_type* dfr=DoubleFrequent_Init(0.01,0.0001,0.1);
	
    char *line =allocate_array(sizeofdata+1);
	int count;
	scanf("%d",&count);


	while(count>0)
	{
		scanf("%s",line);				
        DoubleFrequent_Insert(dfr,line);
		free(line);
		//change: line =allocate_array(sizeofdata+1);    
	    line=allocate_array(sizeofdata+1);
		--count;
    }
	//change: added free(line)
	free(line);
    DoubleFrequent_Report(dfr);
	DoubleFrequent_Destroy(dfr);
	return 0;
}
