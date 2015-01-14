#ifndef DEFINES_H_
#define DEFINES_H_

//#include "globals.h"
///#include "Utils/configReader.h"

//using namespace gcKons;

//Rootset size
#define ROOTSET_SIZE    10

//number of threads
#define NUM_THREADS	10

/* Ratio defining how often allocations are performed in
   comparison to reference operations (in %)*/
#define RATIO_ALLOC_SET 75


//the maximum number of child objects in an object
#define MAX_POINTERS    10

//the minimum/maximum payload size of an object
#define MIN_PAYLOAD     1
#define MAX_PAYLOAD     64

// the probability that a reference operation will create an escaped object (in %)
#define ESCAPE_PROBABILITY 5

/*the probablility that if an escaping is happening,
 * the target thread will be the partner thread (in %)*/
#define ESPACE_TO_PARTNER 90

/*The probability of being a root delete line in the trace file*/
#define ROOT_DELETE_PROBABILITY 10

//quite a vague metric, but it helps.
/*The age of a thread when it becomes immortal (in Trace File Lines)*/
#define ROOT_IMMORTALITY_AGE 1000000

#endif