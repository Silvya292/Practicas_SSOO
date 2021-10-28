#include	<signal.h>
#include	<stdio.h>
#include	<unistd.h>
#include <stdlib.h>
#include <errno.h>

int i=0;

void sig_alarm(int signo)
{
	printf("RING!\n");
    i++;

    if(i==4){
        kill(getpid(), SIGKILL);
    }

}

unsigned int f1(int nsecs)
{
	alarm(nsecs);		
	pause();
}

int main (){

	/* Si la solicitud de tratamiento de senial se puede llevar a cabo, la funcion signal() devolverá 
	el nombre de la función (* void) que la tratará, en caso contrario se devuelve el valor de la macro
	SIG_ERR y se pone errno a un valor positivo*/ 

	if (signal(SIGALRM, sig_alarm) == SIG_ERR)
	{
		perror("Signal error");
   	    printf("errno value= %d\n", errno);  
		exit(EXIT_FAILURE);
	}

    printf ("Una alarma sonará en 5 segundos...\n");
    f1(5);
    printf ("Una alarma sonará en 3 segundos...\n");
    f1(3);
    while (1){
        f1(1);
    }

    printf ("Fin del programa\n"); //Este mensaje no aparecerá ya que el hijo es matado antes
    exit(EXIT_SUCCESS);
}