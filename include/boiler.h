#ifndef BOILER_H
#define BOILER_H

//#define CL_TARGET_OPENCL_VERSION 300

/* A collection of functions wrapping the most common boilerplate
   of OpenCL program. You can now reduce the boilerplate to:
  */

#if 0 // example usage:
#include "ocl_boiler.h"

int main(int argc, char *argv[])
{
	cl_platform_id p = select_platform();
	cl_device_id d = select_device(p);
	cl_context ctx = create_context(p, d);
	cl_command_queue que = create_queue(ctx, d);
	cl_program prog = create_program("kernels.ocl", ctx, d);

	/* Here starts the custom part: extract kernels,
	 * allocate buffers, run kernels, get results */

	return 0;
}
#endif

/* Include the headers defining the OpenCL host API */
#ifdef __APPLE__
#include <OpenCL/cl.h>
#else
#include <CL/cl.h>
#endif

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#define BUFSIZE 4096

//#define CL_TARGET_OPENCL_VERSION 120


typedef struct{
	cl_platform_id p;
	cl_device_id d;
	cl_context ctx;
	cl_command_queue que;
	cl_program prog;
} cl_resources;


/* Check an OpenCL error status, printing a message and exiting
 * in case of failure
 */

void ocl_check(cl_int err, const char *msg, ...); 

// Return the ID of the platform specified in the OCL_PLATFORM
// environment variable (or the first one if none specified)

cl_platform_id select_platform(); 

// Return the ID of the device (of the given platform p) specified in the
// OCL_DEVICE environment variable (or the first one if none specified)

cl_device_id select_device(cl_platform_id p);


// Create a one-device context

cl_context create_context(cl_platform_id p, cl_device_id d);


// Create a command queue for the given device in the given context

cl_command_queue create_queue(cl_context ctx, cl_device_id d);


// Compile the device part of the program, stored in the external
// file `fname`, for device `dev` in context `ctx`

cl_program create_program(const char * const fname, cl_context ctx,
	cl_device_id dev);


// Runtime of an event, in nanoseconds. Note that if NS is the
// runtimen of an event in nanoseconds and NB is the number of byte
// read and written during the event, NB/NS is the effective bandwidth
// expressed in GB/s

void create_resources(cl_resources* resources, const char *const  ocl_filename) ; 


void release_resources(cl_resources* resources) ; 


cl_ulong runtime_ns(cl_event evt);

cl_ulong total_runtime_ns(cl_event from, cl_event to);

// Runtime of an event, in milliseconds

double runtime_ms(cl_event evt);

double total_runtime_ms(cl_event from, cl_event to);

/* divide gws by lws rounding up */

size_t round_div_up(size_t gws, size_t lws);


/* round gws to the next multiple of lws */

size_t round_mul_up(size_t gws, size_t lws);


#endif
