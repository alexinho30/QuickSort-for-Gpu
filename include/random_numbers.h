#ifndef RANDOM_NUMBERS_H
#define RANDOM_NUMBERS_H

#include<math.h> 
#include<stdlib.h>
#include "boiler.h"
#include "utility.h"

typedef struct{
    cl_mem out ; 
    cl_mem rand_values ; 
}device_memory ;

int log_b2(int val) ; 

int random_uniform_value(const int start, const int end) ; 

void distr_arr(cl_resources* resources, float* vec, const int nels, float mu, float sigma, const int lws, bool norm) ; 
 

#endif 