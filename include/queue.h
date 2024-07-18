#ifndef QUEUE_H
#define QUEUE_H

#include <stdio.h>
#include <stdlib.h>
#include "data_structures.h"


void initQueue(Queue* q, int initSize);

void enqueue(Queue* q, sequence* s);

sequence dequeue(Queue* q);

int is_Empty(Queue* q);

void release_queue(Queue* q) ; 

#endif
