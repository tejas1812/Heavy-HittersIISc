// lclazy.h -- header file for Lazy Lossy Counting
// see Manku & Motwani, VLDB 2002 for details
// implementation by Graham Cormode, 2002,2003, 2005

//#define LCLitem_t uint64_t
//#define LCLitem_t uint32_t
#define LCLitem_t unsigned int

typedef struct lclcounter
{
  LCLitem_t item;
  int hash;
  int count;
  int delta;
  int prev,next; // pointers in linked list for hashtable
} LCLCounter;

typedef struct LCL_type
{
  LCLCounter *counters;
  int n;
  int * hashtable;
  int hasha, hashb, hashsize;
  int size;
} LCL_type;

extern LCL_type * LCL_Init(float);
extern void LCL_Destroy(LCL_type *);
extern int LCL_Update(LCL_type *, LCLitem_t, int);
extern int LCL_Size(LCL_type *);
extern int LCL_PointEst(LCL_type *, LCLitem_t);
extern int LCL_PointErr(LCL_type *, LCLitem_t);
extern LCLitem_t * LCL_Output(LCL_type *,int);
