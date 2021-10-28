//Factorial de un número

#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

int factorial(int n){
    int res=1;
    for(int i=n;i>0;i--){
        sleep(1);
        res=res*i;
        printf("Factorial de %d: %d...\n",n,res);
    }
    return res;
}

int main(int argc, char **argv){
    if (argc<2){ //Si no se indica el número, se imprime un mensaje de error y finaliza el programa
        printf("Error: El número de argumentos insertados no es el correcto\n");
        printf("EJEMPLO DE EJECUCIÓN: ./prueba n (siendo n el número del que desee calcular el factorial.\n");
        exit(EXIT_FAILURE);
    }

    printf("El factorial de %d es: %d\n",atoi(argv[1]),factorial(atoi(argv[1])));
}