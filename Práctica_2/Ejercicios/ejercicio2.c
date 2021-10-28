#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <pthread.h>

int nfile=0;

void * nlineas(void * fichero){
    int * nlineas=malloc(sizeof(int));
    *nlineas=0;

    FILE* f=fopen((char *) fichero,"r");
    if(f==NULL){
        printf("ERROR: No se ha podido abrir el archivo\n");
        exit(EXIT_FAILURE);
    }

    char cadena[256];
    while (fgets(cadena, 256, f)!=NULL){
        ++*nlineas;
    }
    nfile++;
    char * filename=(char *) fichero;
    printf("Nº de líneas del fichero %dº (%s): %d\n",nfile,filename,*nlineas);
    fclose(f);
    pthread_exit((void *)nlineas);
}

int main(int argc,char ** argv){
    if(argc<2){
        printf("ERROR: Número de parámetros incorrecto\nEJEMPLO: ./ejercicio1 ficheroX ficheroY ...\n");
        exit(EXIT_FAILURE);
    }

    srand(time(NULL));
    int N_hebras=argc-1;
    pthread_t thr[N_hebras]; // Declaracion de un Array de hebras  

    for(int i=0;i<N_hebras;i++){
        if(pthread_create(&(thr[i]), NULL, nlineas, (void*) argv[i+1])){
            fprintf(stderr, "Error creating thread\n");
	        exit(EXIT_FAILURE);
        }
    }

    int suma=0;
    int *ret;

    for(int i=0;i<N_hebras;i++){
        if(pthread_join(thr[i],(void**) &ret)){
            fprintf(stderr, "Error joining thread\n");
            exit(EXIT_FAILURE);  
        }

        suma+=*ret;
    }

    printf("La suma de todos las líneas de los ficheros indicados es: %d\n",suma);
    exit(EXIT_SUCCESS);

}