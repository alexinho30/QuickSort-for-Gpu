#ifndef UTILITY_H
#define UTILITY_H

#include <stdio.h>
#include <stdlib.h>
#include<stdbool.h>

typedef struct{
    unsigned long split_elements_time ;
    unsigned long partial_scan_time ; 
    unsigned long scan_tails_time ; 
    unsigned long scan_update_time ;  
    unsigned long partition_time ;
    unsigned long partition_copy_time ;
}times ; 

typedef struct{
    int current_nels ; 
    int current_nwg ; 
}sequences_info ; 

int log_b2(int val);

void handle_error(const char *err) ;

void scan(int* vec, const int nels) ;

void init_array(int* vec, const int nels) ; 

void print_vec(const int*vec, const int sstart, const int send) ;

void copy_vec(const int* vec, int* vec_copy, const int start, const int end) ; 

void check_result(const int* vec, const int* vec_copy, const int nels) ; 

void bench_mark(times* t, const int iteration, const sequences_info* s, const int lws ) ; 

#endif