//SSOO-P3 2022-2023

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <stddef.h>
#include <sys/stat.h>
#include <pthread.h>
#include "queue.h"
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdbool.h>
#include <ctype.h>



/**
 * Entry point
 * @param argc
 * @param argv
 * @return
 */


/*Se inicializan las variables globales*/
int client_numop = 0, bank_numop = 0, global_balance = 0, max_op = 0, max_cuentas = 0, num_operacion = -1;
// Se crea una lista global donde se guarda el dinero de cada cuenta.
int *lista_cuentas;
/*Se crea la cola, el mutex y su condición.*/
struct queue *buffer;
pthread_mutex_t mut;
pthread_cond_t lleno, vacio;

/*Para inidicar los datos de cada operación, se ha creado este struct.*/
struct operacion {
    char operacion[9];
    char cuenta[3];
    char cuenta2[3];
    int dinero;
};

int verifyStruct(struct operacion cliente, int orden){
    // Para analizar si cada valor del struct cumple las condiciones dadas en el enunciado.
    if (strcmp(cliente.operacion, "CREAR") == 0 || strcmp(cliente.operacion, "INGRESAR") == 0 || strcmp(cliente.operacion, "RETIRAR") == 0 || strcmp(cliente.operacion, "TRASPASAR") == 0 || strcmp(cliente.operacion, "SALDO") == 0){
        int cuenta1 = atoi(cliente.cuenta);
        if (cuenta1 > max_cuentas) {
            return 0;
        }
        if (strcmp(cliente.operacion, "TRASPASAR")){
            int cuenta2 = atoi(cliente.cuenta2);
            if (cuenta2 > max_cuentas){
                return 0;
            }
        }
        return 1;
    }
    return 0;
}

struct operacion *lectura(struct operacion *lista, FILE* fd) {
    int cantidad;
    char next_op[9];
    char str_cuenta1[3];
    char str_cuenta2[3];
    char linea[80];
    /*Como es un bucle con malloc, hemos decidido utilizar una variable soporte. Cuando no lea operaciones o haya una incorrecta, se termina.*/
    int fin = 1;
    while (fin == 1){
        if (num_operacion == -1) {
            fgets(linea, sizeof(linea), fd);
            max_op = atoi(linea);
            printf("%d\n", max_op);
            num_operacion++;
        }
        if (fgets(linea, sizeof(linea), fd) == NULL){
            printf("%s\n", linea);
            return lista;
        }
        sscanf(linea, "%s", next_op);
        if (strcmp(next_op, "CREAR") == 0){
            sscanf(linea, "%s %s", next_op, str_cuenta1);
            strcpy(lista[num_operacion].operacion, next_op);
            strcpy(lista[num_operacion].cuenta, str_cuenta1);
        }
        else if (strcmp(next_op, "INGRESAR") == 0 || strcmp(next_op, "RETIRAR") == 0 ){
            sscanf(linea, "%s %s %d", next_op, str_cuenta1, &cantidad);
            strcpy(lista[num_operacion].operacion, next_op);
            strcpy(lista[num_operacion].cuenta, str_cuenta1);
            lista[num_operacion].dinero = cantidad;
        }
        else if (strcmp(next_op, "TRASPASAR") == 0) {
            sscanf(linea, "%s %s %s %d", next_op, str_cuenta1, str_cuenta2, &cantidad);
            strcpy(lista[num_operacion].operacion, next_op);
            strcpy(lista[num_operacion].cuenta, str_cuenta1);
            strcpy(lista[num_operacion].cuenta2, str_cuenta2);
            lista[num_operacion].dinero = cantidad;
        }
        else if (strcmp(next_op, "SALDO") == 0) {
            sscanf(linea, "%s %s", next_op, str_cuenta1);
            strcpy(lista[num_operacion].operacion, next_op);
            strcpy(lista[num_operacion].cuenta, str_cuenta1);
        }
        printf("Orden: %s, cuenta1: %s\n", lista[num_operacion].operacion, lista[num_operacion].cuenta);
        if (verifyStruct(lista[num_operacion], num_operacion) == 1) {
            num_operacion ++;
            }
        printf("%d\n", num_operacion);
    }
       
}

void * cajero(struct operacion *lista_ops){
    struct element elemento;
    while(client_numop < max_op) {
        pthread_mutex_lock(&mut);
        while (queue_full(buffer) == 1) {
            pthread_cond_wait(&lleno, &mut);
        }
        elemento.num_operacion = client_numop + 1;
        strcpy(elemento.operacion, lista_ops[client_numop].operacion);
        strcpy(elemento.cuenta1, lista_ops[client_numop].cuenta);
        strcpy(elemento.cuenta2, lista_ops[client_numop].cuenta2);
        elemento.cantidad = lista_ops[client_numop].dinero;
        client_numop++;
        queue_put(buffer, &elemento);
        pthread_cond_signal(&vacio);
        pthread_mutex_unlock(&mut);
    }
    pthread_exit(0);
}

void * trabajador() {
    while(bank_numop < max_op) {
        pthread_mutex_lock(&mut);
        while (queue_empty(buffer)== 1) {
            pthread_cond_wait(&vacio, &mut);
        }
        struct element *elemento = queue_get(buffer);
        bank_numop++;
        // Proceso las operaciones
        // ¿Que pasa si hay un numero de cuenta mayor al numero maximo de cuentas?
        if (strcmp(elemento->operacion, "CREAR") == 0) {
            lista_cuentas[atoi(elemento->cuenta1)] = 0;
            printf("%d CREAR %s SALDO=0 TOTAL=%d", elemento->num_operacion, elemento->cuenta1, global_balance);
        }
        else if (strcmp(elemento->operacion, "INGRESAR") == 0) {
            lista_cuentas[atoi(elemento->cuenta1)] += elemento->cantidad;
            global_balance += elemento->cantidad;
            printf("%d INGRESAR %s %d SALDO=%d TOTAL=%d", elemento->num_operacion, elemento->cuenta1, elemento->cantidad, lista_cuentas[atoi(elemento->cuenta1)], global_balance);
        }
        else if (strcmp(elemento->operacion, "RETIRAR") == 0) {
            lista_cuentas[atoi(elemento->cuenta1)] -= elemento->cantidad;
            global_balance -= elemento->cantidad;
            printf("%d RETIRAR %s %d SALDO=%d TOTAL=%d", elemento->num_operacion, elemento->cuenta1, elemento->cantidad, lista_cuentas[atoi(elemento->cuenta1)], global_balance);
        }
        else if (strcmp(elemento->operacion, "TRASPASAR") == 0) {
            lista_cuentas[atoi(elemento->cuenta1)] -= elemento->cantidad;
            lista_cuentas[atoi(elemento->cuenta2)] += elemento->cantidad;
            printf("%d TRASPASAR %s %s %d SALDO=%d TOTAL=%d", elemento->num_operacion, elemento->cuenta1, elemento->cuenta2, elemento->cantidad, lista_cuentas[atoi(elemento->cuenta2)], global_balance);
        }
        /*else {
            printf("%d SALDO %s SALDO=%d TOTAL=%d", elemento->num_operacion, elemento->cuenta1, lista_cuentas[atoi(elemento->cuenta1)], global_balance);}*/
        elemento->num_operacion = 0;
        pthread_cond_signal(&lleno);
        pthread_mutex_unlock(&mut);
    }
    pthread_exit(0);
}


int main (int argc, const char * argv[] ) {
    /*Como se tienen que meter 5 argumentos, argc debe valer 6 como mínimo.*/
    if (argc < 6){
        printf("Número de argumentos insuficientes.\n");
        return -1;
    }
    /*Antes de empezar, vamos a comprobar que todo funciona correctamente.*/
    if ((atoi(argv[2]) < 0) || (atoi(argv[3]) < 0)) {
        printf("Número de hilos negativo.\n");
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
    max_cuentas = atoi(argv[4]);
    struct operacion *list_client_ops = (struct operacion*)malloc(sizeof(struct operacion) * 200);
    FILE *fd;
    fd = fopen(argv[1], "r");
    list_client_ops = lectura(list_client_ops, fd);
    fclose(fd);
    /*next_op para escanear cadenas y cantidad para escanear cantidades de dinero*/
    int long_cola = atoi(argv[5]);
    /*Se analiza si max_op es correcta.*/
    if (max_op > 200){
        printf("Error. Se exceden las 200 operaciones.\n");
        free(list_client_ops);
        return(-1);
    }
    if (num_operacion > max_op){
        printf("Error. El número de operaciones indicadas es mayor al permitido.\n");
        free(list_client_ops);
        return(-1);
    }
    // inicializar condiciones y mutex
    pthread_mutex_init(&mut, NULL);
    pthread_cond_init(&lleno, NULL);
    pthread_cond_init(&vacio, NULL);
    // inicializar cola
    buffer = queue_init(long_cola);
    /*Se crea una lista global donde se guarda el dinero de cada cuenta. Tiene longitud igual al máximo de cuentas (argv[4]).
    Para indicar que la cuenta no existe, le ponemos de valor -1. Inicialmente, no hay ninguna cuenta creada (todos -1).*/
    lista_cuentas = (int*) malloc((max_cuentas+1) * sizeof(int));
    for(int i = 0; i < max_cuentas + 1; i++){
        lista_cuentas[i] = -1;
    }
    // numero de cajeros y trabajadores
    int num_cajeros = atoi(argv[2]);
    int num_trabajadores = atoi(argv[3]);
    // crear los threads
    pthread_t cajeros[num_cajeros], trabajadores[num_trabajadores];
    for (int i = 0; i < num_cajeros; i ++) {
        pthread_create(&cajeros[i], NULL, (void*)cajero, list_client_ops); }
    for (int j = 0; j < num_trabajadores; j++) {
        pthread_create(&trabajadores[j], NULL, (void*)trabajador, NULL); }
    // joins
    for (int i = 0; i < num_cajeros; i++) {
        pthread_join(cajeros[i], NULL); }
    for (int j = 0; j < num_trabajadores; j++) {
        pthread_join(trabajadores[j], NULL); }
    // destruir condiciones y mutex
    pthread_cond_destroy(&lleno);
    pthread_cond_destroy(&vacio);
    pthread_mutex_destroy(&mut);
    queue_destroy(buffer);
    free(lista_cuentas);
    free(list_client_ops);
    return 0;
}