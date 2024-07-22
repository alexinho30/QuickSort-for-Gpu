#include "../include/random_numbers.h"

int log_b2(int val){
    int cont = 0 ; 
    int tmp_val = val ; 
    while(tmp_val >= 1){
        tmp_val = tmp_val/2 ; 
        cont++ ; 
    }
    return cont ; 
}

int random_uniform_value(const int start, const int end){
    const int n_range = end - start + 1 ;
    const int n_range_bit = (int)log_b2(n_range) ; 

    int random_value = rand() ;
    int random_uniform_value = 0 ;  

    for(int i = 0 ; i < n_range_bit ; i++){
        random_uniform_value <<= 1 ; 
        random_uniform_value |= random_value & 1 ; 
        random_value >>= 1 ; 
        if(random_uniform_value >= n_range){
            random_uniform_value >>= 1 ; 
            break ; 
        }
    }

    return random_uniform_value + start ; 
}

float random_uniform_zero_one(){
    return  ((double)(rand()) + 1. )/( (double)(RAND_MAX) + 1. );
}

int binomial_distribution_value(const int n, const float p){

    int binom_value = 0 ; 

    for(int i = 0 ; i < n ; i++){
        int r = random_uniform_zero_one() ; 
        if(r < p){
            binom_value++ ; 
        }
    }

    return binom_value ; 
}

float normal_value(float mu, float sigma){
    double x1=random_uniform_zero_one();
    double x2=random_uniform_zero_one();
  return cos(2*3.14*x2)*sqrt(-2.*log(x1));
}

void normal_distr_arr(float* arr, const int nels, float mu, float sigma){
    for(int i = 0 ; i < nels ; i++){
        arr[i] = normal_value(mu, sigma) ; 
    }
}

void binom_distr_arr(float* arr, const int nels, const int n, const float p){
    for(int i = 0 ; i < nels ; i++){
        arr[i] = binomial_distribution_value(n, p) ; 
    }
}

void unif_distr_arr(float*arr, const int nels, const int start, const int end){
    for(int i = 0 ; i < nels ; i++){
        arr[i] = random_uniform_zero_one()*(end-start+1) ; 
    }
}
