#include "../include/Quicksort_stream_compacting.h"

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

	err = clSetKernelArg(k->splitting_elements, 4, sizeof(m->lt), &m->lt) ;
	ocl_check(err, "set kernel lt") ;

	err = clSetKernelArg(k->splitting_elements, 5, sizeof(m->gt), &m->gt) ;
	ocl_check(err, "set kernel gt") ;

	err = clSetKernelArg(k->splitting_elements, 6, sizeof(m->bit_map_sup), &m->bit_map_sup) ;
	ocl_check(err, "set kernel bit_array_sup") ;

	err = clSetKernelArg(k->splitting_elements, 7, sizeof(m->bit_map_inf), &m->bit_map_inf) ;
	ocl_check(err, "set kernel bit_array_inf") ;

	err = clSetKernelArg(k->splitting_elements, 8, sizeof(cl_int)*lws[0], NULL) ;
	ocl_check(err, "set kernel local mem inf") ;

	err = clSetKernelArg(k->splitting_elements, 9, sizeof(cl_int)*lws[0], NULL) ;
	ocl_check(err, "set kernel local mem sup") ;

	err = clEnqueueNDRangeKernel(q, k->splitting_elements, 1, NULL, gws, lws, 0, NULL, &evt_split_elements) ;
    ocl_check(err, "enqueue split elements kernel") ; 

    return evt_split_elements;
}

cl_event scan_seq(cl_command_queue q, kernels* k, device_memeory* m, cl_mem in_1, cl_mem in_2, cl_int nels, cl_int lws_, const int nwg){

	cl_int err ; 
	cl_event scan_evt ; 

	size_t lws[] = {lws_} ; 
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

	err = clSetKernelArg(k->scan_gpu, 5, sizeof(cl_int)*lws[0], NULL) ;
	ocl_check(err, "set kernel local mem inf") ;

	err = clSetKernelArg(k->scan_gpu, 6, sizeof(cl_int)*lws[0], NULL) ;
	ocl_check(err, "set kernel local mem sup") ;

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

cl_event partition(cl_command_queue q, kernels* k, device_memeory* m, cl_int lt, cl_int gt,
	sequence seq, const int nels, const int lws_, const int nwg){

	cl_int err ; 
	cl_event partition_evt ; 

	size_t lws[] = {lws_} ; 
	size_t gws[] = {nwg*lws[0]} ; 

	err = clSetKernelArg(k->partitioning, 0, sizeof(cl_int), &nels) ;
	ocl_check(err, "set kernel nels ") ;

	err = clSetKernelArg(k->partitioning, 1, sizeof(sequence), &seq) ;
	ocl_check(err, "set kernel seq ") ;

	err = clSetKernelArg(k->partitioning, 2, sizeof(m->in), &m->in) ;
	ocl_check(err, "set kernel in") ;

	err = clSetKernelArg(k->partitioning, 3, sizeof(m->buff_tmp), &m->buff_tmp) ;
	ocl_check(err, "set kernel out") ;

	err = clSetKernelArg(k->partitioning, 4, sizeof(m->bit_map_sup), &m->bit_map_sup) ;
	ocl_check(err, "set kernel bit_map_sup") ;

	err = clSetKernelArg(k->partitioning, 5, sizeof(m->bit_map_inf), &m->bit_map_inf) ;
	ocl_check(err, "set kernel bit_map_inf") ;

	err = clSetKernelArg(k->partitioning, 6, sizeof(cl_int), &lt) ;
	ocl_check(err, "set kernel lt") ;

	err = clSetKernelArg(k->partitioning, 7, sizeof(cl_int), &gt) ;
	ocl_check(err, "set kernel gt") ;

	err = clEnqueueNDRangeKernel(q,  k->partitioning, 1, NULL, gws, lws, 0, NULL, &partition_evt) ;
    ocl_check(err, "enqueue partition kernel") ; 

    return partition_evt; 
}

cl_event partition_copy(cl_command_queue que, kernels* k, device_memeory* m,  int lt, 
	const int gt, const sequence seq, const int nels, const int lws_, const int nwg){

	cl_int err ; 
	cl_event partition_evt ; 

	size_t lws[] = {lws_} ; 
	size_t gws[] = {nwg*lws[0]} ; 

	err = clSetKernelArg(k->partitioning_copy, 0, sizeof(cl_int), &nels) ;
	ocl_check(err, "set kernel nels ") ;

	err = clSetKernelArg(k->partitioning_copy, 1, sizeof(sequence), &seq) ;
	ocl_check(err, "set kernel seq ") ;

	err = clSetKernelArg(k->partitioning_copy, 2, sizeof(m->in), &m->in) ;
	ocl_check(err, "set kernel in") ;

	err = clSetKernelArg(k->partitioning_copy, 3, sizeof(m->buff_tmp), &m->buff_tmp) ;
	ocl_check(err, "set kernel out") ;

	err = clSetKernelArg(k->partitioning_copy, 4, sizeof(cl_int), &lt) ;
	ocl_check(err, "set kernel lt") ;

	err = clSetKernelArg(k->partitioning_copy, 5, sizeof(cl_int), &gt) ;
	ocl_check(err, "set kernel gt") ;

	err = clEnqueueNDRangeKernel(que,  k->partitioning_copy, 1, NULL, gws, lws, 0, NULL, &partition_evt) ;
    ocl_check(err, "enqueue partition copy kernel") ; 

    return partition_evt; 

}

float* quickSortGpu(const float* vec,  const int nels, const int lws, const int nwg, cl_resources* resources, bool test_correctness){

	if(resources == NULL){
		handle_error("resources set to null\n") ; 
	}

	times* t = calloc(MAX_NUM_SEQ, sizeof(times)) ; 
	sequences_info* s = calloc(MAX_NUM_SEQ, sizeof(sequences_info)) ;  

	time_t start, end ; 
	double time_used ; 
	start = clock() ; 
	

	cl_int err ; 

	kernels k ; 

	k.splitting_elements = clCreateKernel(resources->prog, "split_elements", &err);	
	ocl_check(err, "create kernel split_elements");
	k.scan_gpu = clCreateKernel(resources->prog, "scan", &err);	
	ocl_check(err, "create kernel scan");
	k.scan_update = clCreateKernel(resources->prog, "scan_update", &err);	
	ocl_check(err, "create kernel scan_update");
	k.partitioning = clCreateKernel(resources->prog, "partition_buff_tmp", &err);	
	ocl_check(err, "create kernel partition_buff_tmp");
	k.partitioning_copy = clCreateKernel(resources->prog, "partition_copy", &err);	
	ocl_check(err, "create kernel partition_buff_tmp");

	device_memeory m ; 

	m.in = clCreateBuffer(resources->ctx, CL_MEM_READ_WRITE | CL_MEM_COPY_HOST_PTR, (nels)*sizeof(cl_float), (void*)vec, &err); 
	ocl_check(err, "create buffer d_buf");
	m.buff_tmp = clCreateBuffer(resources->ctx, CL_MEM_READ_WRITE | CL_MEM_ALLOC_HOST_PTR , (nels)*sizeof(cl_float), NULL, &err);
	ocl_check(err, "create buffer lt");
	m.lt = clCreateBuffer(resources->ctx, CL_MEM_READ_WRITE | CL_MEM_ALLOC_HOST_PTR , (nwg)*sizeof(cl_int), NULL, &err);
	ocl_check(err, "create buffer lt");
	m.gt = clCreateBuffer(resources->ctx, CL_MEM_READ_WRITE | CL_MEM_ALLOC_HOST_PTR, (nwg)*sizeof(cl_int), NULL, &err);
	ocl_check(err, "create buffer gt"); 
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
	start_sequence.pivot_value = vec[rand()%nels] ; 
	enqueue(&sequences_to_partion, &start_sequence) ;  

	int iteration = 0; 
	const int nquarts = nels/4 ; 

    while(!is_Empty(&sequences_to_partion)){ 

		sequence curr_seq = dequeue(&sequences_to_partion) ; 
		const int current_nels = curr_seq.send - curr_seq.sstart + 1 ;
		int current_nwg = nwg ;
		const int current_nquarts = current_nels/4 ; 

		while(current_nwg*lws*4 > current_nels){
			current_nwg /= 2 ; 
		}

		if(!current_nwg) current_nwg++ ; 

		//printf("nwg : %d \n", current_nwg) ; 

		cl_event read_s1_evt ; 
		cl_event unmap_s1_evt ; 
		float* s1_arr = NULL;

		/*printf("curr_sstart : %d curr_send : %d curr_pivot : %f \n", curr_seq.sstart, curr_seq.send, curr_seq.pivot_value) ; 

		s1_arr = clEnqueueMapBuffer(resources->que, m.in, CL_TRUE,
				CL_MAP_READ | CL_MAP_WRITE, sizeof(cl_float)*curr_seq.sstart, sizeof(cl_float)*current_nels,
					0, NULL, &read_s1_evt , &err) ; 
		ocl_check(err, "read buffer out") ;

		print_vec(s1_arr, 0, current_nels -1) ; 

		err = clEnqueueUnmapMemObject(resources->que, m.in, s1_arr,
				1, &read_s1_evt, &unmap_s1_evt);
		ocl_check(err, "unmap buffer out");*/


        cl_event evt_split_elements = split_elements(resources->que, &k, &m, current_nels, curr_seq.sstart, lws, curr_seq.pivot_value, current_nwg) ; 

		clWaitForEvents(1, &evt_split_elements) ;

		cl_event read_evt_lt ;
		cl_event read_evt_gt ; 
  
		int* lt_cpu = NULL ;
		int* gt_cpu = NULL ; 
		
		lt_cpu = clEnqueueMapBuffer(resources->que, m.lt, CL_TRUE,
					CL_MAP_READ | CL_MAP_WRITE, 0, sizeof(cl_int)*(current_nwg),
						0, NULL, &read_evt_lt, &err) ; 
		ocl_check(err, "read buffer lt") ; 

		gt_cpu = clEnqueueMapBuffer(resources->que, m.gt, CL_TRUE,
					CL_MAP_READ | CL_MAP_WRITE, 0, sizeof(cl_int)*(current_nwg),
						0, NULL, &read_evt_gt, &err);
		ocl_check(err, "read buffer gt");

		scan(lt_cpu, current_nwg) ; 
		scan(gt_cpu, current_nwg) ;
		/*printf("lt :") ;  
		for(int i = 0 ; i < current_nwg ; i++) printf("%d ", lt_cpu[i]) ; 
		printf("\ngt:") ;
		for(int i = 0 ; i < current_nwg ; i++) printf("%d ", gt_cpu[i]) ; 
		printf("\n") ; */ 

		const int sum_lt = lt_cpu[current_nwg - 1] ;
		const int sum_gt = gt_cpu[current_nwg - 1] ; 

		cl_event unmap_evt_lt;
		err = clEnqueueUnmapMemObject(resources->que, m.lt, lt_cpu,
					1, &read_evt_lt, &unmap_evt_lt);
		ocl_check(err, "unmap lt");

		cl_event unmap_evt_gt;
		err = clEnqueueUnmapMemObject(resources->que, m.gt, gt_cpu,
					1, &read_evt_gt, &unmap_evt_gt);
		ocl_check(err, "unmap gt");


	    /*int* bit_map_sup_cpu = NULL ;
		int* bit_map_inf_cpu = NULL ;
		cl_event read_evt_bit_sup ; 
		cl_event read_evt_bit_inf ;   

		bit_map_sup_cpu = clEnqueueMapBuffer(resources->que, m.bit_map_sup, CL_TRUE,
					CL_MAP_READ | CL_MAP_WRITE, 0, sizeof(cl_int)*current_nels,
						0, NULL, &read_evt_bit_sup , &err) ; 
		ocl_check(err, "read buffer lt") ; 

		bit_map_inf_cpu = clEnqueueMapBuffer(resources->que, m.bit_map_inf, CL_TRUE,
					CL_MAP_READ | CL_MAP_WRITE, 0, sizeof(cl_int)*current_nels,
						0, NULL, &read_evt_bit_inf, &err);
		ocl_check(err, "read buffer gt");

		printf("bit map sup : ") ;
		for(int j = 0 ; j < current_nels ; j++) printf("%d ", bit_map_sup_cpu[j]) ; 
		printf("\n bit map inf : ") ;
		for(int j = 0 ; j < current_nels ; j++) printf("%d ", bit_map_inf_cpu[j]) ; 
		printf("\n") ; 

		cl_event unmap_evt_bit_sup;
		err = clEnqueueUnmapMemObject(resources->que, m.bit_map_sup, bit_map_sup_cpu,
					1, &read_evt_bit_sup, &unmap_evt_bit_sup);
		ocl_check(err, "unmap bit sup");

		cl_event unmap_evt_bit_inf;
		err = clEnqueueUnmapMemObject(resources->que, m.bit_map_inf, bit_map_inf_cpu,
					1, &read_evt_bit_inf, &unmap_evt_bit_inf);
		ocl_check(err, "unmap bit inf");*/

		cl_event scan_evt[3] ; 

		scan_evt[0] = scan_seq(resources->que, &k, &m,  m.bit_map_sup, m.bit_map_inf, round_mul_up(current_nels, 4), lws, current_nwg) ;
		scan_evt[1] = scan_seq(resources->que, &k, &m, m.tails_sup, m.tails_inf, round_mul_up(current_nwg, 4), lws, 1) ;
		scan_evt[2] = scan_seq_update(resources->que, &k, &m, round_mul_up(current_nels, 4), lws, current_nwg - 1) ;

		/*bit_map_sup_cpu = clEnqueueMapBuffer(resources->que, m.bit_map_sup, CL_TRUE,
					CL_MAP_READ | CL_MAP_WRITE, 0, sizeof(cl_int)*current_nels,
						0, NULL, &read_evt_bit_sup , &err) ; 
		ocl_check(err, "read buffer lt") ; 

		bit_map_inf_cpu = clEnqueueMapBuffer(resources->que, m.bit_map_inf, CL_TRUE,
					CL_MAP_READ | CL_MAP_WRITE, 0, sizeof(cl_int)*current_nels,
						0, NULL, &read_evt_bit_inf, &err);
		ocl_check(err, "read buffer gt");

		printf("bit map sup : ") ;
		for(int j = 0 ; j < current_nels ; j++) printf("%d ", bit_map_sup_cpu[j]) ; 
		printf("\n bit map inf : ") ;
		for(int j = 0 ; j < current_nels ; j++) printf("%d ", bit_map_inf_cpu[j]) ; 
		printf("\n") ; 

		err = clEnqueueUnmapMemObject(resources->que, m.bit_map_sup, bit_map_sup_cpu,
					1, &read_evt_bit_sup, &unmap_evt_bit_sup);
		ocl_check(err, "unmap bit sup");

		err = clEnqueueUnmapMemObject(resources->que, m.bit_map_inf, bit_map_inf_cpu,
					1, &read_evt_bit_inf, &unmap_evt_bit_inf);
		ocl_check(err, "unmap bit inf");  */

		clWaitForEvents(3, scan_evt) ; 

		cl_event partition_evt = partition(resources->que, &k,  &m, lt_cpu[current_nwg - 1], gt_cpu[current_nwg -1], curr_seq, current_nels, lws, current_nwg) ; 
		clWaitForEvents(1, &partition_evt) ;

		cl_event read_buf_out_evt ; 
		cl_event unmap_buf_out_evt; 
		float* out = clEnqueueMapBuffer(resources->que, m.buff_tmp, CL_TRUE,
						CL_MAP_READ , sizeof(cl_float)*curr_seq.sstart, sizeof(cl_float)*current_nels,
							0, NULL, &read_buf_out_evt , &err) ; 
			ocl_check(err, "read buffer out") ;

		//printf("tmp buf :\n") ; 
		//print_vec(out, 0, current_nels -1) ; 
	
		err = clEnqueueUnmapMemObject(resources->que,m.buff_tmp, out,
						1, &read_buf_out_evt, &unmap_buf_out_evt);
		ocl_check(err, "unmap buffer out");

		cl_event partition_copy_evt = partition_copy(resources->que, &k, &m, lt_cpu[current_nwg - 1], gt_cpu[current_nwg -1], curr_seq, current_nels, lws, current_nwg) ;
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

		//printf("lt : %d gt :%d \n", sum_lt, sum_gt) ; 

		//printf("part_sstart : %d part_send : %d part_pivot : %f \n", curr_seq.sstart, curr_seq.send, curr_seq.pivot_value) ; 

		/*s1_arr = clEnqueueMapBuffer(resources->que, m.in, CL_TRUE,
				CL_MAP_READ | CL_MAP_WRITE, sizeof(cl_float)*curr_seq.sstart, sizeof(cl_float)*current_nels,
					0, NULL, &read_s1_evt , &err) ; 
		ocl_check(err, "read buffer out") ;

		print_vec(s1_arr, 0, current_nels -1) ; 

		err = clEnqueueUnmapMemObject(resources->que, m.in, s1_arr,
				1, &read_s1_evt, &unmap_s1_evt);
		ocl_check(err, "unmap buffer out");*/

		if((s1_dim > 2*lws)){
			cl_event read_s1_evt ; 
			cl_event unmap_s1_evt ; 
			float* s1_arr = NULL;

			s1_arr = clEnqueueMapBuffer(resources->que, m.in, CL_TRUE,
					CL_MAP_READ | CL_MAP_WRITE, sizeof(cl_float)*s1.sstart, sizeof(cl_float)*s1_dim,
						0, NULL, &read_s1_evt , &err) ; 
			ocl_check(err, "read buffer out") ;

			s1.pivot_value = (float)s1_arr[rand()%s1_dim] ; 
			//printf("s1.sstart :%d s1.send :%d  s1_dim : %d\n", s1.sstart, s1.send, s1_dim) ; 
			//print_vec(s1_arr, 0, s1_dim - 1) ; 

			err = clEnqueueUnmapMemObject(resources->que, m.in, s1_arr,
					1, &read_s1_evt, &unmap_s1_evt);
			ocl_check(err, "unmap buffer out");

			enqueue(&sequences_to_partion, &s1) ; 
		}
		else  if(s1_dim > 1){
			cl_event read_s1_evt ; 
			cl_event unmap_s1_evt ; 
			float* s1_arr = NULL;

			s1_arr = clEnqueueMapBuffer(resources->que, m.in, CL_TRUE,
					CL_MAP_READ | CL_MAP_WRITE, sizeof(cl_float)*s1.sstart, sizeof(cl_float)*s1_dim,
						0, NULL, &read_s1_evt , &err) ; 
			ocl_check(err, "read buffer out") ;

			quicksort(s1_arr, 0, s1_dim - 1) ;

			err = clEnqueueUnmapMemObject(resources->que, m.in, s1_arr,
					1, &read_s1_evt, &unmap_s1_evt);
			ocl_check(err, "unmap buffer out");
		}

		if((s2_dim > 2*lws)){

			cl_event read_s2_evt ; 
			cl_event unmap_s2_evt ; 
			float* s2_arr = NULL;

			s2_arr = clEnqueueMapBuffer(resources->que, m.in, CL_TRUE,
					CL_MAP_READ | CL_MAP_WRITE, sizeof(cl_float)*s2.sstart, sizeof(cl_float)*s2_dim,
						0, NULL, &read_s2_evt , &err) ; 
			ocl_check(err, "read buffer out") ;

			s2.pivot_value  = (float)s2_arr[rand()%s2_dim] ;
		//	printf("s2.sstart :%d s2.send :%d s2_dim: %d\n", s2.sstart, s2.send, s2_dim) ; 
		//	print_vec(s2_arr, 0, s2_dim - 1) ; 

			err = clEnqueueUnmapMemObject(resources->que, m.in, s2_arr,
					1, &read_s2_evt, &unmap_s2_evt);
			ocl_check(err, "unmap buffer out") ; 
 
			enqueue(&sequences_to_partion, &s2) ;
		}
		else if(s2_dim > 1){
			cl_event read_s2_evt ; 
			cl_event unmap_s2_evt ; 
			float* s2_arr = NULL;

			s2_arr = clEnqueueMapBuffer(resources->que, m.in, CL_TRUE,
					CL_MAP_READ | CL_MAP_WRITE, sizeof(cl_float)*s2.sstart, sizeof(cl_float)*s2_dim,
						0, NULL, &read_s2_evt , &err) ; 
			ocl_check(err, "read buffer out") ;

			quicksort(s2_arr, 0, s2_dim - 1) ; 

			err = clEnqueueUnmapMemObject(resources->que, m.in, s2_arr,
					1, &read_s2_evt, &unmap_s2_evt);
			ocl_check(err, "unmap buffer out") ; 
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
		//sleep(1) ; 
    }  

	end = clock() ; 
	time_used = ((double)(end - start))/CLOCKS_PER_SEC ; 
	printf("total time :  %f\n", time_used) ; 

	cl_event read_out_evt ; 
	cl_event unmap_out_evt ; 
	float* out = NULL;
	float* out_copy ; 

	out = clEnqueueMapBuffer(resources->que, m.in, CL_TRUE,
				CL_MAP_READ | CL_MAP_WRITE, 0, sizeof(cl_float)*nels,
					0, NULL, &read_out_evt , &err) ; 
	ocl_check(err, "read buffer out") ;

	if(test_correctness){

		bench_mark(t, iteration, s, lws) ;

		float*vec_to_sort_on_cpu =  calloc(nels, sizeof(float)) ;

		copy_vec(vec, vec_to_sort_on_cpu, 0, nels - 1) ; 
		quicksort(vec_to_sort_on_cpu, 0, nels - 1) ; 
		check_result(out, vec_to_sort_on_cpu, nels) ;

		free(vec_to_sort_on_cpu) ; 

	}
	else{
		out_copy =  calloc(nels, sizeof(float)) ;
		copy_vec(vec, out_copy, 0, nels - 1) ; 
	}

	err = clEnqueueUnmapMemObject(resources->que, m.in, out,
				1, &read_out_evt, &unmap_out_evt);
		ocl_check(err, "unmap buffer out");

	release_queue(&sequences_to_partion) ; 	
		
	clReleaseMemObject(m.in) ;
	clReleaseMemObject(m.buff_tmp) ;
	clReleaseMemObject(m.gt) ;
	clReleaseMemObject(m.lt) ;
	clReleaseMemObject(m.bit_map_inf) ;
	clReleaseMemObject(m.bit_map_sup) ;
	clReleaseMemObject(m.tails_inf) ;
	clReleaseMemObject(m.tails_sup) ; 

	clReleaseKernel(k.splitting_elements) ; 
	clReleaseKernel(k.partitioning) ;
	clReleaseKernel(k.scan_gpu) ;
	clReleaseKernel(k.scan_update) ;

	return out_copy ; 

}