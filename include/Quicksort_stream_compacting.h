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

typedef struct{
    cl_kernel splitting_elements;
	cl_kernel scan_gpu ; 
	cl_kernel scan_update ;
	cl_kernel partitioning ;
	cl_kernel partitioning_copy ;   
}kernels ; 

typedef struct{
    cl_mem in ; 
    cl_mem buff_tmp ; 
    cl_mem lt ;
    cl_mem bit_map_sup ; 
    cl_mem bit_map_inf ; 
    cl_mem tails_inf ; 
    cl_mem tails_sup ;
    cl_mem gt ;  
}device_memeory ; 

float* quickSortGpu(const float* vec, const int nels, const int lws, const int nwg_cu, cl_resources* resources, bool test_mode) ; 

#endif 