#ifndef QUICKSORT_STREAM_COMPACTING_H
#define QUICKSORT_STREAM_COMPACTING_H


#define CL_TARGET_OPENCL_VERSION 120

#include<stdio.h>
#include<unistd.h>
#include "boiler.h"
#include "queue.h"
#include "utility.h"

void quickSortGpu(const int* vec, const int nels, const int lws, const int nwg_cu) ; 

#endif 