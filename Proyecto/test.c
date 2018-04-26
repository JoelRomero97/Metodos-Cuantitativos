#include <stdio.h>
#include <stdlib.h>
#include "PL.h"

int main(int argc, char const *argv[])
{
	restriccion r;
	restriccion aux;
	r.limite = 500;
	char x;
	r.comparador = '>';
	for (int i = 0, x='q'; i < 2; i++,x++)
	{
		r.coeficientes[i] = x;
		r.variables[i] = i+1;
	}
	printf("Original\n");
	for (int i = 0; i < 2; i++)
	{
		printf("%f%c",r.coeficientes[i],r.variables[i]);
	}
	printf("%c%f",r.comparador,r.limite);
	aux = invertir_restriccion(r);
	printf("Inversion \n");
	for (int i = 0; i < 2; i++)
	{
		printf("%f%c",aux.coeficientes[i],aux.variables[i]);
	}
	printf("%c%f",aux.comparador,aux.limite);
	return 0;
}

