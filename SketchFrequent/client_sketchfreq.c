#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "SketchFrequent.h"
#include "../modified_massdalsketches/countmin.h"
#include "../modified_massdalsketches/frequent.h"
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
	SketchFrequent_type* sfr=SketchFrequent_Init(0.01,0.0001,0.1);
	struct timespec s,e;
     char *line =allocate_array(sizeofdata+1);
	int count;
	scanf("%d",&count);

	clock_gettime(CLOCK_REALTIME,&s);
	while(count>0)
	{
		scanf("%s",line);			
        SketchFrequent_Insert(sfr,line);
		free(line);
        line=allocate_array(sizeofdata+1);
		--count;
    }
	free(line);
	clock_gettime(CLOCK_REALTIME,&e);
	printf("%f milliseconds\n",diff(&s,&e));
	printf("%d\n",SketchFrequent_Size(sfr));
    SketchFrequent_Report(sfr);
	SketchFrequent_Destroy(sfr);
   
	return 0;
}
