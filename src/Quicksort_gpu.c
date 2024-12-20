#include "../include/Quicksort_gpu.h"


cl_event split_elements(cl_command_queue q, kernels* k, device_memeory* m , cl_int nels, cl_int sstart,
	cl_int lws_, cl_float pivot, const int nwg){
	size_t lws[] = {lws_} ; 
	size_t gws[] = {nwg*lws[0]};
	 
	cl_int err ; 
	cl_event evt_split_elements ;

	err = clSetKernelArg(k->splitting_elements, 0, sizeof(cl_int), &nels) ;
	ocl_check(err, "set kernel nels ") ;

	err = clSetKernelArg(k->splitting_elements, 1, sizeof(cl_int), &sstart) ;
	ocl_check(err, "set kernel sstart ") ;

	err = clSetKernelArg(k->splitting_elements, 2, sizeof(m->in), &m->in) ;
	ocl_check(err, "set kernel d_buf") ;

	err = clSetKernelArg(k->splitting_elements, 3, sizeof(pivot), &pivot) ;
	ocl_check(err, "set kernel pivot") ;

	err = clSetKernelArg(k->splitting_elements, 4, sizeof(m->bit_map_sup), &m->bit_map_sup) ;
	ocl_check(err, "set kernel bit_array_sup") ;

	err = clSetKernelArg(k->splitting_elements, 5, sizeof(m->bit_map_inf), &m->bit_map_inf) ;
	ocl_check(err, "set kernel bit_array_inf") ;

	err = clEnqueueNDRangeKernel(q, k->splitting_elements, 1, NULL, gws, lws, 0, NULL, &evt_split_elements) ;
	
    ocl_check(err, "enqueue split elements kernel") ; 

    return evt_split_elements;
}

cl_event scan_seq_lmem(cl_command_queue q, kernels* k, device_memeory* m, cl_mem in_1, cl_mem in_2, cl_int nels, cl_int lws_, const int nwg){

	cl_int err ; 
	cl_event scan_evt ; 

	size_t lws[] = {lws_} ; 
	size_t gws[] = {nwg*lws[0]} ; 

	err = clSetKernelArg(k->scan_gpu_lmem, 0, sizeof(cl_int), &nels) ;
	ocl_check(err, "set kernel nels ") ;

	err = clSetKernelArg(k->scan_gpu_lmem, 1, sizeof(in_1), &in_1) ;
	ocl_check(err, "set kernel d_buf") ;

	err = clSetKernelArg(k->scan_gpu_lmem, 2, sizeof(in_2), &in_2) ;
	ocl_check(err, "set kernel d_buf") ;

	err = clSetKernelArg(k->scan_gpu_lmem, 3, sizeof(m->tails_inf), &m->tails_inf) ;
	ocl_check(err, "set kernel lt") ;

	err = clSetKernelArg(k->scan_gpu_lmem, 4, sizeof(m->tails_sup), &m->tails_sup) ;
	ocl_check(err, "set kernel gt") ;

	err = clSetKernelArg(k->scan_gpu_lmem, 5, sizeof(cl_int)*lws[0], NULL) ;
	ocl_check(err, "set kernel local mem inf") ;

	err = clSetKernelArg(k->scan_gpu_lmem, 6, sizeof(cl_int)*lws[0], NULL) ;
	ocl_check(err, "set kernel local mem sup") ;

	err = clEnqueueNDRangeKernel(q, k->scan_gpu_lmem, 1, NULL, gws, lws, 0, NULL, &scan_evt) ;
    ocl_check(err, "enqueue scan seq kernel") ; 

    return scan_evt; 

}

cl_event scan_seq(cl_command_queue q, kernels* k, device_memeory* m, cl_mem in_1, cl_mem in_2, cl_int nels, const int nwg){

	cl_int err ; 
	cl_event scan_evt ; 

	size_t lws[] = {1} ; 
	size_t gws[] = {nwg*lws[0]} ; 

	err = clSetKernelArg(k->scan_gpu, 0, sizeof(cl_int), &nels) ;
	ocl_check(err, "set kernel nels ") ;

	err = clSetKernelArg(k->scan_gpu, 1, sizeof(in_1), &in_1) ;
	ocl_check(err, "set kernel d_buf") ;

	err = clSetKernelArg(k->scan_gpu, 2, sizeof(in_2), &in_2) ;
	ocl_check(err, "set kernel d_buf") ;

	err = clSetKernelArg(k->scan_gpu, 3, sizeof(m->tails_inf), &m->tails_inf) ;
	ocl_check(err, "set kernel lt") ;

	err = clSetKernelArg(k->scan_gpu, 4, sizeof(m->tails_sup), &m->tails_sup) ;
	ocl_check(err, "set kernel gt") ;

	err = clEnqueueNDRangeKernel(q, k->scan_gpu, 1, NULL, gws, lws, 0, NULL, &scan_evt) ;
    ocl_check(err, "enqueue scan seq kernel") ; 

    return scan_evt; 

}

cl_event scan_seq_update(cl_command_queue q, kernels* k, device_memeory* m, cl_int nels, cl_int lws_, const int nwg){

	cl_int err ; 
	cl_event scan_evt ; 

	size_t lws[] = {lws_} ; 
	size_t gws[] = {nwg*lws[0]} ; 

	err = clSetKernelArg(k->scan_update, 0, sizeof(cl_int), &nels) ;
	ocl_check(err, "set kernel nels ") ;
	
	err = clSetKernelArg(k->scan_update, 1, sizeof(m->bit_map_sup), &m->bit_map_sup) ;
	ocl_check(err, "set kernel d_buf") ;

	err = clSetKernelArg(k->scan_update, 2, sizeof(m->bit_map_inf), &m->bit_map_inf) ;
	ocl_check(err, "set kernel d_buf") ;

	err = clSetKernelArg(k->scan_update, 3, sizeof(m->tails_sup), &m->tails_sup) ;
	ocl_check(err, "set kernel gt") ;

	err = clSetKernelArg(k->scan_update, 4, sizeof(m->tails_inf), &m->tails_inf) ;
	ocl_check(err, "set kernel lt") ;

	err = clEnqueueNDRangeKernel(q, k->scan_update, 1, NULL, gws, lws, 0, NULL, &scan_evt) ;
    ocl_check(err, "enqueue scan seq update kernel") ; 

    return scan_evt; 

}

cl_event partition(cl_command_queue q, kernels* k, device_memeory* m,
	sequence seq, const int nels, const int sum_lt, const int sum_gt, const int lws_, const int nwg){

	cl_int err ; 
	cl_event partition_evt ; 

	size_t lws[] = {lws_} ; 
	size_t gws[] = {nwg*lws[0]} ; 

	err = clSetKernelArg(k->partitioning, 0, sizeof(cl_int), &nels) ;
	ocl_check(err, "set kernel nels ") ;

	err = clSetKernelArg(k->partitioning, 1, sizeof(cl_int), &seq.sstart) ;
	ocl_check(err, "set kernel seq ") ;

	err = clSetKernelArg(k->partitioning, 2, sizeof(cl_int), &seq.pivot_value) ;
	ocl_check(err, "set kernel seq ") ;

	err = clSetKernelArg(k->partitioning, 3, sizeof(cl_int), &sum_lt) ;
	ocl_check(err, "set kernel seq ") ;

	err = clSetKernelArg(k->partitioning, 4, sizeof(cl_int), &sum_gt) ;
	ocl_check(err, "set kernel seq ") ;

	err = clSetKernelArg(k->partitioning, 5, sizeof(m->in), &m->in) ;
	ocl_check(err, "set kernel in") ;

	err = clSetKernelArg(k->partitioning, 6, sizeof(m->buff_tmp), &m->buff_tmp) ;
	ocl_check(err, "set kernel out") ;

	err = clSetKernelArg(k->partitioning, 7, sizeof(m->bit_map_sup), &m->bit_map_sup) ;
	ocl_check(err, "set kernel bit_map_sup") ;

	err = clSetKernelArg(k->partitioning, 8, sizeof(m->bit_map_inf), &m->bit_map_inf) ;
	ocl_check(err, "set kernel bit_map_inf") ; 

	err = clEnqueueNDRangeKernel(q,  k->partitioning, 1, NULL, gws, lws, 0, NULL, &partition_evt) ;
    ocl_check(err, "enqueue partition kernel") ; 

    return partition_evt; 
}

cl_event partition_copy(cl_command_queue que, kernels* k, device_memeory* m, const sequence seq, const int nels, const int lws_, const int nwg){

	cl_int err ; 
	cl_event partition_evt ; 

	size_t lws[] = {lws_} ; 
	size_t gws[] = {nwg*lws[0]} ; 

	err = clSetKernelArg(k->partitioning_copy, 0, sizeof(cl_int), &nels) ;
	ocl_check(err, "set kernel nels ") ;

	err = clSetKernelArg(k->partitioning_copy, 1, sizeof(cl_int), &seq.sstart) ;
	ocl_check(err, "set kernel seq ") ;

	err = clSetKernelArg(k->partitioning_copy, 2, sizeof(m->in), &m->in) ;
	ocl_check(err, "set kernel in") ;

	err = clSetKernelArg(k->partitioning_copy, 3, sizeof(m->buff_tmp), &m->buff_tmp) ;
	ocl_check(err, "set kernel out") ;

	err = clEnqueueNDRangeKernel(que,  k->partitioning_copy, 1, NULL, gws, lws, 0, NULL, &partition_evt) ;
    ocl_check(err, "enqueue partition copy kernel") ; 

    return partition_evt; 

}

cl_event final_partition_lmem(cl_command_queue que, kernels* k, device_memeory* m, const int num_sequences, const int lws_, const int nwg){
	cl_int err ; 
	cl_event final_partition_lmem_evt ; 

	size_t lws[] = {lws_} ; 
	size_t gws[] = {nwg*lws[0]} ;


	err = clSetKernelArg(k->quicksort_lmem, 0, sizeof(cl_int), &num_sequences) ;
	ocl_check(err, "set kernel nels ") ;

	err = clSetKernelArg(k->quicksort_lmem, 1, sizeof(m->in), &m->in) ;
	ocl_check(err, "set kernel seq ") ;

	err = clSetKernelArg(k->quicksort_lmem, 2, sizeof(m->sstart_arr), &m->sstart_arr) ;
	ocl_check(err, "set kernel in") ;

	err = clSetKernelArg(k->quicksort_lmem, 3, sizeof(m->send_arr), &m->send_arr) ;
	ocl_check(err, "set kernel out") ;

	err = clSetKernelArg(k->quicksort_lmem, 4, sizeof(cl_int)*lws[0], NULL) ;
	ocl_check(err, "set kernel local mem inf") ;

	err = clSetKernelArg(k->quicksort_lmem, 5, sizeof(cl_int)*lws[0], NULL) ;
	ocl_check(err, "set kernel local mem sup") ;

	err = clEnqueueNDRangeKernel(que,  k->quicksort_lmem, 1, NULL, gws, lws, 0, NULL, &final_partition_lmem_evt) ;
    ocl_check(err, "enqueue partition copy kernel") ; 

    return final_partition_lmem_evt; 
}

cl_event final_partition_lmem4(cl_command_queue que, kernels* k, device_memeory* m, const int num_sequences, const int lws_, const int nwg){
	cl_int err ; 
	cl_event final_partition_lmem_evt ; 

	size_t lws[] = {lws_} ; 
	size_t gws[] = {nwg*lws[0]} ;


	err = clSetKernelArg(k->quicksort_lmem4, 0, sizeof(cl_int), &num_sequences) ;
	ocl_check(err, "set kernel nels ") ;

	err = clSetKernelArg(k->quicksort_lmem4, 1, sizeof(m->in), &m->in) ;
	ocl_check(err, "set kernel seq ") ;

	err = clSetKernelArg(k->quicksort_lmem4, 2, sizeof(m->sstart_arr), &m->sstart_arr) ;
	ocl_check(err, "set kernel in") ;

	err = clSetKernelArg(k->quicksort_lmem4, 3, sizeof(m->send_arr), &m->send_arr) ;
	ocl_check(err, "set kernel out") ;

	err = clSetKernelArg(k->quicksort_lmem4, 4, sizeof(cl_int)*lws[0], NULL) ;
	ocl_check(err, "set kernel local mem inf") ;

	err = clSetKernelArg(k->quicksort_lmem4, 5, sizeof(cl_int)*lws[0], NULL) ;
	ocl_check(err, "set kernel local mem sup") ;

	err = clEnqueueNDRangeKernel(que,  k->quicksort_lmem4, 1, NULL, gws, lws, 0, NULL, &final_partition_lmem_evt) ;
    ocl_check(err, "enqueue partition copy kernel") ; 

    return final_partition_lmem_evt; 
}

float* quickSortGpu(const float* vec,  const int nels, const int lws, const int nwg, cl_resources* resources, const bool test_correctness, const bool local_memory, const bool pruning_lwsx4){

	if(resources == NULL){
		handle_error("resources set to null\n") ; 
	}

	times* t = calloc(MAX_NUM_SEQ, sizeof(times)) ; 
	sequences_info* s = calloc(MAX_NUM_SEQ, sizeof(sequences_info)) ; 

	time_t quicksort_gpu_start, quicksort_gpu_end ; 
	double time_used_gpu ; 
	quicksort_gpu_start = clock() ; 
	

	cl_int err ; 

	kernels k ; 

	k.splitting_elements = clCreateKernel(resources->prog, "split_elements", &err);	
	ocl_check(err, "create kernel split_elements");
	k.scan_gpu_lmem = clCreateKernel(resources->prog, "scan_lmem", &err);	
	ocl_check(err, "create kernel scan_lmem");
	k.scan_gpu = clCreateKernel(resources->prog, "scan", &err);	
	ocl_check(err, "create kernel scan_lmem");
	k.scan_update = clCreateKernel(resources->prog, "scan_update", &err);	
	ocl_check(err, "create kernel scan_update");
	k.partitioning = clCreateKernel(resources->prog, "partition", &err);	
	ocl_check(err, "create kernel partition");
	k.partitioning_copy = clCreateKernel(resources->prog, "partition_copy", &err);	
	ocl_check(err, "create kernel partition copy");
	k.quicksort_lmem = clCreateKernel(resources->prog, "quicksort_lmem", &err);	
	ocl_check(err, "create kernel quicksort_lmem");
	k.quicksort_lmem4 = clCreateKernel(resources->prog, "quicksort_lmem4", &err);	
	ocl_check(err, "create kernel quicksort_lmem4");

	device_memeory m ; 

	m.in = clCreateBuffer(resources->ctx, CL_MEM_READ_WRITE | CL_MEM_COPY_HOST_PTR, (nels)*sizeof(cl_float), (void*)vec, &err); 
	ocl_check(err, "create buffer d_buf");
	m.buff_tmp = clCreateBuffer(resources->ctx, CL_MEM_READ_WRITE | CL_MEM_ALLOC_HOST_PTR , (nels)*sizeof(cl_float), NULL, &err);
	ocl_check(err, "create buffer lt");
	m.bit_map_sup = clCreateBuffer(resources->ctx, CL_MEM_READ_WRITE | CL_MEM_ALLOC_HOST_PTR , nels*sizeof(cl_int), NULL, &err);
	ocl_check(err, "create buffer bit map sup");
	m.bit_map_inf = clCreateBuffer(resources->ctx, CL_MEM_READ_WRITE | CL_MEM_ALLOC_HOST_PTR , nels*sizeof(cl_int), NULL, &err);
	ocl_check(err, "create buffer bit map inf");
	m.tails_inf = clCreateBuffer(resources->ctx, CL_MEM_READ_WRITE | CL_MEM_ALLOC_HOST_PTR , nwg*sizeof(cl_int), NULL, &err);
	ocl_check(err, "create buffer bit map inf");
	m.tails_sup = clCreateBuffer(resources->ctx, CL_MEM_READ_WRITE | CL_MEM_ALLOC_HOST_PTR , nwg*sizeof(cl_int), NULL, &err);
	ocl_check(err, "create buffer bit map inf");
   
    Queue sequences_to_partion ;
    initQueue(&sequences_to_partion, MAX_NUM_SEQ) ;

    sequence start_sequence ; 
    start_sequence.sstart = 0 ; 
    start_sequence.send = nels - 1 ; 
	start_sequence.pivot_value = vec[random_uniform_value(0, nels - 1)] ; 
	enqueue(&sequences_to_partion, &start_sequence) ;


	int* sstart_arr = calloc(nels, sizeof(int)) ;  
	int* send_arr = calloc(nels, sizeof(int)) ; 
	int seq_arr_dim = 0 ;    

	int iteration = 0; 

	int min_seq_len = (pruning_lwsx4) ? lws*4 : lws ;  

    while(!is_Empty(&sequences_to_partion)){ 

		sequence curr_seq = dequeue(&sequences_to_partion) ; 
		const int current_nels = curr_seq.send - curr_seq.sstart + 1 ;
		int current_nwg = nwg ;

		if(current_nwg*lws > current_nels){
			current_nwg = current_nels/lws ;  
		}

		if(current_nwg <= 1) current_nwg=2 ; 

        cl_event evt_split_elements = split_elements(resources->que, &k, &m, current_nels, curr_seq.sstart, lws, curr_seq.pivot_value, current_nwg) ;  

		clWaitForEvents(1, &evt_split_elements) ;

		cl_event scan_evt[3] ; 

		if(local_memory){
			scan_evt[0] = scan_seq_lmem(resources->que, &k, &m,  m.bit_map_sup, m.bit_map_inf, current_nels, lws, current_nwg) ;
			scan_evt[1] = scan_seq_lmem(resources->que, &k, &m, m.tails_sup, m.tails_inf, current_nwg, lws, 1) ;  
			scan_evt[2] = scan_seq_update(resources->que, &k, &m, current_nels, lws, current_nwg - 1) ;
		}
		else{
			scan_evt[0] = scan_seq(resources->que, &k, &m,  m.bit_map_sup, m.bit_map_inf, current_nels, current_nwg) ;
			scan_evt[1] = scan_seq(resources->que, &k, &m, m.tails_sup, m.tails_inf, current_nwg, 1) ;  
			scan_evt[2] = scan_seq_update(resources->que, &k, &m, current_nels, lws, current_nwg - 1) ;
		}  


		clWaitForEvents(3, scan_evt) ; 


		cl_event read_evt_lt ;
		cl_event read_evt_gt ; 
		cl_event read_evt_last_value ; 
  
		int* lt = NULL ;
		int* gt = NULL ; 
		float* last_value =  NULL ; 
		
		lt = clEnqueueMapBuffer(resources->que, m.bit_map_inf, CL_TRUE,
					CL_MAP_READ | CL_MAP_WRITE, (current_nels - 1)*sizeof(cl_int), sizeof(cl_int),
						0, NULL, &read_evt_lt, &err) ; 
		ocl_check(err, "read buffer lt") ; 

		last_value = clEnqueueMapBuffer(resources->que, m.in, CL_TRUE,
					CL_MAP_READ | CL_MAP_WRITE,  (curr_seq.sstart +current_nels - 1)*sizeof(cl_float), sizeof(cl_float),
						0, NULL, &read_evt_last_value, &err) ; 
		ocl_check(err, "read buffer last value") ; 

		gt = clEnqueueMapBuffer(resources->que, m.bit_map_sup, CL_TRUE,
					CL_MAP_READ | CL_MAP_WRITE, (current_nels - 1)*sizeof(cl_int), sizeof(cl_int),
						0, NULL, &read_evt_gt, &err);
		ocl_check(err, "read buffer gt");

		const int sum_lt = (*last_value >= curr_seq.pivot_value) ? *lt : *lt +1 ; 
		const int sum_gt = (*last_value <= curr_seq.pivot_value) ? *gt : *gt +1 ;

		cl_event unmap_evt_lt;
		err = clEnqueueUnmapMemObject(resources->que, m.bit_map_inf, lt,
					1, &read_evt_lt, &unmap_evt_lt);
		ocl_check(err, "unmap lt");

		cl_event unmap_evt_gt;
		err = clEnqueueUnmapMemObject(resources->que, m.bit_map_sup, gt,
					1, &read_evt_gt, &unmap_evt_gt);
		ocl_check(err, "unmap gt");

		cl_event unmap_evt_last_value;
		err = clEnqueueUnmapMemObject(resources->que, m.in, last_value,
					1, &read_evt_last_value, &unmap_evt_last_value);
		ocl_check(err, "unmap last value");

		cl_event partition_evt = partition(resources->que, &k,  &m, curr_seq, current_nels, sum_lt, sum_gt, lws, current_nwg) ; 
		clWaitForEvents(1, &partition_evt) ;
		cl_event partition_copy_evt = partition_copy(resources->que, &k, &m, curr_seq, current_nels, lws, current_nwg) ;
		clWaitForEvents(1, &partition_copy_evt) ; 

		sequence s1, s2 ; 

		s1.sstart = curr_seq.sstart ; 
		s1.send = s1.sstart + sum_lt - 1;

		/** 
		 * sequence pivot_seq ;
		 * pivot_seq.send = curr_seq.send - sum_gt ; 
		 * pivot_seq.sstart = s1.sstart + sum_lt ; 
		 * this is the third partion , which is alredy sorted
		 * because elements are equals
		*/

		s2.sstart = curr_seq.send - sum_gt + 1 ; 
		s2.send = curr_seq.send ;  

		const int s1_dim = s1.send - s1.sstart + 1 ; 
		const int s2_dim = s2.send - s2.sstart + 1 ;  

		if((s1_dim > min_seq_len)){
			const int pivot_index = random_uniform_value(0, s1_dim - 1) + s1.sstart; 

			#if 1
				cl_event read_pivot_s1_evt ; 
				cl_event unmap_pivot_s1_evt ; 
				float* s1_pivot = NULL;

				s1_pivot = clEnqueueMapBuffer(resources->que, m.in, CL_TRUE,
						CL_MAP_READ | CL_MAP_WRITE, sizeof(cl_float)*pivot_index, sizeof(cl_float),
							0, NULL, & read_pivot_s1_evt , &err) ; 
				ocl_check(err, "read s1 pivot") ;

				s1.pivot_value = (float)*s1_pivot ; 

				err = clEnqueueUnmapMemObject(resources->que, m.in, s1_pivot,
						1, &read_pivot_s1_evt , &unmap_pivot_s1_evt);
				ocl_check(err, "unmap s1 pivot");
			#else
				float s1_pivot ; 
				err = clEnqueueReadBuffer(resources->que, m.in, CL_TRUE, 0, sizeof(cl_float), &s1_pivot, 0, NULL,  NULL);
				s1.pivot_value = s1_pivot ; 
			
			#endif

			enqueue(&sequences_to_partion, &s1) ; 
		}
		else if(s1_dim > 1){
			sstart_arr[seq_arr_dim] = s1.sstart ; 
			send_arr[seq_arr_dim] = s1.send ; 
			seq_arr_dim++ ;  
		}

		if((s2_dim > min_seq_len)){
			const int pivot_index = random_uniform_value(0, s2_dim - 1) + s2.sstart; 

			#if 1
				cl_event read_pivot_s2_evt ; 
				cl_event unmap_pivot_s2_evt ; 
				float* s2_pivot = NULL;

				s2_pivot = clEnqueueMapBuffer(resources->que, m.in, CL_TRUE,
						CL_MAP_READ | CL_MAP_WRITE, sizeof(cl_float)*pivot_index, sizeof(cl_float),
							0, NULL, &read_pivot_s2_evt , &err) ; 
				ocl_check(err, "read s2 pivot") ;

				s2.pivot_value = (float)*s2_pivot ; 

				err = clEnqueueUnmapMemObject(resources->que, m.in, s2_pivot,
						1, &read_pivot_s2_evt , &unmap_pivot_s2_evt);
				ocl_check(err, "unmap s2 pivot");
			#else
				float s2_pivot ; 
				err = clEnqueueReadBuffer(resources->que, m.in, CL_TRUE, pivot_index*sizeof(cl_float), sizeof(cl_float), &s2_pivot, 0, NULL,  NULL);
				s2.pivot_value = s2_pivot ; 	
			#endif
 
			enqueue(&sequences_to_partion, &s2) ;
		}
		else if(s2_dim > 1){
			sstart_arr[seq_arr_dim] = s2.sstart ;
			send_arr[seq_arr_dim] = s2.send ; 
			seq_arr_dim++ ; 
		}

		if(test_correctness){
			t[iteration].split_elements_time = runtime_ns(evt_split_elements) ;
			t[iteration].partial_scan_time = runtime_ns(scan_evt[0]) ; 
			t[iteration].scan_tails_time = runtime_ns(scan_evt[1]) ; 
			t[iteration].scan_update_time = runtime_ns(scan_evt[2]) ; 
			t[iteration].partition_time = runtime_ns(partition_evt) ; 
			t[iteration].partition_copy_time = runtime_ns(partition_copy_evt) ; 
			s[iteration].current_nels = current_nels ; 
			s[iteration].current_nwg = current_nwg ; 
			iteration++; 
		}
    }  

	printf("ended first phase...\n") ; 
	fflush(stdout) ; 

	m.sstart_arr = clCreateBuffer(resources->ctx, CL_MEM_READ_WRITE | CL_MEM_COPY_HOST_PTR, (seq_arr_dim)*sizeof(cl_int), (void*)sstart_arr, &err); 
	ocl_check(err, "create buffer sstart arr");
	m.send_arr = clCreateBuffer(resources->ctx, CL_MEM_READ_WRITE | CL_MEM_COPY_HOST_PTR, (seq_arr_dim)*sizeof(cl_int), (void*)send_arr, &err); 
	ocl_check(err, "create buffer ssend arr");

	cl_event final_partition_lmem_evt ; 

	if(pruning_lwsx4){
		final_partition_lmem_evt = final_partition_lmem4(resources->que, &k, &m, seq_arr_dim, lws, nwg) ;
	}
	else{
		final_partition_lmem_evt = final_partition_lmem(resources->que, &k, &m, seq_arr_dim, lws, nwg) ;
	}
	
	clWaitForEvents(1, &final_partition_lmem_evt) ;  
	unsigned long final_partition_lmem_time = runtime_ns( final_partition_lmem_evt) ; 
	printf("number of initial sequences : %d\n", seq_arr_dim) ; 
	printf("final partition lmem kernel : %.4gs\n", final_partition_lmem_time/(1.0e9)) ; 

	quicksort_gpu_end = clock() ; 
	time_used_gpu = ((double)(quicksort_gpu_end - quicksort_gpu_start))/CLOCKS_PER_SEC ; 
	printf("time used gpu:  %f\n", time_used_gpu) ; 

	cl_event read_out_evt ; 
	cl_event unmap_out_evt ; 
	float* out = NULL;
	float* out_copy ; 

	out = clEnqueueMapBuffer(resources->que, m.in, CL_TRUE,
				CL_MAP_READ | CL_MAP_WRITE, 0, sizeof(cl_float)*nels,
					0, NULL, &read_out_evt , &err) ; 
	ocl_check(err, "read buffer out") ;

	if(test_correctness){

		bench_mark(t, iteration, s, lws, seq_arr_dim) ;

		float*vec_to_sort_on_cpu =  calloc(nels, sizeof(float)) ;

		copy_vec(vec, vec_to_sort_on_cpu, 0, nels - 1) ;

		time_t quicksort_cpu_start, quicksort_cpu_end ; 
		double time_used_cpu ; 
		quicksort_cpu_start = clock() ;   

		quicksort(vec_to_sort_on_cpu, 0, nels - 1) ; 

		quicksort_cpu_end = clock() ; 
		time_used_cpu = ((double)(quicksort_cpu_end - quicksort_cpu_start))/CLOCKS_PER_SEC ;
		printf("cpu time:%f\n", time_used_cpu) ;  

		check_result(out, vec_to_sort_on_cpu, nels) ;

		free(vec_to_sort_on_cpu) ; 

	}
	else{
		out_copy =  calloc(nels, sizeof(float)) ;
		copy_vec(out, out_copy, 0, nels - 1) ; 
	}

	err = clEnqueueUnmapMemObject(resources->que, m.in, out,
				1, &read_out_evt, &unmap_out_evt);
		ocl_check(err, "unmap buffer out");

	release_queue(&sequences_to_partion) ; 	
	free(sstart_arr) ; 
	free(send_arr) ; 
		
	clReleaseMemObject(m.in) ;
	clReleaseMemObject(m.buff_tmp) ;
	clReleaseMemObject(m.bit_map_inf) ;
	clReleaseMemObject(m.bit_map_sup) ;
	clReleaseMemObject(m.tails_inf) ;
	clReleaseMemObject(m.tails_sup) ; 
	clReleaseMemObject(m.sstart_arr) ; 
	clReleaseMemObject(m.send_arr) ; 

	clReleaseKernel(k.splitting_elements) ; 
	clReleaseKernel(k.partitioning) ;
	clReleaseKernel(k.partitioning_copy) ;
	clReleaseKernel(k.scan_gpu) ;
	clReleaseKernel(k.scan_update) ;
	clReleaseKernel(k.quicksort_lmem) ; 
	clReleaseKernel(k.quicksort_lmem4) ; 

	return out_copy ; 

}
