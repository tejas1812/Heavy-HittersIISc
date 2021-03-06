#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "DoubleLC.h"
#include "../modified_massdalsketches/countmin.h"
#include "../massdalsketches/prng.h"
#include "../Hashing/hashing.h"
#include <time.h>
double diff(struct timespec* s,struct timespec* e)
{
  double temp;
  temp = (e->tv_sec - s->tv_sec)*1000;
  temp = temp + (e->tv_nsec - s->tv_nsec)*0.000001;
  return temp;
}

int main(int argc, char **argv) 
{

	
	DoubleLC_type* lcfr=DoubleLC_Init(0.01,0.0001,0.1);
	struct timespec s,e;	
    char *line =allocate_array(sizeofdata+1);
	int count;
	scanf("%d",&count);

	clock_gettime(CLOCK_REALTIME,&s);
	while(count>0)
	{
		scanf("%s",line);			
        DoubleLC_Insert(lcfr,line);
		free(line);
		//change: line =allocate_array(sizeofdata+1);    
	    line=allocate_array(sizeofdata+1);
		--count;
    }
	//change: added free(line)
	free(line);
	clock_gettime(CLOCK_REALTIME,&e);
	printf("%f milliseconds\n",diff(&s,&e));
	//printf("Size: %d\n",DoubleLC_Size(lcfr));
    DoubleLC_Report(lcfr);
	DoubleLC_Destroy(lcfr);
	return 0;
}
