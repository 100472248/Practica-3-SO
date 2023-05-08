#ifndef HEADER_FILE
#define HEADER_FILE


struct element {
	// Define the struct yourself
    int num_operacion; // numero de operacion, si no existe aun sera 0
	char operacion [10]; // nombre de la operacion
    char cuenta1[3]; // numero de cuenta
    char cuenta2[3]; // segunda cuenta
    int cantidad; // cantidad a depositar

};

typedef struct queue {
	// Define the struct yourself
	 struct element * elemento;

     int size;
     int element_searching;
     int n_elementos;
}queue;

queue* queue_init (int size);
int queue_destroy (queue *q);
int queue_put (queue *q, struct element* elem);
struct element * queue_get(queue *q);
int queue_empty (queue *q);
int queue_full(queue *q);

#endif
