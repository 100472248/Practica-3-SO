//SSOO-P3 2022-2023

#include <stdlib.h>
#include <string.h>
#include "queue.h"

// Para crear la cola
queue* queue_init(int size){
    /* Almacenamos espacio dinamicamente para la cola y para la lista de elementos
       para almacenar las operaciones de la misma */
    struct queue *q = (struct queue *)malloc(sizeof(struct queue));
    q->elemento = (struct element *)malloc(size * sizeof(struct element));
    // Inicializamos el tamaño, la operacion a buscar, y el numero de elementos
    q->size = size;
    q->element_searching = 0;
    q->n_elementos = 0;
    // Ponemos el numero de operacion a 0 para indicar que eso significa que no existe la operacion aun
    for (int i = 0; i < q->size; i++) {
        q->elemento[i].num_operacion = 0;
    }
    return q;
}

// Para escribir un elemento
int queue_put(struct queue *q, struct element* x) {
    int j = 0;
    // Buscamos un hueco (num_operacion = 0)
    for (int i = 0; i < q->size; i++) {
        if (q->elemento[i].num_operacion == 0) {
            j = i;
            break;
        }
    }
    // Escribimos el elemento en el hueco encontrado
    q->elemento[j].num_operacion = x->num_operacion;
    strcpy(q->elemento[j].operacion, x->operacion);
    strcpy(q->elemento[j].cuenta1, x->cuenta1);
    strcpy(q->elemento[j].cuenta2, x->cuenta2);
    q->elemento[j].cantidad = x->cantidad;
    // Actualizamos el numero de elementos de la cola
    q->n_elementos++;
    return 0;
}


// To Dequeue an element.
struct element* queue_get(struct queue *q) {
    int j = 0;
    // Buscamos elemento que contenga la operacion con el numero de operacion que buscamos
    for (int i = 0; i < q->size; i++) {
        if (q->elemento[i].num_operacion == q->element_searching) {
            j = i;
            break;
        }
    }
    // Devolvemos un puntero a la direccion donde empieza ese elemento
    struct element *elemento = &q->elemento[j];
    // Actualizamos el numero de elementos
    q->n_elementos--;
    return elemento; // return element
}

// Mirar si la cola está vacía
int queue_empty(struct queue *q){
    if (q->n_elementos == 0) {
        return 1;
    }
    return 0;
}

// Mirar si la cola está llena
int queue_full(struct queue *q){
    if (q->n_elementos == q->size) {
        return 1;
    }
    return 0;
}

// Destruir la cola y liberar memoria
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
