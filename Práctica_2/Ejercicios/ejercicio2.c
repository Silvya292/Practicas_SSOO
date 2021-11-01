#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <pthread.h>

int nfile=0;

void * nlineas(void * fichero){
    int * nlineas=malloc(sizeof(int)); //Se reserva memoria
    *nlineas=0;

    FILE* f=fopen((char *) fichero,"r"); //Se abren los ficheros pasados como argumento en modo lectura
    if(f==NULL){
        printf("ERROR: No se ha podido abrir el archivo\n");
        exit(EXIT_FAILURE);
    }

    char cadena[256];
    while (fgets(cadena, 256, f)!=NULL){ //Se van leyendo cada una de las líneas del fichero
        ++*nlineas;
    }
    nfile++;
    char * filename=(char *) fichero; //Se realiza un casting al nombre del fichero pasado como argumento
    printf("Nº de líneas del fichero %dº (%s): %d\n",nfile,filename,*nlineas); //Se imprimen las sumas parciales
    fclose(f); //Se cierra el fichero
    pthread_exit((void *)nlineas); //Se devuelve un puntero void* que apunta a la suma parcial de líneas del fichero leido
}

int main(int argc,char ** argv){
    if(argc<2){
        printf("ERROR: Número de parámetros incorrecto\nEJEMPLO: ./ejercicio1 ficheroX ficheroY ...\n");
        exit(EXIT_FAILURE);
    }

    srand(time(NULL));
    int N_hebras=argc-1; //Número de hebras introducido por terminal
    pthread_t thr[N_hebras]; // Declaracion de un Array de hebras  

    for(int i=0;i<N_hebras;i++){
        if(pthread_create(&(thr[i]), NULL, nlineas, (void*) argv[i+1])){ //Se crean N_hebras y se envían a la función nlineas() con cada uno de los ficheros
            fprintf(stderr, "Error creating thread\n");
	        exit(EXIT_FAILURE);
        }
    }

    int suma=0;
    int *ret;

    for(int i=0;i<N_hebras;i++){ 
        if(pthread_join(thr[i],(void**) &ret)){ //Se esperan a las N_hebras y se devuelve un puntero al resultado de las sumas parciales
            fprintf(stderr, "Error joining thread\n");
            exit(EXIT_FAILURE);  
        }

        suma+=*ret; //Se almacena en la variable suma la dirección del puntero ret que apunta al valor de cada una de las sumas parciales
    }

    printf("La suma de todos las líneas de los ficheros indicados es: %d\n",suma);
    exit(EXIT_SUCCESS);

}