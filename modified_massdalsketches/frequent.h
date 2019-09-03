//frequent.h -- simple frequent items routine
// see Misra&Gries 1982, Demaine et al 2002, Karp et al 2003
// implemented by Graham Cormode, 2002,2003
//global variable to keep track of the number of elements in the stream so far.
#ifndef includeonce
  #define includeonce =1
int stream_size;
typedef struct itemlist ITEMLIST;
typedef struct group GROUP;

struct group 
{
  int diff;
  ITEMLIST *items;
  GROUP *previousg, *nextg;
};

struct itemlist 
{
  char* item;
  GROUP *parentg;
  ITEMLIST *previousi, *nexti;
  ITEMLIST *nexting, *previousing;
  
};

typedef struct freq_type{

  ITEMLIST **hashtable;
  GROUP *groups;
  int k;
  int tblsz;
  char* a;
  char* b;
} freq_type;


extern freq_type * Freq_Init(float,int);
extern void Freq_Destroy(freq_type *);
extern void Freq_Update(freq_type *, char*, long long);
extern int Freq_Size(freq_type *);
extern unsigned int * Freq_Output(freq_type *,int);
extern void ShowGroups_gt_than_phiN(freq_type * freq, float phi);
extern unsigned int * Freq_Output_gt_than_phiN(freq_type * freq, float phi);
#endif
