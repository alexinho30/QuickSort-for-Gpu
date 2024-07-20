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

double random_uniform_zero_one(){
    return  ((double)(rand()) + 1. )/( (double)(RAND_MAX) + 1. );
}

int binomial_distribution_value(const int n, const int p){

    int binom_value = 0 ; 

    for(int i = 0 ; i < n ; i++){
        int r = random_uniform_zero_one() ; 
        if(r < p){
            binom_value++ ; 
        }
    }

    return binom_value ; 
}
