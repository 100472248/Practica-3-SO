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
    char cuenta[3];
    char cuenta2[3];
    int dinero;
};

int main (int argc, const char * argv[] ) {
    /*Como se tienen que meter 5 argumentos, argc debe valer 6 como mínimo.*/
    if (argc < 6){
        printf("Número de argumentos insuficientes.\n");
        return -1;
    }
    /*Antes de empezar, vamos a comprobar que todo funciona correctamente.*/
    if ((atoi(argv[3]) < 0) || (atoi(argv[4]) < 0)) {
        printf("Número de hilos negativo.\n");
        return -1;
    }
    if (atoi(argv[5]) < 1){
        printf("Número de cuentas máximo no positivo.\n");
        return -1;
    }
    if (atoi(argv[6]) < 1){
        printf("Longitud de cola imposible.\n");
        return -1;
    }
    /*next_op para escanear cadenas y cantidad para escanear cantidades de dinero*/
    int max_op, cantidad;
    char next_op[9];
    FILE *fd;
    /*Para diferenciar la lectura del max_op frente el resto de operaciones, marcamos num_operacion como -1 y lo pasamos a 0 cuando se
    realice esta operación.*/
    int num_operacion = -1;
    operacion *list_num_ops[] = (operacion*)malloc(sizeof(operacion) * 200);
    fd = fopen(argv[2], O_RDONLY, 0644);
    /*Como es un bucle con malloc, hemos decidido utilizar una variable soporte. Cuando no lea operaciones o haya una incorrecta, se termina.*/
    int fin = 1;
    while (fin == 1){
        if num_operacion == -1 {
            fscanf(fd, "%d", &max_op);
            num_operacion ++;
        }
        fscanf(fd, "%s", next_op);
        list_num_ops[num_operacion].operacion = next_op;
        if (strcmp(next_op, "CREAR") == 0){
            fscanf(fd, "%s", next_op);
            list_num_ops[num_operacion].cuenta = next_op;
            list_num_ops[num_operacion].dinero = 0;
        }
        else if (strcmp(next_op, "INGRESAR") == 0 || strcmp(next_op, "RETIRAR") == 0 ){
            fscanf(fd, "%s", next_op);
            list_num_ops[num_operacion].cuenta= next_op;
            fscanf(fd, "%d", &cantidad);
            list_num_ops[num_operacion].dinero = cantidad;
        }
        else if strcmp(next_op, "TRASPASAR") == 0 {
            fscanf(fd, "%s", next_op);
            list_num_ops[num_operacion].cuenta= next_op;
            fscanf(fd, "%s", next_op);
            list_num_ops[num_operacion].cuenta2= next_op;
            fscanf(fd, "%d", &cantidad);
            list_num_ops[num_operacion].dinero = cantidad;
        }
        else if strcmp(next_op, "SALDO") == 0 {
            fscanf(fd, "%s", next_op);
            list_num_ops[num_operacion].cuenta= next_op;
        }
        else {
            fin = 0;}
            
        if verifyStruct(list_num_ops, num_operacion) == True {
            num_operacion ++;}
    }
    /*SE analiza si max_op es correcta.*/
    if (max_op > 200){
        printf("Error. Se exceden las 200 operaciones.\n");
        free(*list_num_ops);
        fclose(fd);
        return(-1);
    }
    if (num_operacion > max_op){
        printf("Error. El número de operaciones indicadas es mayor al permitido.\n");
        free(*list_num_ops);
        fclose(fd);
        return(-1);
    }
    free(*list_num_ops);
    fclose(fd);
    return 0;
}

void* cajero(){

pthread_exit(-1);
}

void* trabajador(){

pthread_exit(-1);
}

bool verifyStruct(struct operacion *clientes, int orden){
    operacion cliente = clientes[orden];
    if (strcmp(cliente.operacion, "CREAR") == 0) {
        int cuenta = atoi(&cliente->cuenta);
        if isdigit(cuenta) == False {
            return False;
        }
    }
    if (strcmp(cliente.operacion, "INGRESAR") == 0 || strcmp(cliente.operacion, "RETIRAR") == 0) {
        int cuenta = atoi(&cliente->cuenta);
        if isdigit(cuenta) == False {
            return False;
        }
        if isdigit(cliente.dinero) == False {
            return False
        }
   }
    if (strcmp(cliente.operacion, "TRASPASAR") == 0) {
        int cuenta = atoi(&cliente->cuenta);
        if isdigit(cuenta) == False {
            return False;
        }
        cuenta = atoi(&cliente->cuenta2);
        if isdigit(cuenta) == False {
            return False;
        }
        if isdigit(cliente.dinero) == False {
            return False
        }
   }
    if (strcmp(cliente.operacion, "SALDO") == 0) {
        int cuenta = atoi(&cliente->cuenta);
        if isdigit(cuenta) == False {
            return False;
        }
    }
    else {
        return False;
    }
    return True;
}