#include "../include/quicksort_cpu_version.h"
#include<stdio.h>

void quicksort(int* vec, const int start, const int end){
    if(start >= end){
        return ; 
    }

    int p = partion(vec, start, end) ; 
    quicksort(vec, start, p - 1) ;
    quicksort(vec, p, end) ;  
}

void swap(int* a, int* b){
    int tmp = *a ;  
    *a = *b ; 
    *b = tmp ; 
}

int partion(int*vec, const int start, const int end){
    const int pivot = vec[end] ; 

    int j = start - 1 ; 

    for(int i = start ; i < end ; i++){
        if(vec[i] <= pivot){
            swap(&vec[++j], &vec[i]) ;  
        }
    }

    swap(&vec[++j], &vec[end]) ; 

    return j ; 
}