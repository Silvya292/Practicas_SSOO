#include <stdio.h> 
#include <stdlib.h> 
#include <pthread.h>

//Declaración de una estructura 
struct param
{
    char frase[30];
    int numero; 
};

/*Función que se asignará a los hilos que se creen. Recibe un puntero a estructura */ 
void *hiloMensaje (void * mensa)
{
   	struct param *aux = (struct param *) mensa; 
	printf("%s %d\n", aux->frase, aux->numero);
    
    //Las lineas anteriores se pueden sustituir por esta otra
    //printf("%s %d\n", ((struct param *) mensa)->frase, ((struct param *) mensa)->numero); 

	pthread_exit(NULL); //Fin de la hebra sin devolver nada
}

int main() 
{    
	//Declaración de dos hebras, hilos o procesos ligeros. NO CREACION 
	pthread_t thd1, thd2; 

	// Inicializacion de 2 estructuras de tipo “struct param”
	struct param param1 = {"Soy el hilo", 1};
	struct param param2 = {"Soy el hilo", 2};

	/*Creamos dos hilos. La función la pasaremos como (void *) nombreFuncion. Es decir, hacemos un casting a (void *), aunque por defecto no es necesario, ya que el nombre de una función es su dirección de memoria. También es importante realizar esto con la dirección de memoria de la variable que contiene los parámetros que se le pasan a la función */
	if( pthread_create (&thd1, NULL, (void *) hiloMensaje, (void *) &param1) )
    {
	  fprintf(stderr, "Error creating thread\n");
	  exit(EXIT_FAILURE); 
	}

	if( pthread_create (&thd2, NULL, (void *) hiloMensaje, (void *) &param2) )
	{
	  fprintf(stderr, "Error creating thread\n");
	  exit(EXIT_FAILURE); 
	}

	/*Esperamos la finalización de los hilos. Si la función devolviera algo habría que recogerlo con el segundo argumento, que en este caso esta a NULL. Cuando el segundo argumento no es NULL, se recogen los resultados que vienen de pthread_exit(), que se explicará en las siguientes demos.*/
	if( pthread_join(thd1, NULL) )
	{
	  fprintf(stderr, "Error joining thread\n");
	  exit(EXIT_FAILURE);  
	}

	if( pthread_join(thd2, NULL) )
	{
	  fprintf(stderr, "Error joining thread\n");
	  exit(EXIT_FAILURE);  
	}

	/*Si no se ponen estos join() en el programa principal y simplemente lanzamos los dos hilos y finalizamos, lo más probable es que los hilos no lleguen a ejecutarse completamente o incluso que no lleguen ni a empezar antes de que el programa principal termine.*/
	printf("Hilo principal: han finalizado las threads.\n");
	exit(EXIT_SUCCESS); 
}
