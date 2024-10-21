#ifndef UTILITY_H
#define UTILITY_H

#include <stdio.h>
#include <stdlib.h>
#include<stdbool.h>
#include<time.h>

#include "../include/boiler.h"

typedef struct{
    double split_elements_time ;
    double partial_scan_time ; 
    double scan_tails_time ; 
    double scan_update_time ;  
    double partition_time ;
    double partition_copy_time ;
}times ; 

typedef struct{
    int current_nels ; 
    int current_nwg ; 
}sequences_info ; 

void handle_error(const char *err) ;

void scan(int* vec, const int nels) ;

void print_vec(const float*vec, const int sstart, const int send) ;

void copy_vec(const float* vec, float* vec_copy, const int start, const int end) ; 

void check_result(const float* vec, const float* vec_copy, const int nels) ; 

void bench_mark(times* t, const int iteration, const sequences_info* s, const int lws, const int seq_arr_dim ) ; 

void write_array_on_file(float* vec, const int nels, const char* const file_name) ; 

float* read_array_from_file(int* nels, const char* const file_name) ; 

#endif