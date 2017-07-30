#include <sys/time.h>

extern void StartTheClock();
extern long StopTheClock();
extern long MedSelect(long, long, long[]);
extern long LMedSelect(int, int, long[]);
extern long long LLMedSelect(int, int, long long[]);
extern double DMedSelect(int, int, double[]);
extern void CheckMemory(void *);
