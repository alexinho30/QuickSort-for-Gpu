#include "../include/quicksort_cpu_version.h"
#include<stdio.h>

void quicksort(float* vec, const int start, const int end){
    if(start >= end){
        return ; 
    }

    int lt = start, gt = end ; 

    float pivot = partion(vec, start, end, &lt, &gt) ; 
    
    quicksort(vec, start, lt - 1) ;
    quicksort(vec, gt + 1, end) ;  
}

void swap(float* a, float* b){
    float tmp = *a ;  
    *a = *b ; 
    *b = tmp ; 
}

float partion(float *vec, const int start, const int end, int* lt, int* gt){
    const float pivot = vec[start] ; 
    int i = start + 1;

    while(i <= *gt){
        if(vec[i] < pivot){
            swap(&vec[(*lt)++], &vec[i]) ; 
            i++ ; 
        }
        else if(vec[i] > pivot){
            swap(&vec[(*gt)--], &vec[i]) ; 
        }
        else{
            i++ ; 
        }
    }
    
    return pivot ; 
}