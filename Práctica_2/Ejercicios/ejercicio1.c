#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <pthread.h>

void * suma_hebras(){
    printf("Hebra %ld:\n",(unsigned long) pthread_self());
    float a=(float)rand()/(float)(RAND_MAX);
    printf("Primer número: %f\n",a);
    float b=(float)rand()/(float)(RAND_MAX);
    printf("Segundo número: %f\n",b);

    float * res = malloc(sizeof(float));
	*res=a+b;
    printf("Suma parcial: %f\n\n",*res);
	
	pthread_exit((void *)res);
}

int main(int argc,char ** argv){
    if(argc!=2){
        printf("ERROR: Número de parámetros incorrecto\nEJEMPLO: ./ejercicio1 N (siendo N el nº de hebras que desea crear)\n");
        exit(EXIT_FAILURE);
    }

    srand(time(NULL));
    int N_hebras=atoi(argv[1]);
    pthread_t thr[N_hebras]; // Declaracion de un Array de hebras  

    for(int i=0;i<N_hebras;i++){
        if(pthread_create(&(thr[i]), NULL,(void *) suma_hebras, NULL)){
            fprintf(stderr, "Error creating thread\n");
	        exit(EXIT_FAILURE);
        }
    }

    float suma=0;
    float *ret;

    for(int i=0;i<N_hebras;i++){
        if(pthread_join(thr[i],(void**) &ret)){
            fprintf(stderr, "Error joining thread\n");
            exit(EXIT_FAILURE);  
        }

        suma+=*ret;
    }

    printf("La suma de todos los números generados por las hebras es: %f\n",suma);
    exit(EXIT_SUCCESS);
}