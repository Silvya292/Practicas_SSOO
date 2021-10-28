#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>

int cont=1;

typedef struct{
    int *v;
    int nEle;
}Vector;

int * reservaMemoria(int nEle){
    int *vector;
    if((vector=(int *)malloc(nEle*sizeof(int)))==NULL){
        printf("ERROR: No se ha podido reserva memoria para el vector");
        exit(EXIT_FAILURE);
    }
    return vector;
}

void rellenaVector(int *v,int nEle){
    srand(time(NULL));
    for(int i=0;i<nEle;i++){
        v[i]=(rand()%9)+1;
    }
}

void imprimeVector(int *v,int nEle){
    printf("v=[");
    for(int i=0;i<nEle-1;i++){
        printf("%d, ",v[i]);
    }
    printf("%d]\n\n",v[nEle-1]);
}

Vector * creaHebras(int * vPadre, int N_hebras){
    Vector *vHijos=(Vector *)malloc(N_hebras*sizeof(Vector));
    int nEleHijos=10/N_hebras;
    for(int i=0;i<N_hebras;i++){
        vHijos[i].v=reservaMemoria(nEleHijos);
        vHijos[i].nEle=nEleHijos;
        int position=0;
        for(int j=(i*nEleHijos);j<((i*nEleHijos)+nEleHijos);j++){
            vHijos[i].v[position]=vPadre[j];
            position++;
        }
        printf("El vector hijo nº%d es:\n",cont);
        imprimeVector(vHijos[i].v,vHijos->nEle);
        cont++;        
    }
    return vHijos;
}

void * suma_hebras(void * vHijos){
    int * sumap = malloc(sizeof(int));
    *sumap=0;
    Vector *v=(Vector *)vHijos;
    for(int i=0;i<v->nEle;i++){
        *sumap=*sumap+v->v[i];
    }
    printf("Suma parcial de los elementos del vector (hebra %ld): %d\n",(long int) pthread_self(),*sumap);
	pthread_exit((void *)sumap);
}

int main(int argc, char ** argv){
    if(argc<2 || (atoi(argv[1])!=2 && atoi(argv[1])!=5)){
        printf("ERROR: No se ha realizado la llamada correcta al programa.\n");
        printf("Ejemplo de ejecución: ./ejercicio3 2 o ./ejercicio3 5\n");
        exit(EXIT_FAILURE);
    }

    int N_hebras=atoi(argv[1]);
    
    int * vPadre=reservaMemoria(10);
    rellenaVector(vPadre,10);
    printf("El vector inicial es: \n");
    imprimeVector(vPadre,10);

    Vector *vHijos=creaHebras(vPadre,N_hebras);

    pthread_t thr[N_hebras]; // Declaracion de un Array de hebras  

    for(int i=0;i<N_hebras;i++){
        if(pthread_create(&(thr[i]), NULL, suma_hebras, (void *) vHijos+(sizeof(Vector)*i))){
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
    printf("\nLa suma de todos los elementos del vector padre es: %d\n",suma);
    exit(EXIT_SUCCESS);
}