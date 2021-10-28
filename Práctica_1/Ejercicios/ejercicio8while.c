#include <unistd.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h> 

void sig_pid(){
    printf("Mi PID es %d",getpid());
}

int main(){
    printf("Mi PID es %d\n",getpid());
    if (signal(SIGUSR1, sig_pid) == SIG_ERR){
        perror("Signal error");
        printf("errno value= %d\n", errno);  
        exit(EXIT_FAILURE);
	}
    while(1){
        pause();
    }
    exit(EXIT_SUCCESS);
}