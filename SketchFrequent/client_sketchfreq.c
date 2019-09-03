#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "../DoubleFrequent/DoubleFrequent.h"
#include "../DoubleFrequent/DoubleLC.h"
#include "SketchFrequent.h"
#include "SketchLC.h"
#include "../modified_massdalsketches/countmin.h"
#include "../modified_massdalsketches/frequent.h"
#include "../modified_massdalsketches/lcdelta.h"
#include "../massdalsketches/prng.h"
#include "../Hashing/hashing.h"
#include <time.h>

int test =6;

double diff(struct timespec* s,struct timespec* e)
{
  double temp;
  temp = (e->tv_sec - s->tv_sec)*1000;
  temp = temp + (e->tv_nsec - s->tv_nsec)*0.000001;
  return temp;
}

int main(int argc, char **argv) 
{
//  int* test ={1,2,3,4};
//  double* epsilon={0.0001,0.001,0.005,0.01.0.05}
//  double* phi={0.01,0.001,0.0001,0.05,0.005}
  double epsilon=0.0005;
  //scanf("%lf", &epsilon);
  double phi=0.01;
  //scanf("%lf", &phi);
//  double c_phi =0.01;
//  double c_eps =0.0001;
//  for(
  struct timespec s,e;
  char *line =allocate_array(sizeofdata+1);
  int count;
  scanf("%d",&count);

  SketchFrequent_type* sfr=SketchFrequent_Init(phi,epsilon ,0.1);
  freq_type* ft = Freq_Init(epsilon, sizeofdata);
  LCD_type* lc = LCD_Init(epsilon);
  SketchLC_type* slc = SketchLC_Init(phi, epsilon, 0.1);
  DoubleFrequent_type* dfr=DoubleFrequent_Init(phi,epsilon,0.1);
  DoubleLC_type* dlc=DoubleLC_Init(phi,epsilon,0.1);
  CM_type * cm = CM_Init((long)2.0/(epsilon), (long)2.0*log10l(1/(phi*0.1)), sizeofdata);
  char **items = (char**)malloc(count);
  clock_gettime(CLOCK_REALTIME,&s);

  //int test;
  //int count;
  //for(test=1;test<2;test++)
  //{
  //count= f_count;
  //printf("\n\n\n\n\n\n");
  if(test == 1){
    while(count>0){
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
  }
  else if(test==2){
    while(count>0){
      scanf("%s",line);			
      Freq_Update(ft, line, sfr->w);
      free(line);
      line=allocate_array(sizeofdata+1);
      --count;
    }
    free(line);
    clock_gettime(CLOCK_REALTIME,&e);
    printf("%f milliseconds\n",diff(&s,&e));
    printf("%d\n",Freq_Size(ft));
    ShowGroups_gt_than_phiN(ft, phi);
    Freq_Destroy(ft);
  }
  else if(test==3){
    //printf("Are you here?");
    while(count>0){
      scanf("%s", line);
      LCD_Update(lc, line);
      free(line);
      line=allocate_array(sizeofdata+1);
      --count;
    }
    clock_gettime(CLOCK_REALTIME, &e);
    printf("%f milliseconds\n",diff(&s,&e));
    printf("%d\n",LCD_Size(lc,sizeofdata));
    LCD_Report(lc,phi,sizeofdata);
    LCD_Destroy(lc);
  }
  else if(test==4){

    while(count>0){
      scanf("%s", line);
      SketchLC_Insert(slc, line);
      free(line);
      line=allocate_array(sizeofdata+1);
      --count;
    }
    free(line);
    clock_gettime(CLOCK_REALTIME, &e);
    printf("%f milliseconds\n",diff(&s,&e));
    printf("%d\n",SketchLC_Size(slc,sizeofdata));
    SketchLC_Report(slc);
    SketchLC_Destroy(slc);




  }
  else if(test==5){

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
    printf("%d\n",DoubleFrequent_Size(dfr));
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
  else if(test==7){
    int i =0;
    int dup_count = count;
    while(count>0){
      scanf("%s", line);
      CM_Update(cm, line);
      items[i] = strdup(line);
      i++;
      free(line);
      line=allocate_array(sizeofdata+1);
      --count;
    }
    clock_gettime(CLOCK_REALTIME, &e);
    i=0;
    int frequency;
    while(i<dup_count){
      frequency = CM_PointEst(cm, items[i]);
      if(frequency >= (phi-epsilon)*dup_count){
        printf("%s : %d\n",items[i],frequency);
      }
    ++i;
  }
    printf("%f milliseconds\n",diff(&s,&e));
    printf("%d\n",CM_Size(cm));
    for(i=0;i<dup_count;i++)
    {
      free(items[i]);
    }
    free(items);
    CM_Destroy(cm);
  




  }

  return 0;	
}
