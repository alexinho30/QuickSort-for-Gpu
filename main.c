#include "include/Quicksort_stream_compacting.h"
#include "include/boiler.h"
#include "include/quicksort_cpu_version.h"
#include "include/random_numbers.h"
#include <getopt.h>

int main(int argc, char* const* argv){

	char in_file[BUFSIZE], out_file[BUFSIZE] ; 
	int opt, d = 1, nels = 10000, seed = 21 ; 
	float p1, p2 ;  
	bool test_mode ; 

	while((opt = getopt(argc, argv, ":i:o:tn:s:d:p:r:")) != -1){
		switch(opt){
			case 'i':
				strcpy(in_file, optarg) ; 
				break ;

			case 'o':
				strcpy(out_file, optarg) ; 
				break ; 
			
			case 't':
				test_mode = true ; 
				break ;

			case 's':
				seed = atoi(optarg) ;
				if(seed < 1){
					fprintf(stderr, "seed must be greater than 0\n") ; 
					exit(EXIT_FAILURE) ; 
				}
				break ; 
			
			case 'd':
				d = atoi(optarg) ;
				if(d <1 || d > 4){
					fprintf(stderr, "d must be  between 1 and 4\n") ; 
					exit(EXIT_FAILURE) ; 
				}
				break ; 

			case 'p':
				p1 = atoi(optarg) ; 
				break ; 
			
			case 'r':
				p2 = atoi(optarg) ; 
				break ;

			case 'n':
				nels = atoi(optarg) ; 
				break ;  
			
			case '?':
				fprintf(stderr, "%s unknown arg %c\n", argv[0], optopt) ; 
				exit(EXIT_FAILURE) ; 
				break ; 
			
			case ':':
				fprintf(stderr, "%s missing arg %c\n", argv[0], optopt) ; 
				exit(EXIT_FAILURE) ; 
				break ; 
		}
	}

	cl_int err , ncu = 0, nwg_cu = 4; 
	size_t lws = 512 ;  

	cl_resources resource ; 
	create_resources(&resource, "quickSort.ocl") ; 


	err = clGetDeviceInfo(resource.d, CL_DEVICE_MAX_COMPUTE_UNITS, sizeof(ncu), &ncu, NULL);
	cl_uint nwg = ncu*nwg_cu;
	printf("Nwg: %d*%d = %d\n", nwg_cu, ncu, nwg);

	float* vec ; 

	if(test_mode){ 

		srand(seed) ; 
		vec = calloc(nels, sizeof(float)) ; 
		
		switch(d){
			case 1:
				normal_distr_arr(vec, nels, p1, p2) ;
				break ; 

			case 2:
				binom_distr_arr(vec, nels, p1, p2) ;
				break ; 

			case 3:
				unif_distr_arr(vec, nels, p1, p2) ;
				break ;    
		}

		quickSortGpu(vec, nels, lws, nwg, &resource, test_mode) ; 
	}
	else{

		vec = read_array_from_file( &nels, in_file) ;
		float*out_vec = quickSortGpu(vec, nels, lws, nwg, &resource, test_mode) ;
		write_array_on_file(out_vec, nels, out_file) ; 
 
		free(out_vec) ; 
	}

	free(vec) ; 

	exit(EXIT_SUCCESS) ; 
}