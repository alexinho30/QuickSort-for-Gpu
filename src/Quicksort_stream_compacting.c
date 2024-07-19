#include "../include/Quicksort_stream_compacting.h"
#include<math.h>
#include <stdbool.h>
#include "../include/quicksort_cpu_version.h"
#define MAX_NUM_SEQ 1000000 

cl_event split_elements(cl_command_queue q, cl_kernel splitting_elements,
		cl_mem d_buf, cl_mem lt, cl_mem gt, cl_mem bit_map_sup, cl_mem bit_map_inf, cl_int nels, cl_int sstart, cl_int lws_, cl_int pivot, const int nwg){
	size_t lws[] = {lws_} ; 
	size_t gws[] = {nwg*lws[0]};
	 
	cl_int err ; 
	cl_event evt_split_elements ;

	err = clSetKernelArg(splitting_elements, 0, sizeof(cl_int), &nels) ;
	ocl_check(err, "set kernel nels ") ;

	err = clSetKernelArg(splitting_elements, 1, sizeof(cl_int), &sstart) ;
	ocl_check(err, "set kernel sstart ") ;

	err = clSetKernelArg(splitting_elements, 2, sizeof(d_buf), &d_buf) ;
	ocl_check(err, "set kernel d_buf") ;

	err = clSetKernelArg(splitting_elements, 3, sizeof(pivot), &pivot) ;
	ocl_check(err, "set kernel pivot") ;

	err = clSetKernelArg(splitting_elements, 4, sizeof(lt), &lt) ;
	ocl_check(err, "set kernel lt") ;

	err = clSetKernelArg(splitting_elements, 5, sizeof(gt), &gt) ;
	ocl_check(err, "set kernel gt") ;

	err = clSetKernelArg(splitting_elements, 6, sizeof(bit_map_sup), &bit_map_sup) ;
	ocl_check(err, "set kernel bit_array_sup") ;

	err = clSetKernelArg(splitting_elements, 7, sizeof(bit_map_inf), &bit_map_inf) ;
	ocl_check(err, "set kernel bit_array_inf") ;

	err = clSetKernelArg(splitting_elements, 8, sizeof(cl_int)*lws[0], NULL) ;
	ocl_check(err, "set kernel local mem inf") ;

	err = clSetKernelArg(splitting_elements, 9, sizeof(cl_int)*lws[0], NULL) ;
	ocl_check(err, "set kernel local mem sup") ;

	err = clEnqueueNDRangeKernel(q, splitting_elements, 1, NULL, gws, lws, 0, NULL, &evt_split_elements) ;
    ocl_check(err, "enqueue split elements kernel") ; 

    return evt_split_elements;
}

cl_event scan_seq(cl_command_queue q, cl_kernel scan,cl_mem bit_map_sup, cl_mem bit_map_inf, 
cl_mem tails_sup, cl_mem tails_inf, cl_int nels, cl_int lws_, const int nwg){

	cl_int err ; 
	cl_event scan_evt ; 

	size_t lws[] = {lws_} ; 
	size_t gws[] = {nwg*lws[0]} ; 

	err = clSetKernelArg(scan, 0, sizeof(cl_int), &nels) ;
	ocl_check(err, "set kernel nels ") ;

	err = clSetKernelArg(scan, 1, sizeof(bit_map_sup), &bit_map_sup) ;
	ocl_check(err, "set kernel d_buf") ;

	err = clSetKernelArg(scan, 2, sizeof(bit_map_inf), &bit_map_inf) ;
	ocl_check(err, "set kernel d_buf") ;

	err = clSetKernelArg(scan, 3, sizeof(tails_inf), &tails_inf) ;
	ocl_check(err, "set kernel lt") ;

	err = clSetKernelArg(scan, 4, sizeof(tails_sup), &tails_sup) ;
	ocl_check(err, "set kernel gt") ;

	err = clSetKernelArg(scan, 5, sizeof(cl_int)*lws[0], NULL) ;
	ocl_check(err, "set kernel local mem inf") ;

	err = clSetKernelArg(scan, 6, sizeof(cl_int)*lws[0], NULL) ;
	ocl_check(err, "set kernel local mem sup") ;

	err = clEnqueueNDRangeKernel(q, scan, 1, NULL, gws, lws, 0, NULL, &scan_evt) ;
    ocl_check(err, "enqueue scan seq kernel") ; 

    return scan_evt; 

}

cl_event scan_seq_update(cl_command_queue q, cl_kernel scan_update, cl_mem bit_map_sup, cl_mem bit_map_inf, 
cl_mem tails_sup, cl_mem tails_inf, cl_int nels, cl_int lws_, const int nwg){

	cl_int err ; 
	cl_event scan_evt ; 

	size_t lws[] = {lws_} ; 
	size_t gws[] = {nwg*lws[0]} ; 

	err = clSetKernelArg(scan_update, 0, sizeof(cl_int), &nels) ;
	ocl_check(err, "set kernel nels ") ;

	err = clSetKernelArg(scan_update, 1, sizeof(bit_map_sup), &bit_map_sup) ;
	ocl_check(err, "set kernel d_buf") ;

	err = clSetKernelArg(scan_update, 2, sizeof(bit_map_inf), &bit_map_inf) ;
	ocl_check(err, "set kernel d_buf") ;

	err = clSetKernelArg(scan_update, 3, sizeof(tails_sup), &tails_sup) ;
	ocl_check(err, "set kernel gt") ;

	err = clSetKernelArg(scan_update, 4, sizeof(tails_inf), &tails_inf) ;
	ocl_check(err, "set kernel lt") ;

	err = clEnqueueNDRangeKernel(q, scan_update, 1, NULL, gws, lws, 0, NULL, &scan_evt) ;
    ocl_check(err, "enqueue scan seq update kernel") ; 

    return scan_evt; 

}

cl_event partition(cl_command_queue q, cl_kernel partitioning, cl_mem in, cl_mem out, cl_mem bit_map_inf, 
cl_mem bit_map_sup,cl_int lt, cl_int gt, sequence seq, const int nels, const int lws_, const int nwg){

	cl_int err ; 
	cl_event partition_evt ; 

	size_t lws[] = {lws_} ; 
	size_t gws[] = {nwg*lws[0]} ; 

	err = clSetKernelArg(partitioning, 0, sizeof(cl_int), &nels) ;
	ocl_check(err, "set kernel nels ") ;

	err = clSetKernelArg(partitioning, 1, sizeof(sequence), &seq) ;
	ocl_check(err, "set kernel seq ") ;

	err = clSetKernelArg(partitioning, 2, sizeof(in), &in) ;
	ocl_check(err, "set kernel in") ;

	err = clSetKernelArg( partitioning, 3, sizeof(out), &out) ;
	ocl_check(err, "set kernel out") ;

	err = clSetKernelArg(partitioning, 4, sizeof(bit_map_sup), &bit_map_sup) ;
	ocl_check(err, "set kernel bit_map_sup") ;

	err = clSetKernelArg(partitioning, 5, sizeof(bit_map_inf), &bit_map_inf) ;
	ocl_check(err, "set kernel bit_map_inf") ;

	err = clSetKernelArg(partitioning, 6, sizeof(cl_int), &lt) ;
	ocl_check(err, "set kernel lt") ;

	err = clSetKernelArg(partitioning, 7, sizeof(cl_int), &gt) ;
	ocl_check(err, "set kernel gt") ;

	err = clEnqueueNDRangeKernel(q,  partitioning, 1, NULL, gws, lws, 0, NULL, &partition_evt) ;
    ocl_check(err, "enqueue partition kernel") ; 

    return partition_evt; 
}

cl_event partition_copy(cl_command_queue que, cl_kernel partition_copy, cl_mem in, cl_mem out, const int lt, const int gt, const sequence seq, const int nels, const int lws_, const int nwg){

	cl_int err ; 
	cl_event partition_evt ; 

	size_t lws[] = {lws_} ; 
	size_t gws[] = {nwg*lws[0]} ; 

	err = clSetKernelArg(partition_copy, 0, sizeof(cl_int), &nels) ;
	ocl_check(err, "set kernel nels ") ;

	err = clSetKernelArg(partition_copy, 1, sizeof(sequence), &seq) ;
	ocl_check(err, "set kernel seq ") ;

	err = clSetKernelArg(partition_copy, 2, sizeof(in), &in) ;
	ocl_check(err, "set kernel in") ;

	err = clSetKernelArg(partition_copy, 3, sizeof(out), &out) ;
	ocl_check(err, "set kernel out") ;

	err = clSetKernelArg(partition_copy, 4, sizeof(cl_int), &lt) ;
	ocl_check(err, "set kernel lt") ;

	err = clSetKernelArg(partition_copy, 5, sizeof(cl_int), &gt) ;
	ocl_check(err, "set kernel gt") ;

	err = clEnqueueNDRangeKernel(que,  partition_copy, 1, NULL, gws, lws, 0, NULL, &partition_evt) ;
    ocl_check(err, "enqueue partition copy kernel") ; 

    return partition_evt; 

}

void quickSortGpu(const int* vec,  const int nels, const int lws, const int nwg_cu, cl_resources* resources){

	if(resources == NULL){
		fprintf(stderr, "reosurces are set to null	\n") ; 
		exit(EXIT_FAILURE) ; 
	}

	int*vec_copy =  calloc(nels, sizeof(int)) ;
	copy_vec(vec, vec_copy, nels) ; 
	quicksort(vec_copy, 0, nels - 1) ;

	time_t start, end ; 
	double time_used ; 
	start = clock() ; 

	cl_uint ncu = 0;
	cl_int err ;

	err = clGetDeviceInfo(resources->d, CL_DEVICE_MAX_COMPUTE_UNITS, sizeof(ncu), &ncu, NULL);
	cl_uint nwg = ncu*nwg_cu;
	printf("Nwg: %d*%d = %d\n", nwg_cu, ncu, nwg);

	srand(21) ; 

	cl_kernel splitting_elements;
	cl_kernel scan_gpu ; 
	cl_kernel scan_update ;
	cl_kernel partitioning ;
	cl_kernel partitioning_copy ;   

	splitting_elements = clCreateKernel(resources->prog, "split_elements", &err);	
	ocl_check(err, "create kernel split_elements");
	scan_gpu = clCreateKernel(resources->prog, "scan", &err);	
	ocl_check(err, "create kernel scan");
	scan_update = clCreateKernel(resources->prog, "scan_update", &err);	
	ocl_check(err, "create kernel scan_update");
	partitioning = clCreateKernel(resources->prog, "partition_buff_tmp", &err);	
	ocl_check(err, "create kernel partition_buff_tmp");
	partitioning_copy = clCreateKernel(resources->prog, "partition_copy", &err);	
	ocl_check(err, "create kernel partition_buff_tmp");

	cl_mem in = clCreateBuffer(resources->ctx, CL_MEM_READ_WRITE | CL_MEM_COPY_HOST_PTR, (nels)*sizeof(cl_int), (void*)vec, &err); 
	ocl_check(err, "create buffer d_buf");
	cl_mem buff_tmp = clCreateBuffer(resources->ctx, CL_MEM_READ_WRITE | CL_MEM_ALLOC_HOST_PTR , (nels)*sizeof(cl_int), NULL, &err);
	ocl_check(err, "create buffer lt");
	cl_mem lt = clCreateBuffer(resources->ctx, CL_MEM_READ_WRITE | CL_MEM_ALLOC_HOST_PTR , (nwg)*sizeof(cl_int), NULL, &err);
	ocl_check(err, "create buffer lt");
	cl_mem gt = clCreateBuffer(resources->ctx, CL_MEM_READ_WRITE | CL_MEM_ALLOC_HOST_PTR, (nwg)*sizeof(cl_int), NULL, &err);
	ocl_check(err, "create buffer gt"); 
	cl_mem bit_map_sup = clCreateBuffer(resources->ctx, CL_MEM_READ_WRITE | CL_MEM_ALLOC_HOST_PTR , nels*sizeof(cl_int), NULL, &err);
	ocl_check(err, "create buffer bit map sup");
	cl_mem bit_map_inf = clCreateBuffer(resources->ctx, CL_MEM_READ_WRITE | CL_MEM_ALLOC_HOST_PTR , nels*sizeof(cl_int), NULL, &err);
	ocl_check(err, "create buffer bit map inf");
	cl_mem tails_inf = clCreateBuffer(resources->ctx, CL_MEM_READ_WRITE | CL_MEM_ALLOC_HOST_PTR , nwg*sizeof(cl_int), NULL, &err);
	ocl_check(err, "create buffer bit map inf");
	cl_mem tails_sup = clCreateBuffer(resources->ctx, CL_MEM_READ_WRITE | CL_MEM_ALLOC_HOST_PTR , nwg*sizeof(cl_int), NULL, &err);
	ocl_check(err, "create buffer bit map inf");
   
    Queue sequences_to_partion ;
    initQueue(&sequences_to_partion, MAX_NUM_SEQ) ;

    sequence start_sequence ; 
    start_sequence.sstart = 0 ; 
    start_sequence.send = nels - 1 ; 
	start_sequence.pivot_value = vec[rand()%nels] ; 
	enqueue(&sequences_to_partion, &start_sequence) ;  

    while(!is_Empty(&sequences_to_partion)){ 

		sequence curr_seq = dequeue(&sequences_to_partion) ; 
		const int current_nels = curr_seq.send - curr_seq.sstart + 1 ;
		int current_nwg = nwg ; 

		while(current_nwg*lws > current_nels){
			current_nwg /= 2 ; 
		}

        cl_event evt_split_elements = split_elements(resources->que, splitting_elements, in, lt, gt, bit_map_sup, bit_map_inf, current_nels, curr_seq.sstart, lws, curr_seq.pivot_value, current_nwg) ;  

		clWaitForEvents(1, &evt_split_elements) ;

		cl_event read_evt_lt ;
		cl_event read_evt_gt ; 
  
		int* lt_cpu = NULL ;
		int* gt_cpu = NULL ;  
		
		lt_cpu = clEnqueueMapBuffer(resources->que, lt, CL_TRUE,
					CL_MAP_READ | CL_MAP_WRITE, 0, sizeof(cl_int)*(current_nwg),
						0, NULL, &read_evt_lt, &err) ; 
		ocl_check(err, "read buffer lt") ; 

		gt_cpu = clEnqueueMapBuffer(resources->que, gt, CL_TRUE,
					CL_MAP_READ | CL_MAP_WRITE, 0, sizeof(cl_int)*(current_nwg),
						0, NULL, &read_evt_gt, &err);
		ocl_check(err, "read buffer gt");

		scan(lt_cpu, current_nwg) ; 
		scan(gt_cpu, current_nwg) ; 

		const int sum_lt = lt_cpu[current_nwg - 1] ;
		const int sum_gt = gt_cpu[current_nwg - 1] ;  

		cl_event unmap_evt_lt;
		err = clEnqueueUnmapMemObject(resources->que, lt, lt_cpu,
					1, &read_evt_lt, &unmap_evt_lt);
		ocl_check(err, "unmap lt");

		cl_event unmap_evt_gt;
		err = clEnqueueUnmapMemObject(resources->que, gt, gt_cpu,
					1, &read_evt_gt, &unmap_evt_gt);
		ocl_check(err, "unmap gt");

		cl_event scan_evt[3] ; 

		scan_evt[0] = scan_seq(resources->que, scan_gpu, bit_map_sup, bit_map_inf, tails_sup, tails_inf, current_nels, lws, current_nwg) ;
		scan_evt[1] = scan_seq(resources->que, scan_gpu, tails_sup, tails_inf, tails_sup, tails_inf, current_nwg, lws, 1) ;  
		scan_evt[2] = scan_seq_update(resources->que, scan_update, bit_map_sup, bit_map_inf, tails_sup, tails_inf, current_nels, lws, current_nwg - 1) ;  

		clWaitForEvents(3, scan_evt) ; 

		cl_event partion_evt = partition(resources->que, partitioning, in, buff_tmp, bit_map_inf, bit_map_sup,  lt_cpu[current_nwg - 1], gt_cpu[current_nwg -1], curr_seq, current_nels, lws, current_nwg) ; 
		clWaitForEvents(1, &partion_evt) ;
		cl_event partion_evt_1 = partition_copy(resources->que, partitioning_copy, in, buff_tmp, lt_cpu[current_nwg - 1], gt_cpu[current_nwg -1], curr_seq, current_nels, lws, current_nwg) ;
		clWaitForEvents(1, &partion_evt_1) ; 

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

		if((s1_dim > 2*lws)){
			cl_event read_s1_evt ; 
			cl_event unmap_s1_evt ; 
			int* s1_arr = NULL;

			s1_arr = clEnqueueMapBuffer(resources->que, in, CL_TRUE,
					CL_MAP_READ | CL_MAP_WRITE, sizeof(cl_int)*s1.sstart, sizeof(cl_int)*s1_dim,
						0, NULL, &read_s1_evt , &err) ; 
			ocl_check(err, "read buffer out") ;

			s1.pivot_value = s1_arr[rand()%s1_dim] ; 

			err = clEnqueueUnmapMemObject(resources->que, in, s1_arr,
					1, &read_s1_evt, &unmap_s1_evt);
			ocl_check(err, "unmap buffer out");

			enqueue(&sequences_to_partion, &s1) ; 
		}
		else  if(s1_dim > 1){
			cl_event read_s1_evt ; 
			cl_event unmap_s1_evt ; 
			int* s1_arr = NULL;

			s1_arr = clEnqueueMapBuffer(resources->que, in, CL_TRUE,
					CL_MAP_READ | CL_MAP_WRITE, sizeof(cl_int)*s1.sstart, sizeof(cl_int)*s1_dim,
						0, NULL, &read_s1_evt , &err) ; 
			ocl_check(err, "read buffer out") ;

			quicksort(s1_arr, 0, s1_dim - 1) ;

			err = clEnqueueUnmapMemObject(resources->que, in, s1_arr,
					1, &read_s1_evt, &unmap_s1_evt);
			ocl_check(err, "unmap buffer out");
		}

		if((s2_dim > 2*lws)){

			cl_event read_s2_evt ; 
			cl_event unmap_s2_evt ; 
			int* s2_arr = NULL;

			s2_arr = clEnqueueMapBuffer(resources->que, in, CL_TRUE,
					CL_MAP_READ | CL_MAP_WRITE, sizeof(cl_int)*s2.sstart, sizeof(cl_int)*s2_dim,
						0, NULL, &read_s2_evt , &err) ; 
			ocl_check(err, "read buffer out") ;

			s2.pivot_value  = s2_arr[rand()%s2_dim] ;

			err = clEnqueueUnmapMemObject(resources->que, in, s2_arr,
					1, &read_s2_evt, &unmap_s2_evt);
			ocl_check(err, "unmap buffer out") ; 
 
			enqueue(&sequences_to_partion, &s2) ;
		}
		else if(s2_dim > 1){
			cl_event read_s2_evt ; 
			cl_event unmap_s2_evt ; 
			int* s2_arr = NULL;

			s2_arr = clEnqueueMapBuffer(resources->que, in, CL_TRUE,
					CL_MAP_READ | CL_MAP_WRITE, sizeof(cl_int)*s2.sstart, sizeof(cl_int)*s2_dim,
						0, NULL, &read_s2_evt , &err) ; 
			ocl_check(err, "read buffer out") ;

			quicksort(s2_arr, 0, s2_dim - 1) ; 

			err = clEnqueueUnmapMemObject(resources->que, in, s2_arr,
					1, &read_s2_evt, &unmap_s2_evt);
			ocl_check(err, "unmap buffer out") ; 
		}
    } 

	end = clock() ; 
	time_used = ((double)(end - start))/CLOCKS_PER_SEC ; 
	printf("total time :  %f\n", time_used) ; 

	cl_event read_out_evt ; 
	cl_event unmap_out_evt ; 
	int* out = NULL;

	out = clEnqueueMapBuffer(resources->que, in, CL_TRUE,
			CL_MAP_READ | CL_MAP_WRITE, 0, sizeof(cl_int)*nels,
				0, NULL, &read_out_evt , &err) ; 
	ocl_check(err, "read buffer out") ;

	check_result(out, vec_copy, nels) ; 

	err = clEnqueueUnmapMemObject(resources->que, in, out,
			1, &read_out_evt, &unmap_out_evt);
	ocl_check(err, "unmap buffer out");

	release_queue(&sequences_to_partion) ; 	
		
	clReleaseMemObject(in) ;
	clReleaseMemObject(buff_tmp) ;
	clReleaseMemObject(gt) ;
	clReleaseMemObject(lt) ;
	clReleaseMemObject(bit_map_inf) ;
	clReleaseMemObject(bit_map_sup) ;
	clReleaseMemObject(tails_inf) ;
	clReleaseMemObject(tails_sup) ; 

	clReleaseKernel(splitting_elements) ; 
	clReleaseKernel(partitioning) ;
	clReleaseKernel(scan_gpu) ;
	clReleaseKernel(scan_update) ;

	return ; 	
}


/*s1.pivot_value = median_computation(que, d_buf, gt, lt, bit_map_inf, bit_map_sup, tails_inf, tails_sup, 
	                                                   splitting_elements, scan_gpu, scan_update, partitioning, s1.sstart, s1.send, 1 - idx_buf, lws, nwg, nels) ; 
*/

/*start_sequence.pivot_value = median_computation(que, d_buf, gt, lt, bit_map_inf, bit_map_sup, tails_inf, tails_sup, 
	                                                   splitting_elements, scan_gpu, scan_update, partitioning, 0, nels -1, idx_buf, lws, nwg, nels) ; 
*/



/*	int* bit_map_sup_cpu = NULL ;
		int* bit_map_inf_cpu = NULL ;
		cl_event read_evt_bit_sup ; 
		cl_event read_evt_bit_inf ;   

		bit_map_sup_cpu = clEnqueueMapBuffer(que, bit_map_sup, CL_TRUE,
					CL_MAP_READ | CL_MAP_WRITE, 0, sizeof(cl_int)*current_nels,
						0, NULL, &read_evt_bit_sup , &err) ; 
		ocl_check(err, "read buffer lt") ; 

		bit_map_inf_cpu = clEnqueueMapBuffer(que, bit_map_inf, CL_TRUE,
					CL_MAP_READ | CL_MAP_WRITE, 0, sizeof(cl_int)*current_nels,
						0, NULL, &read_evt_bit_inf, &err);
		ocl_check(err, "read buffer gt");

		printf("bit map sup : ") ;
		for(int j = 0 ; j < current_nels ; j++) printf("%d ", bit_map_sup_cpu[j]) ; 
		printf("\n bit map inf : ") ;
		for(int j = 0 ; j < current_nels ; j++) printf("%d ", bit_map_inf_cpu[j]) ; 
		printf("\n") ; 

		cl_event unmap_evt_bit_sup;
		err = clEnqueueUnmapMemObject(que, bit_map_sup, bit_map_sup_cpu,
					1, &read_evt_bit_sup, &unmap_evt_bit_sup);
		ocl_check(err, "unmap bit sup");

		cl_event unmap_evt_bit_inf;
		err = clEnqueueUnmapMemObject(que, bit_map_inf, bit_map_inf_cpu,
					1, &read_evt_bit_inf, &unmap_evt_bit_inf);
		ocl_check(err, "unmap bit inf");*/


/*
	int* bit_map_sup_cpu_ = NULL ;
		int* bit_map_inf_cpu_ = NULL ;
		cl_event read_evt_bit_sup_ ; 
		cl_event read_evt_bit_inf_ ;   

		bit_map_sup_cpu_ = clEnqueueMapBuffer(que, bit_map_sup, CL_TRUE,
					CL_MAP_READ | CL_MAP_WRITE, 0, sizeof(cl_int)*current_nels,
						0, NULL, &read_evt_bit_sup_ , &err) ; 
		ocl_check(err, "read buffer lt") ; 

		bit_map_inf_cpu_ = clEnqueueMapBuffer(que, bit_map_inf, CL_TRUE,
					CL_MAP_READ | CL_MAP_WRITE, 0, sizeof(cl_int)*current_nels,
						0, NULL, &read_evt_bit_inf_, &err);
		ocl_check(err, "read buffer gt");

		printf("bit map sup : ") ;
		for(int j = 0 ; j < current_nels ; j++) printf("%d ", bit_map_sup_cpu_[j]) ; 
		printf("\n bit map inf : ") ;
		for(int j = 0 ; j < current_nels ; j++) printf("%d ", bit_map_inf_cpu_[j]) ; 
		printf("\n") ; 

		cl_event unmap_evt_bit_sup_;
		err = clEnqueueUnmapMemObject(que, bit_map_sup, bit_map_sup_cpu_,
					1, &read_evt_bit_sup_, &unmap_evt_bit_sup_);
		ocl_check(err, "unmap bit sup");

		cl_event unmap_evt_bit_inf_;
		err = clEnqueueUnmapMemObject(que, bit_map_inf, bit_map_inf_cpu_,
					1, &read_evt_bit_inf_, &unmap_evt_bit_inf_);
		ocl_check(err, "unmap bit inf");
*/