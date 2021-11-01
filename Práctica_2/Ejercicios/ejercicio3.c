#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>

int cont=1;

typedef struct{ //Se define un struct que almacenará un puntero a los elementos de cada uno de los vectores y un entero con el nº de elementos del vector
    int *v;
    int nEle;
}Vector;

int * reservaMemoria(int nEle){ //Función que reservará memoria para cada uno de los vectores creados
    int *vector;
    if((vector=(int *)malloc(nEle*sizeof(int)))==NULL){
        printf("ERROR: No se ha podido reserva memoria para el vector");
        exit(EXIT_FAILURE);
    }
    return vector;
}

void rellenaVector(int *v,int nEle){ //Función que rellena el vector padre con 10 nº aleatorios
    srand(time(NULL));
    for(int i=0;i<nEle;i++){
        v[i]=(rand()%9)+1;
    }
}

void imprimeVector(int *v,int nEle){ //Función que imprimirá cada uno de los vectores
    printf("v=[");
    for(int i=0;i<nEle-1;i++){
        printf("%d, ",v[i]);
    }
    printf("%d]\n\n",v[nEle-1]);
}

Vector * creaHijos(int * vPadre, int N_hebras){ //Función que crea a partir del vector padre N vectores hijo para los N vectores necesarios
    Vector *vHijos=(Vector *)malloc(N_hebras*sizeof(Vector)); //Se reserva memoria para cada uno de los vectores creados
    int nEleHijos=10/N_hebras; //Se define el nº de elementos de cada uno de los vectores
    for(int i=0;i<N_hebras;i++){
        vHijos[i].v=reservaMemoria(nEleHijos); //Se reserva memoria para cada uno de los vectores hijo
        vHijos[i].nEle=nEleHijos;
        int position=0;
        for(int j=(i*nEleHijos);j<((i*nEleHijos)+nEleHijos);j++){
            vHijos[i].v[position]=vPadre[j]; //Se asocia a cada uno de los vectores hijo los N elementos que toman del vector padre
            position++;
        }
        printf("El vector hijo nº%d es:\n",cont);
        imprimeVector(vHijos[i].v,vHijos->nEle); //Se imprimen cada uno de los vectores creados
        cont++;        
    }
    return vHijos;
}

void * suma_hebras(void * vHijos){ //Función que realizará la suma parcial de cada uno de los vectores que componen las N hebras
    int * sumap = malloc(sizeof(int));
    *sumap=0;
    Vector *v=(Vector *)vHijos;
    for(int i=0;i<v->nEle;i++){
        *sumap=*sumap+v->v[i];
    }
    printf("Suma parcial de los elementos del vector (hebra %ld): %d\n",(long int) pthread_self(),*sumap);
	pthread_exit((void *)sumap); //Se devuelve un puntero void* que apunta al valor de la suma parcial de la hebra
}

int main(int argc, char ** argv){
    if(argc<2 || (atoi(argv[1])!=2 && atoi(argv[1])!=5)){
        printf("ERROR: No se ha realizado la llamada correcta al programa.\n");
        printf("Ejemplo de ejecución: ./ejercicio3 2 o ./ejercicio3 5\n");
        exit(EXIT_FAILURE);
    }

    int N_hebras=atoi(argv[1]); //Se hace casting a entero del número de hebras necesarias
    
    int * vPadre=reservaMemoria(10); //Se reserva memoria para el vector padre
    rellenaVector(vPadre,10); //Se rellena el vector padre de nº aleaorios
    printf("El vector inicial es: \n");
    imprimeVector(vPadre,10); //Se imprime el vector padre

    Vector *vHijos=creaHijos(vPadre,N_hebras); //Se crean N vectores hijo dependiendo del valor insertado por terminal

    pthread_t thr[N_hebras]; //Declaración de un Array de hebras  

    for(int i=0;i<N_hebras;i++){
        if(pthread_create(&(thr[i]), NULL, suma_hebras, (void *) vHijos+(sizeof(Vector)*i))){ //Se crean N hebras que ejecutarán la función suma_hebras() y se les envía un puntero void* al comienzo del vector hijo i
            fprintf(stderr, "Error creating thread\n");
	        exit(EXIT_FAILURE);
        }
    }

    int suma=0;
    int *ret;

    for(int i=0;i<N_hebras;i++){
        if(pthread_join(thr[i],(void**) &ret)){ //Se esperan a las N hebras y se devuelve un puntero al resultado de las sumas parciales
            fprintf(stderr, "Error joining thread\n");
            exit(EXIT_FAILURE);  
        }

        suma+=*ret; //Se almacena en la variable suma la dirección del puntero ret que apunta al valor de cada una de las sumas parciales
    }
    printf("\nLa suma de todos los elementos del vector padre es: %d\n",suma);
    exit(EXIT_SUCCESS);
}