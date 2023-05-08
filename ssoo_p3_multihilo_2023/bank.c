//SSOO-P3 2022-2023

#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <pthread.h>
#include "queue.h"
#include <string.h>
#include <sys/types.h>

/**
 * Entry point
 * @param argc
 * @param argv
 * @return
 */


//Se inicializan las variables globales
int client_numop = 0, bank_numop = 0, global_balance = 0, operaciones = 0;
// Se crea una lista global donde se guarda el dinero de cada cuenta
int *lista_cuentas;
//Se crea la cola, el mutex y su condición
struct queue *buffer;
pthread_mutex_t mut;
pthread_cond_t lleno, vacio;
//Para almacenar los datos de cada operación, se ha creado este struct
struct operacion {
    char operacion[10];
    char cuenta[3];
    char cuenta2[3];
    int dinero;
};

void * cajero(struct operacion *lista_ops){
    struct element elemento;
    // Mientras que no se hayan realizado las operaciones pedidas
    while(client_numop < operaciones) {
        // Bloqueamos el mutex
        pthread_mutex_lock(&mut);
        // Si está vacía la cola, esperaremos a que se vacíe algun elemento
        while ((queue_full(buffer) == 1) && (client_numop < operaciones)) {
            pthread_cond_wait(&lleno, &mut);
        }
        // Si al salir del wait otro hilo cajero ya ha terminado con las operaciones correspondientes, salimos
        if (client_numop >= operaciones) {
            pthread_mutex_unlock(&mut);
            break;
        }
        // Copiamos la operacion que toca de la lista a un elmemento que se añadirá a la cola
        elemento.num_operacion = client_numop + 1;
        strcpy(elemento.operacion, lista_ops[client_numop].operacion);
        strcpy(elemento.cuenta1, lista_ops[client_numop].cuenta);
        strcpy(elemento.cuenta2, lista_ops[client_numop].cuenta2);
        elemento.cantidad = lista_ops[client_numop].dinero;
        // Actualizamos client_numop
        client_numop++;
        // Escribimos el elemento en la cola
        queue_put(buffer, &elemento);
        // Mandamos la señal de que la cola no está vacia y liberamos el mutex
        pthread_cond_signal(&vacio);
        pthread_mutex_unlock(&mut);
    }
    pthread_exit(0);
}

void * trabajador() {
    // Mientras no se hayan procesado todas las operaciones
    while(bank_numop < operaciones) {
        // Bloqueamos el mutex
        pthread_mutex_lock(&mut);
        // Si el buffer está vacio esperamos a que se escriba algun elemento
        while ((queue_empty(buffer)== 1) && (bank_numop < operaciones)) {
            pthread_cond_wait(&vacio, &mut);
        }
        // Si al salir del wait otro hilo trabajador ya ha terminado con las operaciones correspondientes, salimos
        if (bank_numop >= operaciones) {
            pthread_mutex_unlock(&mut);
            break;
        }
        // Actualizamos bank_numop
        bank_numop++;
        // Actualizamos el elemento que se buscará en el buffer al numero de operacion bank_numop
        buffer->element_searching = bank_numop;
        // Cogemos el elemento y lo almacenamos
        struct element *elemento = queue_get(buffer);
        /* Proceso las operaciones usando la lista con el dinero de cada cuenta siempre y cuando exista tal
           cuenta (el dinero no es -1) e imprimimos la salida correspondiente */
        if (strcmp(elemento->operacion, "CREAR") == 0) {
            lista_cuentas[atoi(elemento->cuenta1)] = 0;
            printf("%d CREAR %s SALDO=0 TOTAL=%d\n", elemento->num_operacion, elemento->cuenta1, global_balance);
        }
        else if ((strcmp(elemento->operacion, "INGRESAR") == 0) && (lista_cuentas[atoi(elemento->cuenta1)] != -1)) {
            lista_cuentas[atoi(elemento->cuenta1)] += elemento->cantidad;
            global_balance += elemento->cantidad;
            printf("%d INGRESAR %s %d SALDO=%d TOTAL=%d\n", elemento->num_operacion, elemento->cuenta1,
                   elemento->cantidad, lista_cuentas[atoi(elemento->cuenta1)], global_balance);
        }
        else if ((strcmp(elemento->operacion, "RETIRAR") == 0) && (lista_cuentas[atoi(elemento->cuenta1)] != -1)) {
            lista_cuentas[atoi(elemento->cuenta1)] -= elemento->cantidad;
            global_balance -= elemento->cantidad;
            printf("%d RETIRAR %s %d SALDO=%d TOTAL=%d\n", elemento->num_operacion, elemento->cuenta1,
                   elemento->cantidad, lista_cuentas[atoi(elemento->cuenta1)], global_balance);
        }
        else if ((strcmp(elemento->operacion, "TRASPASAR") == 0) &&
        (lista_cuentas[atoi(elemento->cuenta1)] != -1) && (lista_cuentas[atoi(elemento->cuenta2)] != -1)) {
            lista_cuentas[atoi(elemento->cuenta1)] -= elemento->cantidad;
            lista_cuentas[atoi(elemento->cuenta2)] += elemento->cantidad;
            printf("%d TRASPASAR %s %s %d SALDO=%d TOTAL=%d\n", elemento->num_operacion, elemento->cuenta1,
                   elemento->cuenta2, elemento->cantidad, lista_cuentas[atoi(elemento->cuenta2)], global_balance);
        }
        else if ((strcmp(elemento->operacion, "SALDO") == 0) && (lista_cuentas[atoi(elemento->cuenta1)] != -1)) {
            printf("%d SALDO %s SALDO=%d TOTAL=%d\n", elemento->num_operacion, elemento->cuenta1,
                   lista_cuentas[atoi(elemento->cuenta1)], global_balance);
        }
        /* Especificamos que la operacion ha sido procesada y ahora ese elemento está vacío poniendo el numero de
           operacion a 0 */
        elemento->num_operacion = 0;
        // Mandamos la señal de que la cola no está llena y liberamos el mutex
        pthread_cond_signal(&lleno);
        pthread_mutex_unlock(&mut);
    }
    pthread_exit(0);
}


int main (int argc, const char * argv[] ) {
    // Comprobamos el numero de argumentos
    if (argc != 6){
        printf("Número de argumentos incorrecto.\n");
        return -1;
    }
    // Comprobamos la validez de los argumentos
    if ((atoi(argv[2]) < 1) || (atoi(argv[3]) < 1)) {
        printf("Número de hilos incorrecto.\n");
        return -1;
    }
    if (atoi(argv[4]) < 1){
        printf("Número de cuentas máximo no positivo.\n");
        return -1;
    }
    if (atoi(argv[5]) < 1){
        printf("Longitud de cola imposible.\n");
        return -1;
    }
    // Declaramos e inicializamos las variables que vamos a utilizar para la lectura y almacenamiento del fichero
    int cuentas = 0;
    int max_cuentas = atoi(argv[4]);
    int cantidad = 0;
    int long_cola = atoi(argv[5]);
    char next_element[10];
    FILE *fd;
    int num_operacion = -1;
    // Almacenaremos espacio dinamico para guardar la lista de structs operacion donde se almacenará cada operacion
    struct operacion *list_client_ops = (struct operacion*)malloc(sizeof(struct operacion) * 200);
    fd = fopen(argv[1], "r");
    if (fd == NULL) {
        printf("Error al abrir el fichero\n");
        return -1;
    }
    /* Mientras el fichero no se haya leido entero, iremos leyendo cada elemento con fscanf, ya sea un %d o un %s
       y almacenaremos cada operacion en la lista de operaciones listclientops */
    while (feof(fd) == 0) {
        // La primera vez solo leeremos el numero de operaciones a procesar
        if (num_operacion == -1) {
            fscanf(fd, "%d", &operaciones);
            num_operacion++;
            continue;
        }
        // Leemos la operacion y procesamos los siguientes elementos según la que sea, pues variarán los argumentos
        fscanf(fd, "%s", next_element);
        if (strcmp(next_element, "CREAR") == 0){
            strcpy(list_client_ops[num_operacion].operacion, next_element);
            fscanf(fd, "%s", next_element);
            strcpy(list_client_ops[num_operacion].cuenta, next_element);
            num_operacion++;
            cuentas++;
        }
        else if (strcmp(next_element, "INGRESAR") == 0 || strcmp(next_element, "RETIRAR") == 0 ){
            strcpy(list_client_ops[num_operacion].operacion, next_element);
            fscanf(fd, "%s %d", next_element, &cantidad);
            strcpy(list_client_ops[num_operacion].cuenta, next_element);
            list_client_ops[num_operacion].dinero = cantidad;
            num_operacion++;
        }
        else if (strcmp(next_element, "TRASPASAR") == 0) {
            strcpy(list_client_ops[num_operacion].operacion, next_element);
            fscanf(fd, "%s", next_element);
            strcpy(list_client_ops[num_operacion].cuenta, next_element);
            fscanf(fd, "%s %d", next_element, &cantidad);
            strcpy(list_client_ops[num_operacion].cuenta2, next_element);
            list_client_ops[num_operacion].dinero = cantidad;
            num_operacion++;
        }
        else if (strcmp(next_element, "SALDO") == 0) {
            strcpy(list_client_ops[num_operacion].operacion, next_element);
            fscanf(fd, "%s", next_element);
            strcpy(list_client_ops[num_operacion].cuenta, next_element);
            num_operacion++;
        }
    }
    fclose(fd);
    // Comprobamos si no se exceden las 200 operaciones
    if (operaciones > 200){
        printf("Error. Se exceden las 200 operaciones.\n");
        free(list_client_ops);
        return(-1);
    }
    // Comprobamos que el numero de operaciones es correcto
    if (num_operacion != operaciones){
        printf("Error. El número de operaciones es incorrecto.\n");
        free(list_client_ops);
        return(-1);
    }
    // Comprobamos que no se crean mas cuentas de las permitidas
    if (cuentas > max_cuentas){
        printf("Error. El número de cuentas es mayor al permitido\n");
        free(list_client_ops);
        return(-1);
    }
    // Inicializar condiciones y mutex
    pthread_mutex_init(&mut, NULL);
    pthread_cond_init(&lleno, NULL);
    pthread_cond_init(&vacio, NULL);
    // Inicializar cola
    buffer = queue_init(long_cola);
    /* Se crea una lista global almacenando espacio dinamico donde se guarda el dinero de cada cuenta.
       Tiene longitud igual al máximo de cuentas + 1, ya que será necesario para poder acceder correctamente usando
       los numeros de cuenta. Para indicar que la cuenta no existe, le ponemos valor -1.
       Inicialmente, no hay ninguna cuenta creada (todos -1). */
    lista_cuentas = (int*) malloc((cuentas+1) * sizeof(int));
    for(int i = 0; i < cuentas + 1; i++){
        lista_cuentas[i] = -1;
    }
    // Numero de cajeros y trabajadores
    int num_cajeros = atoi(argv[2]);
    int num_trabajadores = atoi(argv[3]);
    // Crear los threads
    pthread_t cajeros[num_cajeros], trabajadores[num_trabajadores];
    for (int i = 0; i < num_cajeros; i ++) {
        pthread_create(&cajeros[i], NULL, (void*)cajero, list_client_ops); }
    for (int j = 0; j < num_trabajadores; j++) {
        pthread_create(&trabajadores[j], NULL, (void*)trabajador, NULL); }
    // Joins por los threads
    for (int i = 0; i < num_cajeros; i++) {
        pthread_join(cajeros[i], NULL); }
    for (int j = 0; j < num_trabajadores; j++) {
        pthread_join(trabajadores[j], NULL); }
    // Destruir condiciones y mutex
    pthread_cond_destroy(&lleno);
    pthread_cond_destroy(&vacio);
    pthread_mutex_destroy(&mut);
    queue_destroy(buffer);
    // Liberar la memoria dinámica reservada anteriormente
    free(lista_cuentas);
    free(list_client_ops);
    return 0;
}
