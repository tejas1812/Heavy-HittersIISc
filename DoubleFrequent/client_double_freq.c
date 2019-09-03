#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "DoubleFrequent.h"
#include "../modified_massdalsketches/frequent.h"
#include "../modified_massdalsketches/lcdelta.h"
#include "../massdalsketches/prng.h"
#include "../Hashing/hashing.h"
#include <time.h>
int test =5;
double diff(struct timespec* s,struct timespec* e)
{
  double temp;
  temp = (e->tv_sec - s->tv_sec)*1000;
  temp = temp + (e->tv_nsec - s->tv_nsec)*0.000001;
  return temp;
}

int main(int argc, char **argv) 
{
	double epsilon;
  	scanf("EPS - %lf", &epsilon);
  	double phi;
  	scanf("PHI - %lf", &phi);
	stream_size=0;
	struct timespec s,e;	
    char *line =allocate_array(sizeofdata+1);
	int count;
	scanf("%d",&count);
	DoubleFrequent_type* dfr=DoubleFrequent_Init(phi,epsilon,0.1);
	DoubleLC_type* dlc=DoubleLC_Init(phi,epsilon,0.1);
	

	clock_gettime(CLOCK_REALTIME,&s);
	if(test==5){

    while(count>0){
      scanf("%s", line);
      DoubleFrequent_Insert(dfr,line);
      free(line);
      line=allocate_array(sizeofdata+1);
      --count;
    }
    free(line);
    clock_gettime(CLOCK_REALTIME, &e);
    printf("%f milliseconds\n",diff(&s,&e));
    printf("Size: %d\n",DoubleFrequent_Size(dfr));
    DoubleFrequent_Report(dfr);
    DoubleFrequent_Destroy(dfr);




  }
  else if(test==6){

    while(count>0){
      scanf("%s", line);
      DoubleLC_Insert(dlc,line);
      free(line);
      line=allocate_array(sizeofdata+1);
      --count;
    }
    clock_gettime(CLOCK_REALTIME, &e);
    printf("%f milliseconds\n",diff(&s,&e));
    printf("%d\n",DoubleLC_Size(dlc,sizeofdata));
    DoubleLC_Report(dlc);
    DoubleLC_Destroy(dlc);




  }
	return 0;
}
