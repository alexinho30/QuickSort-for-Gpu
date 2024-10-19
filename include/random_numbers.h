#ifndef RANDOM_NUMBERS_H
#define RANDOM_NUMBERS_H

#include<math.h> 
#include<stdlib.h>

int log_b2(int val) ; 

int random_uniform_value(const int start, const int end) ; 

void normal_distr_arr(float* arr, const int nels, float mu, float sigma) ; 

void unif_distr_arr(float*arr, const int nels, const int start, const int end) ; 

#endif 