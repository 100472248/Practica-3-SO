//SSOO-P3 2022-2023

#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>
#include "queue.h"



//To create a queue
queue* queue_init(int size){

	queue * q = (queue *)malloc(size * sizeof(queue));

	return q;
}


// To Enqueue an element
int queue_put(queue *q, struct element* x) {
    int i = 0;
    while (0 != q[i].param1){ // iterate through all the filled places
        i ++;
    }
    q[i].param1 = x;
	return 0;
}


// To Dequeue an element.
struct element* queue_get(queue *q) {
	struct element* element;
	
	return element;
}


//To check queue state
int queue_empty(queue *q){
	
	return 0;
}

int queue_full(queue *q){
	
	return 0;
}

//To destroy the queue and free the resources
int queue_destroy(queue *q){
	return 0;
}

// for testing the queue
/*int main(){

    queue *q;
    struct element s1;
    s1.param1 = 3;
    struct element * ptr;
    ptr = &s1;
    q = queue_init(5);
    queue_put(q,ptr);
    s1.param1 = 4;
    queue_put(q, ptr);
//    int i;
//    while (0 == q->param1){
//        i ++;
//    }
    struct element * aux;
    aux = q[0].param1;

    printf("%d\n", aux->param1);*/



}