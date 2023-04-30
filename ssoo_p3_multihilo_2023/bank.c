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
int main (int argc, const char * argv[] ) {
    /*Como se tienen que meter 5 argumentos, argc debe valer 6 como mínimo.*/
    if (argc < 6){
        perror("Número de argumentos insuficientes\n")
    }
    /*Antes de empezar, vamos a comprobar que todo funciona correctamente.*/
    if (argv[2] < 0) || (argv[3] < 0) {
        perror("Número de hilos negativo")
    }
    int fd;
    fd = open(argv[1], O_RDNLY|O_CREAT|O_TRUNC, 0544);

    close(fd);
    return 0;
}
