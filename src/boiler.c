#define CL_TARGET_OPENCL_VERSION 120

#include "../include/boiler.h"

void ocl_check(cl_int err, const char *msg, ...) {
	if (err != CL_SUCCESS) {
		char msg_buf[BUFSIZE + 1];
		va_list ap;
		va_start(ap, msg);
		vsnprintf(msg_buf, BUFSIZE, msg, ap);
		va_end(ap);
		msg_buf[BUFSIZE] = '\0';
		fprintf(stderr, "%s - error %d\n", msg_buf, err);
		exit(1);
	}
}


cl_platform_id select_platform() {
	cl_uint nplats;
	cl_int err;
	cl_platform_id *plats;
	const char * const env = getenv("OCL_PLATFORM");
	cl_uint nump = 0;
	if (env && env[0] != '\0')
		nump = atoi(env);

	err = clGetPlatformIDs(0, NULL, &nplats);
	ocl_check(err, "counting platforms");

	printf("number of platforms: %u\n", nplats);

	plats = (cl_platform_id *)malloc(nplats*sizeof(*plats));

	err = clGetPlatformIDs(nplats, plats, NULL);
	ocl_check(err, "getting platform IDs");

	if (nump >= nplats) {
		fprintf(stderr, "no platform number %u", nump);
		exit(1);
	}

	cl_platform_id choice = plats[nump];

	char buffer[BUFSIZE];

	err = clGetPlatformInfo(choice, CL_PLATFORM_NAME, BUFSIZE,
		buffer, NULL);
	ocl_check(err, "getting platform name");

	printf("selected platform %d: %s\n", nump, buffer);

	return choice;
}


cl_device_id select_device(cl_platform_id p)
{
	cl_uint ndevs;
	cl_int err;
	cl_device_id *devs;
	const char * const env = getenv("OCL_DEVICE");
	cl_uint numd = 0;
	if (env && env[0] != '\0')
		numd = atoi(env);

	err = clGetDeviceIDs(p, CL_DEVICE_TYPE_ALL, 0, NULL, &ndevs);
	ocl_check(err, "counting devices");

	printf("number of devices: %u\n", ndevs);

	devs = (cl_device_id *)malloc(ndevs*sizeof(*devs));

	err = clGetDeviceIDs(p, CL_DEVICE_TYPE_ALL, ndevs, devs, NULL);
	ocl_check(err, "devices #2");

	if (numd >= ndevs) {
		fprintf(stderr, "no device number %u", numd);
		exit(1);
	}

	cl_device_id choice = devs[numd];

	char buffer[BUFSIZE];

	err = clGetDeviceInfo(choice, CL_DEVICE_NAME, BUFSIZE,
		buffer, NULL);
	ocl_check(err, "device name");

	printf("selected device %d: %s\n", numd, buffer);

	return choice;
}


cl_context create_context(cl_platform_id p, cl_device_id d)
{
	cl_int err;

	cl_context_properties ctx_prop[] = {
		CL_CONTEXT_PLATFORM, (cl_context_properties)p, 0
	};

	cl_context ctx = clCreateContext(ctx_prop, 1, &d,
		NULL, NULL, &err);
	ocl_check(err, "create context");

	return ctx;
}


cl_command_queue create_queue(cl_context ctx, cl_device_id d)
{
	cl_int err;

	cl_command_queue que = clCreateCommandQueue(ctx, d,
		CL_QUEUE_PROFILING_ENABLE, &err); 
	ocl_check(err, "create queue");
	return que;
}

cl_program create_program(const char * const fname, cl_context ctx,
	cl_device_id dev)
{
	cl_int err, errlog;
	cl_program prg;

	char src_buf[BUFSIZE + 1];
	char *log_buf = NULL;
	size_t logsize;
	const char* buf_ptr = src_buf;
	time_t now = time(NULL);

	memset(src_buf, 0, BUFSIZE);

	snprintf(src_buf, BUFSIZE, "// %s#include \"%s\"\n",
		ctime(&now), fname);
	printf("compiling:\n%s", src_buf);
	prg = clCreateProgramWithSource(ctx, 1, &buf_ptr, NULL, &err);
	ocl_check(err, "create program");

	err = clBuildProgram(prg, 1, &dev, "-I.", NULL, NULL);
	errlog = clGetProgramBuildInfo(prg, dev, CL_PROGRAM_BUILD_LOG,
		0, NULL, &logsize);
	ocl_check(errlog, "get program build log size");
	log_buf =(char*) malloc(logsize);
	errlog = clGetProgramBuildInfo(prg, dev, CL_PROGRAM_BUILD_LOG,
		logsize, log_buf, NULL);
	ocl_check(errlog, "get program build log");
	while (logsize > 0 &&
		(log_buf[logsize-1] == '\n' ||
		 log_buf[logsize-1] == '\0')) {
		logsize--;
	}
	if (logsize > 0) {
		log_buf[logsize] = '\n';
		log_buf[logsize+1] = '\0';
	} else {
		log_buf[logsize] = '\0';
	}
	printf("=== BUILD LOG ===\n%s\n=========\n", log_buf);
	ocl_check(err, "build program");

	return prg;
}


void create_resources(cl_resources* resources, const char *const  ocl_filename){
	resources->p = select_platform() ; 
	resources->d = select_device(resources->p) ;
	resources->ctx = create_context(resources->p, resources->d) ; 
	resources->que = create_queue(resources->ctx, resources->d) ;
	resources->prog = create_program(ocl_filename, resources->ctx, resources->d) ; 
}

void release_resources(cl_resources* resources){
	cl_int err ; 

	err = clReleaseCommandQueue(resources->que) ;
	ocl_check(err, "release commandQueue") ; 

	err = clReleaseContext(resources->ctx) ;
	ocl_check(err, "release contex") ;  

	err = clReleaseDevice(resources->d) ;
	ocl_check(err, "release device") ;  

	err = clReleaseProgram(resources->prog) ;
	ocl_check(err, "release program") ;   
}


cl_ulong runtime_ns(cl_event evt)
{
	cl_int err;
	cl_ulong start, end;
	err = clGetEventProfilingInfo(evt, CL_PROFILING_COMMAND_START,
		sizeof(start), &start, NULL);
	ocl_check(err, "get start");
	err = clGetEventProfilingInfo(evt, CL_PROFILING_COMMAND_END,
		sizeof(end), &end, NULL);
	ocl_check(err, "get end");
	return (end - start);
}

cl_ulong total_runtime_ns(cl_event from, cl_event to)
{
	cl_int err;
	cl_ulong start, end;
	err = clGetEventProfilingInfo(from, CL_PROFILING_COMMAND_START,
		sizeof(start), &start, NULL);
	ocl_check(err, "get start");
	err = clGetEventProfilingInfo(to, CL_PROFILING_COMMAND_END,
		sizeof(end), &end, NULL);
	ocl_check(err, "get end");
	return (end - start);
}

double runtime_ms(cl_event evt)
{
	return runtime_ns(evt)*1.0e-6;
}

double total_runtime_ms(cl_event from, cl_event to)
{
	return total_runtime_ns(from, to)*1.0e-6;
}

size_t round_div_up(size_t gws, size_t lws)
{
	return ((gws + lws - 1)/lws);
}


size_t round_mul_up(size_t gws, size_t lws)
{
	return ((gws + lws - 1)/lws)*lws;
}