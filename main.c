#include<stdio.h>
#include<stdlib.h>
#include<time.h>

#include "include/Quicksort_stream_compacting.h"
#include "include/boiler.h"
#include "include/quicksort_cpu_version.h"

/*int main(int argc, char **argv) {
	time_t start, end ; 
	double cpu_time_used ; 
	start = clock() ;
	srand(20) ; 
	cl_platform_id p = select_platform(); 
	cl_device_id d = select_device(p); 
	cl_context ctx = create_context(p, d); 
	cl_command_queue que = create_queue(ctx, d);
	cl_program prog = create_program("sort.ocl", ctx, d);

	if(argc != 6){
		handle_error("./main <nels> <min_seq_len_phase_1> <min_seq_len_phase_2> <lws_partion_phase_1> <lws_final_partion>") ; 
	}

	const int nels = atoi(argv[1]) ;
	const int min_seq_len_phase_one = atoi(argv[2]) ; 
	const int min_seq_len_phase_two = atoi(argv[3]) ;  
	const int lws_partion_phase_one = atoi(argv[4]) ;
	const int lws_final_partion = atoi(argv[5]) ;

	if(nels <= 0 || lws_partion_phase_one <= 0 || lws_final_partion <= 0){
		handle_error("nels and nwg must be greater than zero") ; 
	}

	const int nwg_partion_phase_one = nels/lws_partion_phase_one ;
	const int nwg_final_partion = nels/lws_final_partion ; 

	if(lws_partion_phase_one*nwg_partion_phase_one != nels){
		handle_error("nels must be a multiple of lws") ; 
	}

	if(lws_final_partion*nwg_final_partion != nels){
		handle_error("nels must be a multiple of lws") ; 
	}
	
	cl_int err;
	size_t memsize = nels * sizeof(cl_int);
	cl_kernel partition_phs_1, partition_phs_2, final_partion;

	final_partion = clCreateKernel(prog, "partion_final_v1", &err);	
	ocl_check(err, "create kernel partion_final");
	partition_phs_1 = clCreateKernel(prog, "partition_first_phase_v1", &err);	
	ocl_check(err, "create kernel partion_v1");
	partition_phs_2 = clCreateKernel(prog, "partition_second_phase_v1", &err);	
	ocl_check(err, "create kernel partion_v2");

	int *vec = calloc(nels, sizeof(int));
	init_array(vec, nels);
	print_vec(vec, 0, nels -1) ; 
	int*vec_copy =  calloc(nels, sizeof(int)) ;
	copy_vec(vec, vec_copy, nels) ;  
	quicksort(vec_copy, 0, nels - 1) ; 
	printf("cpu version quicksort executed\n") ; 

	cl_mem d_buf[2];
	d_buf[0] = clCreateBuffer(ctx, CL_MEM_READ_WRITE | CL_MEM_COPY_HOST_PTR, memsize, vec, &err); 
	ocl_check(err, "create buffer d_in");
	d_buf[1] = clCreateBuffer(ctx, CL_MEM_READ_WRITE | CL_MEM_ALLOC_HOST_PTR, memsize, NULL, &err); 
	ocl_check(err, "create buffer d_out");
	cl_mem lt = clCreateBuffer(ctx, CL_MEM_READ_WRITE | CL_MEM_ALLOC_HOST_PTR , (nwg_partion_phase_one+1)*sizeof(cl_int), NULL, &err);
	ocl_check(err, "create buffer lt");
	cl_mem gt = clCreateBuffer(ctx, CL_MEM_READ_WRITE | CL_MEM_ALLOC_HOST_PTR, (nwg_partion_phase_one+1)*sizeof(cl_int), NULL, &err);
	ocl_check(err, "create buffer gt");
	cl_mem pivot_position = clCreateBuffer(ctx, CL_MEM_READ_WRITE, sizeof(cl_int), NULL, &err);
	ocl_check(err, "create buffer pivot position");

	output_first_phase* out = quickSort_cpu_phase_one_v1(que, partition_phs_1, partition_phs_2, d_buf, lt, gt, vec, nels, pivot_position, lws_partion_phase_one, min_seq_len_phase_one) ;
 
	quickSort_cpu_phase_two_v1(ctx, que, final_partion, d_buf[out->buf_out_index], d_buf[(out->buf_out_index + 1)%2], vec_copy, out->sstart, out->send, out->number_of_sequences, nels, lws_final_partion, min_seq_len_phase_two) ; 

	cl_event read_evt ; 
	int* current_vec= clEnqueueMapBuffer(que, d_buf[(out->buf_out_index+ 1)%2], CL_TRUE, CL_MAP_READ, 
	0, memsize, 0, NULL, &read_evt, &err) ; 
	ocl_check(err, "map object error") ; 

	cl_event unmap_evt_;
	err = clEnqueueUnmapMemObject(que, d_buf[(out->buf_out_index + 1)%2], current_vec,
				1, &read_evt, &unmap_evt_) ;
	ocl_check(err, "unmap d_out");
	
	clReleaseMemObject(d_buf[0]) ;
	clReleaseMemObject(d_buf[1]) ;
	clReleaseMemObject(gt) ;
	clReleaseMemObject(lt) ;
	clReleaseMemObject(pivot_position) ;
	clReleaseKernel(partition_phs_1) ;
	clReleaseKernel(partition_phs_2) ;
	clReleaseKernel(final_partion) ;

	clReleaseCommandQueue(que) ; 
	clReleaseContext(ctx) ; 
	clReleaseDevice(d) ; 
	end = clock() ; 
	cpu_time_used = ((double)(end - start))/CLOCKS_PER_SEC ; 
	printf("time spent :  %f\n", cpu_time_used) ; 

	return 0 ;
}
*/
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
	quickSortGpu(vec, nels, lws, nwg_cu, &resource) ;

	return 0 ;
}