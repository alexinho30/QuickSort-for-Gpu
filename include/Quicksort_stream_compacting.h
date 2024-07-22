#ifndef QUICKSORT_STREAM_COMPACTING_H
#define QUICKSORT_STREAM_COMPACTING_H

#include<math.h>
#include <stdbool.h>
#include "../include/quicksort_cpu_version.h"
#define MAX_NUM_SEQ 300000
#define TEST 1

#include<stdio.h>
#include<unistd.h>
#include "boiler.h"
#include "queue.h"
#include "utility.h"

float* quickSortGpu(const float* vec, const int nels, const int lws, const int nwg_cu, cl_resources* resources, bool test_mode) ; 

#endif 