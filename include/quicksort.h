
#ifndef QUICKSORT_H
#define QUICKSORT_H


#define CL_TARGET_OPENCL_VERSION 120

#include<stdio.h>
#include<unistd.h>
#include "boiler.h"
#include "queue.h"
#include "utility.h"

cl_event partition_phase_1_v1(cl_command_queue q, cl_kernel partition_1,
		cl_mem d_in, cl_mem lt, cl_mem gt, cl_int nels, cl_int lws_, const cl_int sstart, const cl_int send, const cl_int pivot);


cl_event partition_phase_1_v2(cl_command_queue q, cl_kernel partition_1,
		cl_mem d_in, cl_mem lt, cl_mem gt, cl_int nels, cl_int lws_, const cl_int sstart, const cl_int send, const cl_int pivot);

cl_event partition_phase_2_v1(cl_command_queue q, cl_kernel partition_1,
		cl_mem d_in, cl_mem d_out, cl_mem lt, cl_mem gt, cl_int current_nels, cl_int nels,
		 cl_int lws_, cl_int pivot, cl_int sstart, cl_int send, cl_mem pivot_position);

output_first_phase* quickSort_cpu_phase_one_v1(cl_command_queue que, cl_kernel partion_phs_1, cl_kernel partion_phs_2, 
cl_mem* d_buf, cl_mem lt, cl_mem gt,int* vec, const int nels, cl_mem pivot_position, 
const int lws, const int max_seq_len);

cl_event final_partion_v1(cl_command_queue q, cl_kernel final_partion, cl_mem d_in, cl_mem d_out, cl_int nels, cl_int lws_, cl_mem sstart,
	cl_mem send, const int number_of_sequences, const int min_seq_len) ; 

void quickSort_cpu_phase_two_v1(cl_context ctx, cl_command_queue que, cl_kernel final_partioning, cl_mem d_in, cl_mem d_out, 
		const int* vec_copy, const int* sstart, const int* send, const int number_of_sequences, const int nels, const int lws, const int min_seq_len) ; 
	
#endif