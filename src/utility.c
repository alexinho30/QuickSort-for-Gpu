#include "../include/utility.h"
#include <stdbool.h>
#include "../include/boiler.h"

int log_b2(int val){
    int cont = 0 ; 
    while(val > 1){
        val = val/2 ; 
        cont++ ; 
    }
    return cont ; 
}

void handle_error(const char *err) {
	perror(err) ; 
	exit(EXIT_FAILURE);
}

void init_array(int *vec, const int nels){
	srand(21);

	for (int i = 0; i < nels; i++){
		vec[i] = rand()%1000000000;
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

void copy_vec(const int* vec, int* vec_copy, const int start, const int end){
	for(int i = start ; i <= end ; i++){
		vec_copy[i] = vec[i] ; 
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

void bench_mark(times* t, const int num_iteration, const sequences_info* s, const int lws ){

	time_t current_time ;  
	time(&current_time) ; 
	struct tm* time_info = localtime(&current_time) ; 
	char file_name[BUFSIZE] ; 
	snprintf(file_name, BUFSIZE, "benchMarking%s.txt", asctime(time_info)) ; 

	FILE*f = fopen(file_name, "a+") ; 

	if(!f){
		handle_error("error on opening benchmarking file\n") ; 
	}

	for(int iteration = 0 ; iteration < num_iteration ; iteration++){
		fprintf(f, "--------\n") ; 
		fprintf(f, "current_nels : %d current_nwg : %d  iteration : %d\n", s[iteration].current_nels, s[iteration].current_nwg, iteration) ; 

		fprintf(f,"split elements evt : %lu ns\t %.4g GE/s\t %.4g GB/s\n", 
				t[iteration].split_elements_time,  (3*s[iteration].current_nels + s[iteration].current_nwg)/(double)t[iteration].split_elements_time, 
				sizeof(cl_int)*(3*s[iteration].current_nels + s[iteration].current_nwg)/(double)t[iteration].split_elements_time) ;

		fprintf(f, "partial scan evt : %lu ns\t %.4g GE/s\t %.4g GB/s\n", 
				t[iteration].partial_scan_time, (4*s[iteration].current_nels + s[iteration].current_nwg)/(double)t[iteration].partial_scan_time, 
				sizeof(cl_int)*(4*s[iteration].current_nels +s[iteration].current_nwg)/(double)t[iteration].partial_scan_time) ;

		fprintf(f, "scan tails evt : %lu ns\t %.4g GE/s\t %.4g GB/s\n", 
				t[iteration].scan_tails_time, (4*s[iteration].current_nwg)/(double)t[iteration].scan_tails_time, 
				sizeof(cl_int)*(4*s[iteration].current_nwg)/(double)t[iteration].scan_tails_time) ;

		fprintf(f, "scan update evt : %lu ns\t %.4g GE/s\t %.4g GB/s\n", 
			t[iteration].scan_update_time, 2*(s[iteration].current_nels - lws)/(double)t[iteration].scan_update_time, 
			sizeof(cl_int)*2*(s[iteration].current_nels - lws)/(double)t[iteration].scan_update_time) ;

		fprintf(f, "partition evt : %lu ns\t %.4g GE/s\t %.4g GB/s\n", 
			t[iteration].partition_time, 2*s[iteration].current_nels /(double)t[iteration].partition_time, 
			sizeof(cl_int)*2*s[iteration].current_nels/(double)t[iteration].partition_time) ;
		
		fprintf(f, "partition copy evt : %lu ns\t %.4g GE/s\t %.4g GB/s\n", 
			t[iteration].partition_copy_time, 2*s[iteration].current_nels/(double)t[iteration].partition_copy_time, 
			sizeof(cl_int)*2*s[iteration].current_nels/(double)t[iteration].partition_copy_time) ;
	}

	if(fclose(f) < 0){
		handle_error("error on closing benchmarking file\n") ; 
	}

}