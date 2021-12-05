#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>

#define tamBuff 3

sem_t s;
sem_t cn;
sem_t pr;

int hebraProductor=0;
int hebraConsumidor=0;

int buffer[tamBuff];
int indexin=0;
int indexout=0;

void imprimeVector(){
    printf("Buffer actual: [ ");
    for(int i=0;i<tamBuff;i++){
        printf("%d ",buffer[i]);
    }
    printf("]\n\n");
}

void addDato(int n){
    buffer[indexin]=n;
    indexin=(indexin+1)%tamBuff;
}

int extractDato(){
    int n=buffer[indexout];
    buffer[indexout]=0;
    indexout=(indexout+1)%tamBuff;
    return n;
}

void * productor(){
    int n=(rand()%10)+1;
    sem_wait(&pr);
    sem_wait(&s);
    hebraProductor++;
    addDato(n);
    printf("→ Productor %d (%ld): Insertado %d en el buffer\n",hebraProductor,pthread_self(),n);
    imprimeVector();
    sem_post(&s);
    sem_post(&cn);
    
    pthread_exit(NULL);
}

void * consumidor(){
    sem_wait(&cn);
    sem_wait(&s);
    hebraConsumidor++;
    printf("← Consumidor %d (%ld): Extraido %d del buffer\n",hebraConsumidor,pthread_self(),extractDato());
    imprimeVector();
    sem_post(&s);
    sem_post(&pr);

    pthread_exit(NULL);
}

int main(int argc, char **argv){
    sem_init(&s,0,1);
    sem_init(&cn,0,0);
    sem_init(&pr,0,tamBuff);

    for(int i=0;i<tamBuff;i++){
        buffer[i]=0;
    }

    srand(time(NULL));
    pthread_t thr[6];

    for(int i=0;i<3;i++){
        if(pthread_create(&(thr[i]), NULL,(void *) productor, NULL)){
            fprintf(stderr, "Error creating thread\n");
	        exit(EXIT_FAILURE);
        }
    }

    for(int i=3;i<6;i++){
        if(pthread_create(&(thr[i]), NULL,(void *) consumidor, NULL)){
            fprintf(stderr, "Error creating thread\n");
	        exit(EXIT_FAILURE);
        }
    }

    for(int i=0;i<6;i++){
        if(pthread_join(thr[i],NULL)){
            fprintf(stderr, "Error joining thread\n");
            exit(EXIT_FAILURE);  
        }
    }

}