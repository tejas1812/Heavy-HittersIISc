#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "SketchFrequent.h"
#include "SketchLC.h"
#include "../modified_massdalsketches/countmin.h"
#include "../modified_massdalsketches/frequent.h"
#include "../modified_massdalsketches/lcdelta.h"
#include "../massdalsketches/prng.h"
#include "../Hashing/hashing.h"
#include <time.h>

int test = 4;

double diff(struct timespec* s,struct timespec* e)
{
  double temp;
  temp = (e->tv_sec - s->tv_sec)*1000;
  temp = temp + (e->tv_nsec - s->tv_nsec)*0.000001;
  return temp;
}

int main(int argc, char **argv) 
{
  struct timespec s,e;
  char *line =allocate_array(sizeofdata+1);
  int count;
  scanf("%d",&count);

  SketchFrequent_type* sfr=SketchFrequent_Init(0.01,0.0001,0.1);
  freq_type* ft = Freq_Init(0.0001, sizeofdata);
  LCD_type* lc = LCD_Init(0.0001);
  SketchLC_type* slc = SketchLC_Init(0.01, 0.0001, 0.1);
  clock_gettime(CLOCK_REALTIME,&s);

  
  
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
    ShowGroups_gt_than_phiN(ft, 0.01);
    Freq_Destroy(ft);
  }
  else if(test==3){
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
    LCD_Report(lc, 0.01,sizeofdata);
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
    clock_gettime(CLOCK_REALTIME, &e);
    printf("%f milliseconds\n",diff(&s,&e));
    printf("%d\n",SketchLC_Size(slc,sizeofdata));
    SketchLC_Report(slc);
    SketchLC_Destroy(slc);




  }
  return 0;	
}
