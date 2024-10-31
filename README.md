# Quicksort-gpu
An implemenation of quicksort algorithm on gpu using opencl

## structure of the project 

```bash
Quicksort-gpu
├── README.md
├── quicksort.ocl
├── Makefile
├── main.c
├── src
│   ├── boiler.c
│   ├── queue.c
│   ├── quicksort_cpu_version.c
│   ├── random_numbers.c
│   ├── utility.c
│   ├── quicksort_gpu.c
├── include
│   ├── boiler.h
│   ├── queue.h
│   ├── quicksort_cpu_version.h
│   ├── random_numbers.h
│   ├── utility.h
│   ├── quicksort_gpu.h
├── test
│   ├── input.txt
│   ├── output.txt
```

#### Requirements
- [POSIX]
- [gcc]
- [Opencl]

## usage

```bash
to compile use make 

if you want to order your own array use

./main  -l [lws] -g[nwg cu] -i test/input.txt -o test/output.txt -m[local memory] -x [vect4]

if you want to test the algorithm with random data use

./main -l [lws] -g[nwg cu] -t [test] -m[local memory] -x [vect4] -s [seed] -n[number of elements] -d [distrisbution] -p[first parameter of distribution ] -r [second parameter of the distribution]

-l local work size
    [lws dimension]

-g number of work group per compute unit
    [nwg_cu]

-t means that you are using test mode so write it 

-m means that you want to use local memory

-x means that you want to use vectorized version

-d distribution you want to use 
    [1 normal distribution]
    [2 uniform distribution]

-p first parameter 
    [lower element for uniform distribution]
    [mean for normal distribution]

-r second parameter 
    [greater element for uniform distribution]
    [standard deviation for normal distribution]

At the end in the test directory you'll find a file named benchmark with some statistics.  
```
