#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<math.h>

#include "include/Quicksort_stream_compacting.h"
#include "include/boiler.h"
#include "include/quicksort_cpu_version.h"
#include "include/random_numbers.h"

int main(int argc, char **argv) {
	if(argc != 4){
		handle_error("./main <nels> <lws> <nwg_cu>") ; 
	}

	const int nels = atoi(argv[1]) ;
	int lws = atoi(argv[2]) ;
	const int nwg_cu = atoi(argv[3]) ;

	if(nels <= 0 || nwg_cu <= 0){
		handle_error("nels and nwg must be greater than zero") ; 
	}

	int log_lws = log_b2(lws) ; 
	if((int)(1 <<log_lws) != lws){
		lws-- ; 
		for(int i =1 ; i <=sizeof(int)*8 ; i*=2) lws |= lws >> i ; 
		lws++; 
		printf("lws must be a power of two , new lws : %d\n", lws) ; 
	}

	if(lws > nels){
		fprintf(stderr, "lws must be lower or equal than nels\n") ;
		exit(EXIT_FAILURE) ;  
	}

	int *vec = calloc(nels, sizeof(int));
	init_array(vec, nels);

	cl_resources resource ; 
	create_resources(&resource, "quickSort.ocl") ; 

	for(int i = 0 ; i < 100; i++){
		printf("%d ", random_uniform_value(0, 100)) ; 
	}
	printf("\n") ;
	printf("log :%d\n", log_b2(33)) ;  
	quickSortGpu(vec, nels, lws, nwg_cu, &resource) ;

	return 0 ;
}