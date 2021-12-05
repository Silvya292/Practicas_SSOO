#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>

#define tamBuff 5

sem_t s;
sem_t cn;
sem_t pr;

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

void * productor(void * nIt){
    int *nItproducer=(int *) nIt;
    while(*nItproducer>0){
        int n=(rand()%10)+1;
        sem_wait(&pr);
        sem_wait(&s);
        addDato(n);
        printf("→ Productor %ld: Insertado %d en el buffer\n",pthread_self(),n);
        imprimeVector();
        sem_post(&s);
        sem_post(&cn);
        --*nItproducer;
    }
    pthread_exit(NULL);
}

void * consumidor(void * nIt){
    int *nItconsumer=(int *) nIt;
    while(*nItconsumer>0){
        sem_wait(&cn);
        sem_wait(&s);
        printf("← Consumidor %ld: Extraido %d del buffer\n",pthread_self(),extractDato());
        imprimeVector();
        sem_post(&s);
        sem_post(&pr);
        --*nItconsumer;
    }
    pthread_exit(NULL);
}


int main(int argc, char **argv){
    sem_init(&s,0,1);
    sem_init(&cn,0,0);
    sem_init(&pr,0,5);

    for(int i=0;i<tamBuff;i++){
        buffer[i]=0;
    }

    srand(time(NULL));
    pthread_t thr[2];
    int nIt[2];

    for(int i=0;i<2;i++){
        nIt[i]=10;
    }

    if(pthread_create(&(thr[0]), NULL,(void *) productor,(void *) &nIt[0])){
            fprintf(stderr, "Error creating thread\n");
	        exit(EXIT_FAILURE);
    }

    if(pthread_create(&(thr[1]), NULL,(void *) consumidor,(void *) &nIt[1])){
            fprintf(stderr, "Error creating thread\n");
	        exit(EXIT_FAILURE);
    }

    for(int i=0;i<2;i++){
        if(pthread_join(thr[i],NULL)){
            fprintf(stderr, "Error joining thread\n");
            exit(EXIT_FAILURE);  
        }
    }

}