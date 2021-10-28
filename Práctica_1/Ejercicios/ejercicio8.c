#include <sys/signal.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>

int main(int argc,char **argv){
    if (argc<2){ 
        printf("Error: El número de argumentos insertados no es el correcto\n");
        printf("EJEMPLO DE EJECUCIÓN: ./prueba n (siendo n el PID del proceso.\n");
        exit(EXIT_FAILURE);
    }
    int precep=atoi(argv[1]);
    printf("Proceso receptor de la señal: PID %d\n",precep);
    
    if(!kill(precep,SIGUSR1)){
        printf("Señal SIGUSR1 enviada correctamente\n");
    }
    else{
        printf("ERROR en el envió de la señal. Valor de errno: %d, definido como: %s\n",errno,strerror(errno));
    }

    sleep(1);

    if(!kill(precep,SIGKILL)){
        printf("Señal SIGKILL enviada correctamente\n");
    }
    else{
        printf("ERROR en el envió de la señal. Valor de errno: %d, definido como: %s\n",errno,strerror(errno));
    }

    exit(EXIT_SUCCESS);

}