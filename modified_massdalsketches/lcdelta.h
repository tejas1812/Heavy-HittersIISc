// lossycount.h -- header file for Lossy Counting
// see Manku & Motwani, VLDB 2002 for details
// implementation by Graham Cormode, 2002,2003

#ifndef LCDELTA_H
#define LCDELTA_H
int streamsize;
int stream_size;
int stream_sze;
typedef struct lcdcounter
{
  char* item;
  int count;
  int delta;
} LCDCounter;

typedef struct LCD_type
{
  LCDCounter *bucket;
  LCDCounter *holder;
  LCDCounter *newcount;
  int buckets;
  int holdersize;
  int maxholder;
  int window;
  int epoch;
  float epsilon;
} LCD_type;

extern LCD_type * LCD_Init(float);
extern void LCD_Destroy(LCD_type *);
extern void LCD_Update(LCD_type *, char*);
extern int LCD_Size(LCD_type *,int);
extern int LCD_PointEst(LCD_type *, char*);
// extern unsigned int * LCD_Output(LCD_type *,int);
extern char** LCD_Report(LCD_type*, float, int);
#endif
