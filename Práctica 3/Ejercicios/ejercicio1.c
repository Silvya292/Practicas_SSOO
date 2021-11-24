#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>
#include <semaphore.h>

typedef struct{
    int *vector;
    int nEle;
}Camisetas;

pthread_mutex_t mutex=PTHREAD_MUTEX_INITIALIZER;

bool comprueba(int num){
    if(num<0){
        return false;
    }
    return true;
}

int * reservaMemoria(int nEle){
    int *vector;
    if((vector=(int *)malloc(nEle*sizeof(int)))==NULL){
        printf("ERROR: No se ha podido reserva memoria para el vector");
        exit(EXIT_FAILURE);
    }
    return vector;
}

void rellenarStock(int * vector, int nEle){
    for(int i=0;i<nEle;i++){
        vector[i]=(rand()%100)+1;
    }
}

void imprimirStock(int * vector,int nEle){
    printf("Stock de tipos de camisetas: %d\n",nEle);
    for(int i=0;i<nEle;i++){
        printf("Camiseta nº%d: %d\n",i+1,vector[i]);
    }
    printf("\n");
}

void * compraCamisetas(void * vectorCamisetas){
    Camisetas * stock;
    stock=(Camisetas *) vectorCamisetas;
    int modelo=(rand()%(stock->nEle));
    int cantidad=(rand()%10)+1;
    pthread_mutex_lock(&mutex);
    int newStock=stock->vector[modelo]-cantidad;
    if(newStock<0){
        printf("<--(CLIENTE): El cliente %ld ha comprado %d unidades de la camiseta nº%d\n",pthread_self(),stock->vector[modelo],modelo+1);
        stock->vector[modelo]=0;
        pthread_mutex_unlock(&mutex);
        pthread_exit(NULL);
    }
    stock->vector[modelo]=newStock;
    printf("<--(CLIENTE): El cliente %ld ha comprado %d unidades de la camiseta nº%d\n",pthread_self(),cantidad,modelo+1);
    pthread_mutex_unlock(&mutex);
    pthread_exit(NULL);
}

void * supplyCamisetas(void * vectorCamisetas){
    Camisetas * stock;
    stock=(Camisetas *) vectorCamisetas;
    int modelo=(rand()%(stock->nEle));
    int cantidad=(rand()%10)+1;
    pthread_mutex_lock(&mutex);
    stock->vector[modelo]+=cantidad;
    printf("-->(PROVEEDOR): El proveedor %ld ha suministrado %d unidades de la camiseta nº%d\n",pthread_self(),cantidad,modelo+1);
    pthread_mutex_unlock(&mutex);
    pthread_exit(NULL);
}

int main(int argc, char * argv[]){
    if(argc<3){
        printf("ERROR: Número de argumentos incorrecto\n");
        printf("Inicialice el programa del siguiente modo: ./ejercicio1 (Nº de clientes) (Nº de proveedores)\n");
        exit(EXIT_FAILURE);
    }

    int nclientes,nproveedores;
    nclientes=atoi(argv[1]);
    if(!comprueba(nclientes)){
        printf("Debe insertar un número de clientes válido\n");
        exit(EXIT_FAILURE);
    }
    nproveedores=atoi(argv[2]);
    if(!comprueba(nproveedores)){
        printf("Debe insertar un número de proveedores válido\n");
        exit(EXIT_FAILURE);
    }
    Camisetas stock;
    stock.nEle=nproveedores;
    stock.vector=reservaMemoria(stock.nEle);

    srand(time(NULL));
    rellenarStock(stock.vector,stock.nEle);
    printf("------APERTURA DE LA TIENDA----\n");
    imprimirStock(stock.vector,stock.nEle);

    pthread_t thrClients[nclientes]; //Declaración de un Array de hebras  

    for(int i=0;i<nclientes;i++){
        if(pthread_create(&(thrClients[i]), NULL, (void *) compraCamisetas, (void *) &stock)){ //Se crean N hebras que ejecutarán la función suma_hebras() y se les envía un puntero void* al comienzo del vector hijo i
            fprintf(stderr, "Error creating thread\n");
	        exit(EXIT_FAILURE);
        }
    }

    pthread_t thrSuppliers[nproveedores];

    for(int i=0;i<nproveedores;i++){
        if(pthread_create(&(thrSuppliers[i]), NULL, (void *) supplyCamisetas, (void *) &stock)){ //Se crean N hebras que ejecutarán la función suma_hebras() y se les envía un puntero void* al comienzo del vector hijo i
            fprintf(stderr, "Error creating thread\n");
	        exit(EXIT_FAILURE);
        }
    }

    for(int i=0;i<nclientes;i++){
        if(pthread_join(thrClients[i],NULL)){ //Se esperan a las N hebras y se devuelve un puntero al resultado de las sumas parciales
            fprintf(stderr, "Error joining thread\n");
            exit(EXIT_FAILURE);  
        }
    }

    for(int i=0;i<nproveedores;i++){
        if(pthread_join(thrSuppliers[i],NULL)){ //Se esperan a las N hebras y se devuelve un puntero al resultado de las sumas parciales
            fprintf(stderr, "Error joining thread\n");
            exit(EXIT_FAILURE);  
        }
    }

    printf("\n------CIERRE DE LA TIENDA-----\n");
    imprimirStock(stock.vector,stock.nEle);
}