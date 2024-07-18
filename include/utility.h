#ifndef UTILITY_H
#define UTILITY_H

#include <stdio.h>
#include <stdlib.h>
#include<stdbool.h>

int log_b2(int val);

void handle_error(const char *err) ;

void scan(int* vec, const int nels) ;

void init_array(int* vec, const int nels) ; 

void print_vec(const int*vec, const int sstart, const int send) ;

int dummy_median(int* vec, const int sstart, const int send) ; 

void copy_vec(const int* vec, int* vec_copy, const int nels) ; 

void bubble_sort(int* vec, const int nels) ; 

bool check_result(const int* vec, const int* vec_copy, const int nels) ; 

#endif