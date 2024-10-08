#ifndef QUICKSORT_GPU_H
#define QUICKSORT_GPU_H

#include<math.h>
#include <stdbool.h>
#include "../include/quicksort_cpu_version.h"
#include "../include/random_numbers.h"

#define MAX_NUM_SEQ 30000000

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
    cl_kernel quicksort_lmem ;   
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
    cl_mem sstart_arr ; 
    cl_mem send_arr ; 
}device_memeory ; 

float* quickSortGpu(const float* vec, const int nels, const int lws, const int nwg_cu, cl_resources* resources, bool test_mode) ; 

#endif 

