#include <sys/types.h> 
#include <sys/wait.h> 
#include <unistd.h> 
#include <stdio.h> 
#include <stdlib.h> 
#include <errno.h> 
#include <string.h> //Para la funcion strerror(), que permite describir el valor de errno como cadena.

//Corregido

int main(){

    pid_t idProceso, childpid;
    int status;

    printf("Soy el proceso padre, mi PID es %d.\n\n", getpid()); //Como no se ha creado ningún hijo aún, el PID devuelto es el del proceso padre

    idProceso=fork(); //Cada vez que se recorra el bucle, el proceso hijo creado realizará una copia del BCP del padre

    switch(idProceso){
        case -1: //Error al crear el hijo
            perror("fork error\n");
            printf("ERROR al crear el proceso hijo\nerrno value= %d\n", errno); //Se imprime el valor de errno que informa del error
            exit(EXIT_FAILURE); 

        case 0:
            printf("Soy el hijo. Mi PID es %d y el de mi padre %d.\n",getpid(),getppid());
            exit(EXIT_SUCCESS);
        
        default:
            printf("Proceso padre en espera durante 20 segundos\n");
            sleep(20);

        while((childpid=waitpid(-1,&status,WUNTRACED | WCONTINUED))>0){ //El proceso padre espera a cualquier hijo, indicando además su estado
            if(WIFEXITED(status)){//El hijo terminó con normalidad
                printf("Hijo con PID %d finalizado, status= %d\n",(int)childpid, WEXITSTATUS(status));//Se imprime el estado del hijo
            }
            else if (WIFSIGNALED(status)){//Indica si el hijo terminó a causa de una señal no capturada
                printf("Hijo con PID %d finalizado al recibir la señal %d\n",(int)childpid, WTERMSIG(status));//Se devuelve el número de la señal que provocó la muerte del proceso hijo
            } 
            else if (WIFSTOPPED(status)){ //Para cuando se para un proceso. Con wait() no nos serviria.
                printf("Hijo con PID %d parado al recibir la señal %d\n",(int)childpid,WSTOPSIG(status));
            } 
            else if (WIFCONTINUED(status)){ //Para cuando se reanuda un proceso parado. Con wait() no nos serviria.
                printf("Hijo con PID %d reanudado\n",(int) childpid);		  
            }
        }

        if (childpid==(pid_t)-1 && errno==ECHILD){//Cuando no hay más hijos que esperar se devuelve -1 y se le da un valor a errno
            printf("\nNo hay mas hijos que esperar para el proceso padre %d. Valor de errno = %d, definido como: %s.\n\n", getpid(), errno, strerror(errno));
        }
        else{//Se imprime un mensaje de error si la llamada no tiene éxito
            printf("Error en la invocacion de wait o waitpid. Valor de errno = %d, definido como: %s.\n", errno, strerror(errno));
            exit(EXIT_FAILURE);
        }

        sleep(20);
        exit(EXIT_SUCCESS);//El proceso padre acaba ya que todo ha ido bien
    }
}