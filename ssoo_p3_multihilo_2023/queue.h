#ifndef HEADER_FILE
#define HEADER_FILE


struct element {
    int num_operacion; // numero de operacion, será 0 para indicar que el elemento está vacío
    char operacion [10]; // nombre de la operacion
    char cuenta1[3]; // numero de cuenta
    char cuenta2[3]; // segunda cuenta
    int cantidad; // cantidad a depositar

};

typedef struct queue {
    struct element * elemento; // Puntero a la lista de operaciones, la cola en si
    int size; // Tamaño de la cola
    int element_searching; // Numero de operacion a buscar en la cola
    int n_elementos; // Numero de elementos en la cola actualmente
}queue;

queue* queue_init (int size);
int queue_destroy (queue *q);
int queue_put (queue *q, struct element* elem);
struct element * queue_get(queue *q);
int queue_empty (queue *q);
int queue_full(queue *q);

#endif
