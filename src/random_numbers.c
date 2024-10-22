#include "../include/random_numbers.h"

int log_b2(int val){
    int cont = 0 ; 
    int tmp_val = val ; 
    while(tmp_val >= 1){
        tmp_val = tmp_val/2 ; 
        cont++ ; 
    }
    return cont ; 
}

int random_uniform_value(const int start, const int end){
    const int n_range = end - start + 1 ;
    const int n_range_bit = (int)log_b2(n_range) ; 

    int random_value = rand() ;
    int random_uniform_value = 0 ;  

    for(int i = 0 ; i < n_range_bit ; i++){
        random_uniform_value <<= 1 ; 
        random_uniform_value |= random_value & 1 ; 
        random_value >>= 1 ; 
        if(random_uniform_value >= n_range){
            random_uniform_value >>= 1 ; 
            break ; 
        }
    }

    return random_uniform_value + start ; 
}

float random_uniform_zero_one(){
    return  ((double)(rand()) + 1. )/( (double)(RAND_MAX) + 1. );
}

cl_event init_vec(cl_resources* resources, cl_kernel init_array, device_memory* m, const cl_int nels, const cl_float p1, const cl_float p2, const int lws_ ){
    cl_int err ; 
	cl_event init_evt ; 

	size_t lws[] = {lws_} ; 
	size_t gws[] = {round_mul_up(nels, lws_)} ;

    err = clSetKernelArg(init_array, 0, sizeof(m->out), &m->out) ;
	ocl_check(err, "set kernel out ") ;

    err = clSetKernelArg(init_array, 1, sizeof(m->rand_values), &m->rand_values) ;
	ocl_check(err, "set kernel out ") ;

	err = clSetKernelArg(init_array, 2, sizeof(cl_int), &nels) ;
	ocl_check(err, "-set kernel nels ") ;

	err = clSetKernelArg(init_array, 3, sizeof(cl_float), &p1) ;
	ocl_check(err, "set kernel p1") ;

	err = clSetKernelArg(init_array, 4, sizeof(cl_float), &p2) ;
    ocl_check(err, "set kernel p2") ;

    err = clEnqueueNDRangeKernel(resources->que,  init_array, 1, NULL, gws, lws, 0, NULL, &init_evt) ;
    ocl_check(err, "enqueue partition copy kernel") ; 

    return init_evt; 
}


void distr_arr(cl_resources* resources, float* vec, const cl_int nels, const cl_float mu, const cl_float sigma, const int lws, const bool norm){

    if( resources == NULL){
        fprintf(stderr, "resources arre null\n") ; 
        exit(EXIT_FAILURE) ; 
    }

    cl_int err ;

    float* rand_values_cpu = calloc(nels, sizeof(cl_float)) ; 

    for(int i = 0 ; i < nels ; i++){
        rand_values_cpu[i] = random_uniform_zero_one() ; 
    } 

    device_memory m ; 

    m.out = clCreateBuffer(resources->ctx, CL_MEM_READ_WRITE | CL_MEM_ALLOC_HOST_PTR, sizeof(cl_float)*nels, NULL, &err) ; 
    ocl_check(err, "create buffer out") ; 
    m.rand_values = clCreateBuffer(resources->ctx, CL_MEM_READ_WRITE | CL_MEM_COPY_HOST_PTR, sizeof(cl_float)*nels, (void*)rand_values_cpu, &err) ; 
    ocl_check(err, "create buffer out") ; 

     cl_kernel init_array ; 

    if(norm){
        init_array = clCreateKernel(resources->prog, "init_vec_norm", &err) ;
        ocl_check(err, "create kernel init_vec_norm"); 
    }
    else{
        init_array = clCreateKernel(resources->prog, "init_vec_uniform", &err) ; 
        ocl_check(err, "create kernel init_vec_uniform"); 
    }
    
    cl_event init_evt = init_vec(resources, init_array, &m, nels, mu, sigma, lws) ;

    clWaitForEvents(1, &init_evt) ; 

    double init_evt_ms = runtime_ms(init_evt) ; 

    printf("random values array created...\n") ;
    printf("init_evt : %.4g ms %.4g GE/s %.4g GB/s\n", init_evt_ms, (2*nels)/init_evt_ms/(1.0e6), (2*nels*sizeof(int))/init_evt_ms/(1.0e6)) ;  

    cl_event read_pivot_s1_evt ; 
    cl_event unmap_pivot_s1_evt ; 
    float* arr_gpu = NULL;

    arr_gpu = clEnqueueMapBuffer(resources->que, m.out, CL_TRUE,
            CL_MAP_READ, 0, sizeof(cl_float)*nels,
                0, NULL, & read_pivot_s1_evt , &err) ; 
    ocl_check(err, "read arr_gpu") ;

    copy_vec(arr_gpu, vec, 0, nels - 1) ;  

    err = clEnqueueUnmapMemObject(resources->que, m.out, arr_gpu,
            1, &read_pivot_s1_evt , &unmap_pivot_s1_evt);
    ocl_check(err, "unmap arr_gpu");

    free(rand_values_cpu) ; 
    clReleaseKernel(init_array) ;  
    clReleaseMemObject(m.rand_values) ; 
    clReleaseMemObject(m.out) ; 

    return ; 

}