#include <stdint.h>
#include <string.h>

#include <stdio.h>
#include <stdlib.h>

#ifdef DATASET
#include <time.h>
#include <dirent.h>
#endif

#ifdef HISTOGRAM
#include <math.h> 
#endif

#ifdef SVM
#include <math.h> 
#endif

#define SUCCESS 0
#define ANZEIGEN
// #define DATENSATZ_ANZEIGEN
// #define HISTOGRAM_ANZEIGEN
#define SVM_ANZEIGEN
#define NORMALIZE_HOSTOGRAM

#define NICHT_GEFUNDEN -1
#define EQAUL 0 

#define _type uint8_t
#define __type float


#define delete_p(x) do{(x)->destroy(x);free_p(x);}while(0)
#define delete(x) (x).destroy(&(x))
#define free_p(x) do{free(x);(x)=NULL;}while(0)