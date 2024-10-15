#ifndef QUEUE_H
#define QUEUE_H

#include <stdio.h>
#include <stdlib.h>

typedef struct{
	int sstart ;
	int send ;
	float pivot_value ; 	
} sequence;

typedef struct{
	int sstart ;
	int send ;	
} sequence_;

typedef struct {
    sequence *arr ;
    int cur_size ;
    int tot_size ;
    int head ;
    int tail ; 
} Queue;


void initQueue(Queue* q, int initSize);

void enqueue(Queue* q, sequence* s);

sequence dequeue(Queue* q);

int is_Empty(Queue* q);

void release_queue(Queue* q) ; 

#endif
