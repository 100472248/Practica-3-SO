//SSOO-P3 2022-2023

#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>
#include "queue.h"



//To create a queue
queue* queue_init(int size){

    struct queue *q = (struct queue *)malloc(sizeof(struct queue));
    q->elemento = (struct element *)malloc(size * sizeof(struct element));
    q->head = 0;
    q->tail = size;
    q->size = size;
    q->n_elementos = 0;
	return q;
}


// To Enqueue an element
int queue_put(struct queue *q, struct element* x, int i) {
    // first check capacity before puting an element
    int check_capacity = queue_full(q);
    while (1 == check_capacity) {   // hacer loops hasta que la queue no este llena
        check_capacity = queue_full(q);
    }
    // No merece la pena mirar si es una operacion u otra para poner los elementos
    // ya que de igual forma los que no existan por como es la operacion van a ser NULL
    strcpy(q->elemento[i].operacion, x->operacion);
    strcpy(q->elemento[i].cuenta1, x->cuenta1);
    strcpy(q->elemento[i].cuenta2, x->cuenta2);
    q->elemento[i].cantidad = x->cantidad;
    q->n_elementos++;
	return 0;
}


// To Dequeue an element.
struct element* queue_get(struct queue *q, int i) {
    int check_capacity = queue_empty(q);
    while (1 == check_capacity){
        check_capacity = queue_empty(q); // hacer loops que la queue tenga un elemento
    }
	struct element *elemento;
	strcpy(elemento->operacion, q->elemento[i].operacion);
    strcpy(elemento->cuenta1, q->elemento[i].cuenta1);
    strcpy(elemento->cuenta2, q->elemento[i].cuenta2);
    elemento->cantidad = q->elemento[i].cantidad;
    q->n_elementos--;
	return elemento; // return element
}


//To check queue state
int queue_empty(struct queue *q){
    if (q->n_elementos == 0) {
        return 1;
    }
    return 0;
}

int queue_full(struct queue *q){
    if (q->n_elementos == q->size) {
        return 1;
    }
    return 0;
}

//To destroy the queue and free the resources
int queue_destroy(struct queue *q){
    for (int i = 0; i < q->size; i++) {
        queue_get(q, i);
    }
    free(q->elemento);
    free(q);
	return 0;
}

// for testing the queue
/*
int main(){

    queue *q;
    struct element s1;
    s1.cantidad = 3;
    strcpy(s1.cuenta1, "03");
    struct element * ptr;
    ptr = &s1;
    q = queue_init(5);
    queue_put(q,ptr, 1);
    s1.cantidad = 5;
    queue_put(q, ptr, 2);
    printf("%s\n", q->elemento[1].cuenta1);
    return 0;
}*/
