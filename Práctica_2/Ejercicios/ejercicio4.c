#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

int variable=0;

void * suma_hebras(){
    for(int i=0;i<10000;i++){
        variable++;
    }
	pthread_exit(NULL);
}

int main(){
    pthread_t hebra1, hebra2;
    printf("Hebra principal (main), valor de la variable: %d\n",variable);

    if(pthread_create(&hebra1, NULL,(void *) suma_hebras, NULL)){
        fprintf(stderr, "Error creating thread\n");
	    exit(EXIT_FAILURE);
    }
    if(pthread_create(&hebra2, NULL,(void *) suma_hebras, NULL)){
        fprintf(stderr, "Error creating thread\n");
	    exit(EXIT_FAILURE);
    }

    if(pthread_join(hebra1,NULL)){
        fprintf(stderr, "Error joining thread\n");
        exit(EXIT_FAILURE);  
    }
    if(pthread_join(hebra2,NULL)){
        fprintf(stderr, "Error joining thread\n");
        exit(EXIT_FAILURE);  
    }
    printf("El valor de la variable global es ahora: %d\n",variable);
    exit(EXIT_SUCCESS);
}