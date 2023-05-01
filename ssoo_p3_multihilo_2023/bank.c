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

int main (int argc, const char * argv[] ) {
    /*Como se tienen que meter 5 argumentos, argc debe valer 6 como mínimo.*/
    if (argc < 6){
        printf("Número de argumentos insuficientes.\n");
        return -1;
    }
    /*Antes de empezar, vamos a comprobar que todo funciona correctamente.*/
    if ((argv[2] < 0) || (argv[3] < 0)) {
        printf("Número de hilos negativo.\n");
        return -1;
    }
    if (argv[4] < 1){
        printf("Número de cuentas máximo no positivo.\n");
        return -1;
    }
    if (argv[5] < 1){
        printf("Longitud de cola imposible.\n");
        return -1;
    }
    int fd, max_op;
    int posicion_actual = 0;
    int saldo_cuenta[argv[4]];
    char buffer_max_op[3];
    fd = open(argv[1], O_RDNLY|O_CREAT|O_TRUNC, 0544);
    /*Se estudia si el número máximo de operaciones no supera a 200.*/
    read (fd, &buffer_max_op, 3);
    max_op = atoi(buffer_max_op);
    if (max_op > 200){
        printf("Error. Se exceden las 200 operaciones.\n");
        close(fd);
        return(-1);
    }
    int list_client_ops[max_op];
    




    close(fd);
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