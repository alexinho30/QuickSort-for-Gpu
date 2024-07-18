#ifndef QUICKSORT_STREAM_COMPACTING_H
#define QUICKSORT_STREAM_COMPACTING_H


#include<stdio.h>
#include<unistd.h>
#include "boiler.h"
#include "queue.h"
#include "utility.h"

void quickSortGpu(const int* vec, const int nels, const int lws, const int nwg_cu, cl_resources* resources) ; 

#endif 