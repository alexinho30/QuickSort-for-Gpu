#include "../include/quicksort_cpu_version.h"
#include<stdio.h>

void swap(float* a, float* b){
    float tmp = *a ;  
    *a = *b ; 
    *b = tmp ; 
}

void partition_median(float *vec, const int start, const int end, int* lt, int* gt){
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
}


float median(float* vec, const int start, const int end, const int med){
    if(start == end){
        return vec[start] ; 
    }

    int lt = start ; 
    int gt = end ; 
    partition_median(vec, start, end, &lt, &gt) ; 
    
    for(int j = lt ; j <= gt ; j++){
        if(j == med) return vec[j] ; 
    }

    if(med < lt) return median(vec, start, lt - 1, med) ; 
    else return median(vec, gt + 1, end, med) ; 
}


void partition_cpu(float *vec, const int start, const int end, int* lt, int* gt){
    const int nels = end - start + 1 ; 
    float pivot = 0 ; 

    if(nels & 1) pivot = median(vec, start, end, (nels/2) +start) ; 
    else pivot = median(vec, start, end, (nels/2 -1) + start) ; 

    int i = start ;

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
}

void quicksort(float* vec, const int start, const int end){
    if(start >= end){
        return ; 
    }

    int lt = start, gt = end ; 

    partition_cpu(vec, start, end, &lt, &gt) ; 
    
    quicksort(vec, start, lt - 1) ;
    quicksort(vec, gt + 1, end) ;  
}
