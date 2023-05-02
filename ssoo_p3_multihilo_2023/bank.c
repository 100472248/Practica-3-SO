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



/**
 * Entry point
 * @param argc
 * @param argv
 * @return
 */

int client_numop = 0, bank_numop = 0, global_balance = 0;
mutex_t mut;
struct operacion {
    char operacion[9];
    char cuenta;
    char cuenta2;
    char dinero[3];
};

int main (int argc, const char * argv[] ) {
    /*Como se tienen que meter 5 argumentos, argc debe valer 6 como mínimo.*/
    if (argc < 6){
        printf("Número de argumentos insuficientes.\n");
        return -1;
    }
    /*Antes de empezar, vamos a comprobar que todo funciona correctamente.*/
    if ((argv[3] < 0) || (argv[4] < 0)) {
        printf("Número de hilos negativo.\n");
        return -1;
    }
    if (argv[5] < 1){
        printf("Número de cuentas máximo no positivo.\n");
        return -1;
    }
    if (argv[6] < 1){
        printf("Longitud de cola imposible.\n");
        return -1;
    }
    int max_op, fd;
    char buffer_max_op[4];
    char caracter_leido;
    fd = open(argv[2], O_RDONLY, 0644);
    while (caracter_leido != '\n') {
        read(fd, buffer_max_op, 1);
    }
    max_op = atoi(buffer_max_op);
    if (max_op > 200){
        printf("Error. Se exceden las 200 operaciones.\n");
        close(fd);
        return(-1);
    }
    operacion *list_num_ops[] = (operacion*)malloc(sizeof(operacion) * max_op);



    return 0;
}

void* cajero(){

pthread_exit(-1);
}

void* trabajador(){

pthread_exit(-1);
}

char leer_orden(int fichero, int posicion_inicial) {
    /*Leer cada tipo de orden (Aún no enlazada al main)*/
    char buffer, palabra[10];
    int i = 0;
    lseek(fichero, posicion_inicial, 0);
    while (buffer != ' ' && buffer != '\t' && buffer != '\n'){
        read(fichero, &buffer, 1);
        if (isalpha(buffer)){
            palabra[i] = buffer;
            i++;
        }
    }
    return palabra;
}