#include "../include/utility.h"
#include <stdbool.h>

int log_b2(int val){
    int cont = 0 ; 
    while(val > 1){
        val = val/2 ; 
        cont++ ; 
    }
    return cont ; 
}

void handle_error(const char *err) {
	fprintf(stderr, "%s\n", err);
	exit(EXIT_FAILURE);
}

void init_array(int *vec, const int nels){
	srand(21);

	for (int i = 0; i < nels; i++){
		vec[i] = rand()%1000000;
	}
}

void scan(int* vec, const int nels){
	for(int i = 1 ; i < nels ; i++){
		vec[i] += vec[i-1] ;  
	}
}

void print_vec(const int*vec, const int sstart, const int send){
	for(int i = sstart ; i <=send ; i++){
		printf("%d ", vec[i]) ; 
	}
	printf("\n") ; 
}

void copy_vec(const int* vec, int* vec_copy, const int nels){
	for(int i = 0 ; i < nels ; i++){
		vec_copy[i] = vec[i] ; 
	}
}

void copy_vec_(const int* vec, int* vec_copy, const int sstart, const int nels){
	for(int i = 0 ; i < nels ; i++){
		vec_copy[i] = vec[i + sstart] ; 
	}
}

void bubble_sort(int*vec, const int nels){
	for(int i = 0 ; i < nels ; i++){
		for(int j = 0 ; j < nels - 1; j++){
			if(vec[j] > vec[j + 1]){
				int tmp = vec[j] ; 
				vec[j] = vec[j + 1] ; 
				vec[j + 1] = tmp ; 
			}
		}
	}
}

void check_result(const int* vec, const int* vec_copy, const int nels){
	for(int i = 0 ; i < nels ; i++){
		if(vec[i] != vec_copy[i]){
			fprintf(stderr, "error vec[%d] = %d different from expected[%d] = %d\n", i, vec[i], i, vec_copy[i]) ;
			exit(EXIT_FAILURE) ;   
		}
	}
}