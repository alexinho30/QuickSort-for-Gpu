#include "../include/quicksort.h"


cl_event partition_phase_1_v1(cl_command_queue q, cl_kernel partition_1,
		cl_mem d_in, cl_mem lt, cl_mem gt, cl_int nels, cl_int lws_, const cl_int sstart, const cl_int send, const cl_int pivot) {
	
	size_t lws[] = {lws_} ; 
	size_t gws[] = {round_mul_up(nels, lws[0])} ;
	//printf(" phase one : lws: %ld gws: %ld nels : %d \n", lws[0], gws[0], nels) ; 
	cl_int err ; 
	cl_event evt_partion_phase_1 ; 

	err = clSetKernelArg(partition_1, 0, sizeof(cl_int), &nels) ;
	ocl_check(err, "set kernel nels ") ;

	err = clSetKernelArg(partition_1, 1, sizeof(d_in), &d_in) ;
	ocl_check(err, "set kernel d_in") ;

	err = clSetKernelArg(partition_1, 2, sizeof(pivot), &pivot) ;
	ocl_check(err, "set kernel pivot") ;

	err = clSetKernelArg(partition_1, 3, sizeof(lt), &lt) ;
	ocl_check(err, "set kernel lt") ;

	err = clSetKernelArg(partition_1, 4, sizeof(gt), &gt) ;
	ocl_check(err, "set kernel gt") ;

	err = clSetKernelArg(partition_1, 5, sizeof(sstart), &sstart) ;
	ocl_check(err, "set kernel gt") ;

	err = clSetKernelArg(partition_1, 6, sizeof(send), &send) ;
	ocl_check(err, "set kernel gt") ;

	err = clSetKernelArg(partition_1, 7, sizeof(cl_int), NULL) ;
	ocl_check(err, "set kernel gt") ;

	err = clSetKernelArg(partition_1, 8, sizeof(cl_int), NULL) ;
	ocl_check(err, "set kernel gt") ;

	err = clEnqueueNDRangeKernel(q, partition_1, 1, NULL, gws, lws, 0, NULL, &evt_partion_phase_1) ;
    ocl_check(err, "enqueue partion phase 1") ; 

    return evt_partion_phase_1;

}

cl_event partition_phase_1_v2(cl_command_queue q, cl_kernel partition_1,
		cl_mem d_in, cl_mem lt, cl_mem gt, cl_int nels, cl_int lws_, const cl_int sstart, const cl_int send, const cl_int pivot){
		
	const int nwg = nels/lws_ ; 
	size_t lws[] = {lws_} ; 
	size_t gws[] = {(nwg/2)*lws[0]} ;
	printf("partion pahse one --> nels : %d lws : %ld gws : %ld \n", nels, lws[0], gws[0]) ; 
	cl_int err ; 
	cl_event evt_partion_phase_1 ; 

	err = clSetKernelArg(partition_1, 0, sizeof(cl_int), &nels) ;
	ocl_check(err, "set kernel nels ") ;

	err = clSetKernelArg(partition_1, 1, sizeof(d_in), &d_in) ;
	ocl_check(err, "set kernel d_in") ;

	err = clSetKernelArg(partition_1, 2, sizeof(pivot), &pivot) ;
	ocl_check(err, "set kernel pivot") ;

	err = clSetKernelArg(partition_1, 3, sizeof(lt), &lt) ;
	ocl_check(err, "set kernel lt") ;

	err = clSetKernelArg(partition_1, 4, sizeof(gt), &gt) ;
	ocl_check(err, "set kernel gt") ;

	err = clSetKernelArg(partition_1, 5, sizeof(sstart), &sstart) ;
	ocl_check(err, "set kernel gt") ;

	err = clSetKernelArg(partition_1, 6, sizeof(send), &send) ;
	ocl_check(err, "set kernel gt") ;

	err = clSetKernelArg(partition_1, 7, sizeof(cl_int)*lws[0], NULL) ;
	ocl_check(err, "set kernel gt") ;

	err = clSetKernelArg(partition_1, 8, sizeof(cl_int)*lws[0], NULL) ;
	ocl_check(err, "set kernel gt") ;

	err = clEnqueueNDRangeKernel(q, partition_1, 1, NULL, gws, lws, 0, NULL, &evt_partion_phase_1) ;
    ocl_check(err, "enqueue partion phase 1") ; 

    return evt_partion_phase_1;
		}

cl_event partition_phase_2_v1(cl_command_queue q, cl_kernel partion_1,
		cl_mem d_in, cl_mem d_out, cl_mem lt, cl_mem gt, cl_int current_nels, cl_int nels,
		 cl_int lws_, cl_int pivot, cl_int sstart, cl_int send, cl_mem pivot_position) {
	
	size_t lws[] = {lws_} ; 
	size_t gws[] = {round_mul_up(nels, lws[0])} ;

	cl_int err ; 
	cl_event evt_partion_phase_2 ; 

	const int n_wg_to_order = (current_nels%lws[0] == 0) ? current_nels/lws[0] : (current_nels + lws[0])/lws[0]  ; 
	//printf("phase 2 - > lws: %ld gws: %ld n_wg_to_order : %d\n", lws[0], gws[0], n_wg_to_order) ; 

	err = clSetKernelArg(partion_1, 0, sizeof(d_in), &d_in) ;
	ocl_check(err, "set kernel d_in") ;

	err = clSetKernelArg(partion_1, 1, sizeof(d_out), &d_out) ;
	ocl_check(err, "set kernel d_in") ;

	err = clSetKernelArg(partion_1, 2, sizeof(pivot), &pivot) ;
	ocl_check(err, "set kernel pivot") ;

	err = clSetKernelArg(partion_1, 3, sizeof(lt), &lt) ;
	ocl_check(err, "set kernel lt") ;

	err = clSetKernelArg(partion_1, 4, sizeof(gt), &gt) ;
	ocl_check(err, "set kernel gt") ;

	err = clSetKernelArg(partion_1, 5, sizeof(sstart), &sstart) ;
	ocl_check(err, "set kernel sstart") ;

	err = clSetKernelArg(partion_1, 6, sizeof(send), &send) ;
	ocl_check(err, "set kernel send") ;

	err = clSetKernelArg(partion_1, 7, sizeof(cl_int), &nels) ;
	ocl_check(err, "set kernel nels") ;

	err = clSetKernelArg(partion_1, 8, sizeof(pivot_position), &pivot_position) ;
	ocl_check(err, "set kernel pivot position") ;

	err = clSetKernelArg(partion_1, 9, sizeof(cl_int), &n_wg_to_order) ;
	ocl_check(err, "set kernel nels") ;

	err = clSetKernelArg(partion_1, 10, sizeof(cl_int), NULL) ;
	ocl_check(err, "set kernel count_inf") ;

	err = clSetKernelArg(partion_1, 11, sizeof(cl_int), NULL) ;
	ocl_check(err, "set kernel count_sup") ;

	err = clEnqueueNDRangeKernel(q, partion_1, 1, NULL, gws, lws, 0, NULL, &evt_partion_phase_2) ;
    ocl_check(err, "enqueue partion phase 2 ") ; 

    return evt_partion_phase_2;

}

cl_event final_partion_v1(cl_command_queue q, cl_kernel final_partion, cl_mem d_in, cl_mem d_out, cl_int nels, cl_int lws_, cl_mem sstart,
		cl_mem send, const int number_of_sequences, const int min_seq_len){
		
		size_t lws[] = {lws_} ; 
		size_t gws[] = {(number_of_sequences + 1)*lws_, lws[0]} ;

		//printf("lws %ld gws : %ld number_of_sequences : %d\n", lws[0], gws[0], number_of_sequences + 1)  ; 

		cl_int err ; 
		cl_event evt_final_partion ; 

		err = clSetKernelArg(final_partion, 0, sizeof(d_in), &d_in) ;
		ocl_check(err, "set kernel d_in") ;

		err = clSetKernelArg(final_partion, 1, sizeof(sstart), &sstart) ;
		ocl_check(err, "set kernel sstart arr") ;

		err = clSetKernelArg(final_partion, 2, sizeof(send), &send) ;
		ocl_check(err, "set kernel send arr") ;

		err = clSetKernelArg(final_partion, 3, sizeof(nels), &nels) ;
		ocl_check(err, "set kernel nels") ;

		err = clSetKernelArg(final_partion, 4, sizeof(number_of_sequences), &number_of_sequences) ;
		ocl_check(err, "set kernel number of sequences") ;

		err = clSetKernelArg(final_partion, 5, sizeof(min_seq_len), &min_seq_len) ;
		ocl_check(err, "set kernel nels") ;

		err = clSetKernelArg(final_partion, 6, sizeof(cl_int)*5000, NULL) ;
		ocl_check(err, "set kernel stack sstart") ;

		err = clSetKernelArg(final_partion, 7, sizeof(cl_int)*5000, NULL) ;
		ocl_check(err, "set kernel stack send") ;

		err = clSetKernelArg(final_partion, 8, sizeof(cl_int), NULL) ;
		ocl_check(err, "set kernel nels") ;

		err = clSetKernelArg(final_partion, 9, sizeof(cl_int)*(lws[0] + 1), NULL) ;
		ocl_check(err, "set kernel lt") ;

		err = clSetKernelArg(final_partion, 10, sizeof(cl_int)*(lws[0] + 1), NULL) ;
		ocl_check(err, "set kernel gt") ;

		err = clSetKernelArg(final_partion, 11, sizeof(cl_int), NULL) ;
		ocl_check(err, "set kernel current sstart") ;

		err = clSetKernelArg(final_partion, 12, sizeof(cl_int), NULL) ;
		ocl_check(err, "set kernel current ssend") ;

		err = clSetKernelArg(final_partion, 13, sizeof(cl_int), NULL) ;
		ocl_check(err, "set kernel current pivot") ;

		err = clSetKernelArg(final_partion, 14, sizeof(cl_int)*lws[0], NULL) ;
		ocl_check(err, "set kernel pivot elements") ;

		err = clEnqueueNDRangeKernel(q, final_partion, 1, NULL, gws, lws, 0, NULL, &evt_final_partion) ;
    	ocl_check(err, "enqueue final partion") ;

		return evt_final_partion ; 
}

output_first_phase* quickSort_cpu_phase_one_v1(cl_command_queue que, cl_kernel partion_phs_1, cl_kernel partion_phs_2, 
		cl_mem* d_buf, cl_mem lt, cl_mem gt,int* vec, const int nels, cl_mem pivot_position, 
		const int lws, const int max_seq_len){

	srand(11) ; 
    cl_int err ; 
    int buffer_swap_index = 0 ; 
   
    Queue sequences_to_partion ;
	int number_of_sequences = -1 ;    
    initQueue(&sequences_to_partion, 100000) ; 

    sequence start_sequence ; 
    start_sequence.sstart = 0 ; 
    start_sequence.send = nels - 1 ; 
	start_sequence.pivot_value = dummy_median(vec, 0, nels - 1) ; 
	enqueue(&sequences_to_partion, &start_sequence) ; 
	size_t memsize = sizeof(cl_int)*nels ;

	output_first_phase* out = calloc(1, sizeof(output_first_phase)) ; 
	out->sstart = calloc(100000, sizeof(int)) ; 
	out->send = calloc(100000, sizeof(int)) ; 

    while(!is_Empty(&sequences_to_partion)){
		sequence curr_seq = dequeue(&sequences_to_partion) ; 
		const int current_nels = curr_seq.send - curr_seq.sstart + 1 ; 

        cl_event evt_partion_phase_1 = partition_phase_1_v1(que, partion_phs_1, d_buf[buffer_swap_index%2], lt, gt, current_nels, lws, curr_seq.sstart, curr_seq.send,  curr_seq.pivot_value) ; 
		clWaitForEvents(1, &evt_partion_phase_1) ;

		err = clFinish(que) ; 
		ocl_check(err, "cl_finish") ; 

		cl_event scan_evt_lt;
		const int nwg = (current_nels)%lws == 0 ? current_nels/lws : (current_nels + lws)/lws ;  
		
		int* lt_cpu = NULL; 
		lt_cpu = clEnqueueMapBuffer(que, lt, CL_TRUE,
					CL_MAP_READ | CL_MAP_WRITE, 0, sizeof(cl_int)*(nwg+1),
						0, NULL, &scan_evt_lt, &err) ; 
		ocl_check(err, "read buffer lt") ; 

		cl_event scan_evt_gt ; 
		int* gt_cpu ; 
		gt_cpu = clEnqueueMapBuffer(que, gt, CL_TRUE,
					CL_MAP_READ | CL_MAP_WRITE, 0, sizeof(cl_int)*(nwg+1),
						0, NULL, &scan_evt_gt, &err);
		ocl_check(err, "read buffer gt");

		scan(lt_cpu, nwg + 1) ; 
		scan(gt_cpu, nwg + 1) ;  

		cl_event unmap_evt_lt;
		err = clEnqueueUnmapMemObject(que, lt, lt_cpu,
					1, &scan_evt_lt, &unmap_evt_lt);
		ocl_check(err, "unmap lt");

		cl_event unmap_evt_gt;
		err = clEnqueueUnmapMemObject(que, gt, gt_cpu,
					1, &scan_evt_gt, &unmap_evt_gt);
		ocl_check(err, "unmap gt");


		cl_event evt_partion_phase_2 = partition_phase_2_v1(que, partion_phs_2, d_buf[buffer_swap_index%2], d_buf[(buffer_swap_index+1)%2], lt, gt, current_nels, nels, lws, 
		curr_seq.pivot_value, curr_seq.sstart, curr_seq.send, pivot_position) ; 

		cl_event read_pivot;
		int *pivot_pos = calloc(1, sizeof(int));
	
		err = clEnqueueReadBuffer(que, pivot_position, CL_TRUE, 0, sizeof(cl_int), pivot_pos, 1, &evt_partion_phase_2, &read_pivot);
		ocl_check(err, "read pivot");

		if(pivot_pos == NULL){
			handle_error("pivot wasn't initialized") ; 
		}

		cl_event read_evt ; 
		int* current_vec = clEnqueueMapBuffer(que, d_buf[(buffer_swap_index + 1)%2], CL_TRUE, CL_MAP_READ, 
        0, memsize, 0, NULL, &read_evt, &err) ;
		ocl_check(err, "map object error") ;
        
        sequence s1, s2 ; 

        s1.sstart = curr_seq.sstart ; 
        s1.send = *pivot_pos - 1 ;  
		const int s1_dim = s1.send - s1.sstart + 1 ; 

		s1.pivot_value = dummy_median(current_vec, s1.sstart, s1.send) ; 

		s2.sstart = *pivot_pos ; 
		s2.send = curr_seq.send ; 
		const int s2_dim = s2.send - s2.sstart + 1 ; 

		s2.pivot_value = dummy_median(current_vec, s2.sstart, s2.send) ; 

		cl_event unmap_evt;
		err = clEnqueueUnmapMemObject(que, d_buf[(buffer_swap_index+1)%2], current_vec,
					1, &read_evt, &unmap_evt);
		ocl_check(err, "unmap d_out");

		if((s1_dim > max_seq_len)){
			enqueue(&sequences_to_partion, &s1) ; 
		}
		else{
			number_of_sequences++ ; 
			out->sstart[number_of_sequences] = s1.sstart ; 
			out->send[number_of_sequences] = s1.send ; 
		}

		if((s2_dim > max_seq_len)){
			enqueue(&sequences_to_partion, &s2) ;
		}
		else{
			number_of_sequences++ ; 
			out->sstart[number_of_sequences] = s2.sstart ; 
			out->send[number_of_sequences] = s2.send ; 
		}

		buffer_swap_index++ ;  

	//	double runtime_partion_phase_1_ms = runtime_ms(evt_partion_phase_1);
	//	double runtime_partion_phase_2_ms = runtime_ms(evt_partion_phase_2);
	//	printf("partion_phase_1: runtime %.4gms, %.4g GE/s, %.4g GB/s\n",
	//	runtime_partion_phase_1_ms, current_nels/runtime_partion_phase_1_ms/1.0e6, current_nels/runtime_partion_phase_1_ms/1.0e6);
	//	printf("partion_phase_2: runtime %.4gms, %.4g GE/s, %.4g GB/s\n",
	//	runtime_partion_phase_2_ms, 2*current_nels/runtime_partion_phase_2_ms/1.0e6, 2*current_nels/runtime_partion_phase_2_ms/1.0e6);
		
		//break ; 
		
    }

	printf("exited\n") ; 

	out->buf_out_index = buffer_swap_index%2 ; 
	out->number_of_sequences = number_of_sequences ; 

	cl_event read_evt ; 
	int* current_vec = clEnqueueMapBuffer(que, d_buf[(out->buf_out_index)], CL_TRUE, CL_MAP_READ, 
	0, memsize, 0, NULL, &read_evt, &err) ;
	ocl_check(err, "map object error") ; 

	cl_event unmap_evt;
	err = clEnqueueUnmapMemObject(que, d_buf[(out->buf_out_index)], current_vec,
				1, &read_evt, &unmap_evt);
	ocl_check(err, "unmap d_out");

	release_queue(&sequences_to_partion) ; 	

	return out ; 
}

void quickSort_cpu_phase_two_v1(cl_context ctx, cl_command_queue que, cl_kernel final_partioning, cl_mem d_in, cl_mem d_out, 
	const int* vec_copy, const int* sstart, const int* send, const int number_of_sequences, const int nels, const int lws, const int min_seq_len){
		
		cl_int err ; 
		const size_t memsize = nels*sizeof(cl_int) ; 
		cl_event read_evt ; 

		cl_mem sstart_ker = clCreateBuffer(ctx, CL_MEM_READ_WRITE | CL_MEM_USE_HOST_PTR  , (number_of_sequences + 1)*sizeof(cl_int), (void*)sstart, &err);
		ocl_check(err, "create buffer sstart_ker");

		cl_mem send_ker = clCreateBuffer(ctx, CL_MEM_READ_WRITE | CL_MEM_USE_HOST_PTR  , (number_of_sequences + 1)*sizeof(cl_int), (void*)send, &err);
		ocl_check(err, "create buffer sstart_ker");

		cl_event final_partion_evt = final_partion_v1(que, final_partioning, d_in, d_out, nels, lws, sstart_ker, send_ker, number_of_sequences, min_seq_len) ;   

		err = clFinish(que) ; 
		ocl_check(err, "finish final_partition") ; 

		double runtime_ms_final_partion = runtime_ms(final_partion_evt) ; 
		printf("final: runtime %.4gms, %.4g GE/s, %.4g GB/s\n",
		runtime_ms_final_partion, nels/runtime_ms_final_partion/1.0e6, nels/runtime_ms_final_partion/1.0e6);


		int* current_vec = clEnqueueMapBuffer(que, d_in, CL_TRUE, CL_MAP_READ, 
        0, memsize, 0, NULL, &read_evt, &err) ; 
		ocl_check(err, "map object error") ;  

		check_result(current_vec, vec_copy, nels) ;

		cl_event unmap_evt ; 
		err = clEnqueueUnmapMemObject(que, d_in, current_vec,
						1, &read_evt, &unmap_evt) ; 
		ocl_check(err, "unmap d_out") ;

		clReleaseMemObject(sstart_ker) ; 
		clReleaseMemObject(send_ker) ; 

		free((void*)sstart) ; 
		free((void*)send) ;  

}	