#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>
#include <semaphore.h>

int par=0,impar=0,sumaPar=0,sumaImpar=0;

pthread_mutex_t mutex=PTHREAD_MUTEX_INITIALIZER;

void * generateNumber(void *orden){
    int *cont=(int *)orden;
    int suma=0;
    int *res = malloc(sizeof(int));

    pthread_mutex_lock(&mutex);
    for(int i=0;i<5;i++){
        suma+=(rand()%10)+1;
    }
    
    if(*cont%2==0){
        par+=suma;
        sumaPar+=suma;
    }
    else{
        impar+=suma;
        sumaImpar+=suma;
    }

    *res=suma;
	
    printf("La hebra de orden de creación %d devolvió un valor de suma parcial de %d\n",*cont,*res);
    pthread_mutex_unlock(&mutex);
	
	pthread_exit((void *)res);
}

int main(int argc,char ** argv){
    if(argc!=2){
        printf("ERROR: Número de parámetros incorrecto\nEJEMPLO: ./ejercicio2 N (siendo N el nº de hilos que desea crear)\n");
        exit(EXIT_FAILURE);
    }

    srand(time(NULL));
    int N_hilos=atoi(argv[1]);
    pthread_t thr[N_hilos]; // Declaracion de un array de hilos
    int orden[N_hilos]; //Se declara un vector para el orden de creación

    for(int i=0;i<N_hilos;i++){
        orden[i]=i+1;
        if(pthread_create(&(thr[i]), NULL,(void *) generateNumber,(void *) &orden[i])){
            fprintf(stderr, "Error creating thread\n");
	        exit(EXIT_FAILURE);
        }
    }


    for(int i=0;i<N_hilos;i++){
        if(pthread_join(thr[i],NULL)){
            fprintf(stderr, "Error joining thread\n");
            exit(EXIT_FAILURE);  
        }
    }

    printf("\nValor de la variable compartida impar: %d\n",impar);
    printf("Suma de los valores devueltos por las hebras de creación impares: %d\n",sumaImpar);
    printf("\nValor de la variable compartida par: %d\n",par);
    printf("Suma de los valores devueltos por las hebras de creación pares: %d\n",sumaPar);

    exit(EXIT_SUCCESS);
}