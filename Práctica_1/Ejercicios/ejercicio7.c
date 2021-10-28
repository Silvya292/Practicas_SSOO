#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

void tratarSennal(int signo){
	printf("He recibido un mensaje de mi padre\n");
    printf("Señal: %d\n",signo);
}

int main(){

    pid_t idProceso, childpid;
    int status;

    printf("Soy el proceso padre, mi PID es %d.\n", getpid());

    idProceso=fork();

    switch(idProceso){
         case -1: //Error al crear el hijo
            perror("fork error\n");
            printf("ERROR al crear el proceso hijo\nerrno value= %d\n", errno); //Se imprime el valor de errno que informa del error
            exit(EXIT_FAILURE); 

        case 0:
            
            if (signal(SIGUSR1, tratarSennal) == SIG_ERR){
                perror("Signal error");
                printf("errno value= %d\n", errno);  
                exit(EXIT_FAILURE);
	        }

            printf("Soy el hijo. Mi PID es %d y el de mi padre %d.\n",getpid(),getppid());

            for(int i=0;i<5;i++){
                pause();
            }
            sleep(5);
            
        default:
            printf("Proceso padre esperando a que acabe el hijo\n");

            for(int i=0;i<5;i++){
                sleep(1);
                kill(idProceso,SIGUSR1);
            }

            sleep(2);
            kill(idProceso,SIGKILL);

    }

    while((childpid=waitpid(-1,&status,WUNTRACED | WCONTINUED))>0){ //El proceso padre espera a cualquier hijo, indicando además su estado
        if(WIFEXITED(status)){//El hijo terminó con normalidad
            printf("Hijo con PID %d finalizado, status= %d\n",(int)childpid, WEXITSTATUS(status));//Se imprime el estado del hijo
        }
        else if (WIFSIGNALED(status)){//Indica si el hijo terminó a causa de una señal no capturada
            printf("Hijo con PID %d finalizado al recibir la señal %d\n",(int)childpid, WTERMSIG(status));//Se devuelve el número de la señal que provocó la muerte del proceso hijo
        } 
    }

    if (childpid==(pid_t)-1 && errno==ECHILD){//Cuando no hay más hijos que esperar se devuelve -1 y se le da un valor a errno
        printf("\nNo hay mas hijos que esperar para el proceso padre %d. Valor de errno = %d, definido como: %s.\n\n", getpid(), errno, strerror(errno));
    }
    else{//Se imprime un mensaje de error si la llamada no tiene éxito
        printf("Error en la invocacion de wait o waitpid. Valor de errno = %d, definido como: %s.\n", errno, strerror(errno));
        exit(EXIT_FAILURE);
    }

    exit(EXIT_SUCCESS);
}
