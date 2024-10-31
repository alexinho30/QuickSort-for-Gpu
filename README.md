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

./main  -l[lws] -g [nwg] -i test/input.txt -o test/output.txt

if you want to test the algorithm with random data use

./main -l[lws] -g [nwg] -t -s [seed] -n[number of elements] -d [distrisbution] -p[first parameter of distribution ] -r [second parameter of the distribution]

-t means that you are using test mode so write it 

-d distribution you want to use 
    [1 normal distribution]
    [2 uniform distribution]

-p first parameter 
    [lower element for uniform distribution]
    [mean for normal distribution]

-r second parameter 
    [greater element for uniform distribution]
    [standard deviation for normal distribution]
Also you can choose to use an ibrid version or only gpu version changing the threshold in the quicksort_gpu.c file .
At the end in the test directory you'll find a file named benchmark with some statistics.  

```
