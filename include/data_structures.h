#ifndef DATA_STRUCTURES_H
#define DATA_STRUCTURES_H

typedef struct __attribute__ ((packed)) {
	cl_int sstart ;
	cl_int send ;
	cl_int pivot_value ; 	
} sequence;

typedef struct{
	int sstart ; 
	int send ; 
}final_sequence ; 

typedef struct {
    sequence *arr ;
    int cur_size ;
    int tot_size ;
    int head ;
    int tail ; 
} Queue;

typedef struct {
	int buf_out_index ; 
	int* sstart;
	int* send ;  
	int number_of_sequences ; 
} output_first_phase ; 

#endif