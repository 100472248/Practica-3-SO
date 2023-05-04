//SSOO-P3 2022-2023

#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>
#include "queue.h"



//To create a queue
queue* queue_init(int size){

    queue *q = (queue *)malloc(sizeof(queue));
    q->elemento = (element *)malloc(size * sizeof(element));
    q->tail = size;
    q->size = size;



	return q;
}


// To Enqueue an element
int queue_put(queue *q, struct element* x) {
    // first check capacity before puting an element
    int checK_capacity = queue_full(q);
    while (1 == checK_capacity){   // loop until queue is empty
        checK_capacity = queue_full(q);

	return 0;
}


// To Dequeue an element.
struct element* queue_get(queue *q) {
    int check_capacity = queue_empty(q);
    if (1 == check_capacity){
        check_capacity = queue_empty(q); // loop until the queue has an element
    }
	struct element* element;
	
	return element; // return element
}


//To check queue state
int queue_empty(queue *q){
//    int i;
//    for (i = 0; i < size;){}
//	return 0;
}

int queue_full(queue *q){
    for (int index = 0; index < q.size; index++){

    }
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
    s1.element = 3;
    struct element * ptr;
    ptr = &s1;
    q = queue_init(5);
    queue_put(q,ptr);
    s1.element = 4;
    queue_put(q, ptr);
//    int i;
//    while (0 == q->element){
//        i ++;
//    }
    struct element * aux;
    aux = q[0].element;

    printf("%d\n", aux->element);*/



}