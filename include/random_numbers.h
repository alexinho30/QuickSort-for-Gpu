#ifndef RANDOM_NUMBERS_H
#define RANDOM_NUMBERS_H

#include<math.h> 
#include<stdlib.h>

int log_b2(int val) ; 

void normal_distr_arr(float* arr, const int nels, float mu, float sigma) ; 

void binom_distr_arr(float* arr, const int nels, const int n, const float p) ;

void unif_distr_arr(float*arr, const int nels, const int start, const int end) ; 

#endif 