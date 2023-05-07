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
    // Ponemos el numero de operacion a 0 para indicar que eso significa que no existe la operacion aun
    for (int i = 0; i < q->size; i++) {
        q->elemento[i].num_operacion = 0;
    }
	return q;
}


// To Enqueue an element
int queue_put(struct queue *q, struct element* x) {
    // first check capacity before puting an element
    int j = 0;
    int check_capacity = queue_full(q);
    while (1 == check_capacity) {   // hacer loops hasta que la queue no este llena
        check_capacity = queue_full(q);
    }
    // Ponemos el elemento en la primera casilla despues del elemento mas avanzado en la cola
    for (int i = q->size - 1; i >= 0; i--) {
        if (q->elemento[i].num_operacion != 0) {
            if (i != q->size - 1) {
                j = i + 1;
                break;
            }
            else {
                j = 0;
                break;
            }
        }
    }
    // No merece la pena mirar si es una operacion u otra para poner los elementos
    // ya que de igual forma los que no existan por como es la operacion van a ser NULL
    q->elemento[j].num_operacion = x->num_operacion;
    strcpy(q->elemento[j].operacion, x->operacion);
    strcpy(q->elemento[j].cuenta1, x->cuenta1);
    strcpy(q->elemento[j].cuenta2, x->cuenta2);
    q->elemento[j].cantidad = x->cantidad;
    q->n_elementos++;
	return 0;
}


// To Dequeue an element.
struct element* queue_get(struct queue *q) {
    int j = 0;
    int check_capacity = queue_empty(q);
    while (1 == check_capacity){
        check_capacity = queue_empty(q); // hacer loops que la queue tenga un elemento
    }
    // Buscamos el primer elemento lleno de la cola
    for (int i = 0; i < q->size; i++) {
        if (q->elemento[i].num_operacion != 0) {
            j = i;
            break;
        }
    }
	struct element *elemento = &q->elemento[j];
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
