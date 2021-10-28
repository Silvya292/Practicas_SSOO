#include <pthread.h> 
#include <stdio.h>
#include <stdlib.h>	

void * hebra(void * x_void_ptr)
{
	//incrementar x hasta 100
	int *x_ptr = (int *) x_void_ptr; 
   
    printf("Hebra hija: valor de x=%d, incrementandolo 100 veces.\n", *x_ptr);

	while(*x_ptr < 100)
	{
	   *x_ptr=*x_ptr+1;
	} 

	printf("Hebra hija: incremento de x finalizado.\n");
 
	pthread_exit(NULL); 
}

int main()
{
	int x = 0, y = 0;

	//Declaración de una hebra
	pthread_t hebra_incr;

	printf("Hebra principal: valor de variable x: %d\n", x);
    printf("Hebra principal: valor de variable y: %d\n", y);
	
	//Creación de la hebra
	if(pthread_create(&hebra_incr, NULL, (void *) hebra, (void *) &x)) 
	{
	  fprintf(stderr, "Error creating thread\n");
	  exit(EXIT_FAILURE); 
	}
   
    printf("Hebra principal: incrementando y 100 veces.\n");
	//Incremetar y hasta 100
	while(y < 100)
	{
	   y=y+1;
	}

	printf("Hebra principal: incremento de y finalizado.\n");

	//Espera de la hebra
	if(pthread_join(hebra_incr, NULL)) 
	{
	  fprintf(stderr, "Error joining thread\n");
	  exit(EXIT_FAILURE);  
	}

	printf("Hebra principal: Hilo hijo recogido con pthread_join(), valor de x: %d\n",x);
	printf("Hebra principal: Hilo hijo recogido con pthread_join(), valor de y: %d\n",y);
	exit(EXIT_SUCCESS);
}
