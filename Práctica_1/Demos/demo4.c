#include <stdio.h> 
#include <stdlib.h>
#include <unistd.h>

extern char **environ;

int main(int argc, char **argv) //Equivalente a (int argc, char *argv[]) 
{
    int i;
    
    printf("Lista de variables de entorno del proceso %s:\n",argv[0]);
    
    for (i=0; environ[i]!= NULL; i++) 
        printf("environ[%d] = %s\n", i, environ[i]);

    printf("\n");

    printf("El usuario que ha ejecutado este proceso tiene el UID: %d\n", getuid());

    printf("Tambien puede consultar el UID de su usuario desde el terminal mediante el comando \"id\" \n");
}