#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "PL.h"

Z obtener_funcion_objetivo ()
{
	Z funcion_objetivo;
	//Obtener el criterio de optimización
	funcion_objetivo.criterio = criterio_funcion_objetivo ();
	//Obtener los coeficientes y variables de la F.O según el criterio
	funcion_objetivo = cuerpo_funcion_objetivo (funcion_objetivo);
	return funcion_objetivo;
}

int criterio_funcion_objetivo ()
{
	int criterio;
	printf ("\nSelecciona el criterio a aplicar:\n\n1. Maximizar\t\t2.Minimizar\t\t");
	scanf ("%d", &criterio);
	if (criterio == 2)
		return -1;
	else
		return 1;
}

Z cuerpo_funcion_objetivo (Z objetivo)
{
	Z aux;
	char numero_variables;													//Almacena el numero de variables
	char * coeficiente = (char *) malloc (sizeof (char));					//Almacena coeficiente a coeficiente
	float * coeficientes = (float *) malloc (sizeof (float));				//Almacena los coeficientes de la F.O
	char * variables = (char *) malloc (sizeof (char));						//Almacena las variables de la F.O
	char * funcion_objetivo = (char *) malloc (sizeof (char));				//Almacena la F.O
	char * ptr;																//Apuntador a la F.O para ciclo
	char i;																	//Auxiliar para manejar ciclos
	system ("cls");
	printf ("\nINGRESA LA FUNCI%cN OBJETIVO:\t", 224);
	scanf ("%s", funcion_objetivo);
	for (ptr = funcion_objetivo, i = 0, numero_variables = 0; * ptr != '\0'; ptr ++)
	{
		//COEFICIENTES
		if (((* ptr >= '0') && (* ptr <= '9')) || ((* ptr == '.') || (* ptr == '-')))
			coeficiente [i ++] = (* ptr);
		//VARIABLES
		if (((* ptr >= 'a') && (* ptr <= 'z')) || (* ptr >= 'A') && (* ptr <= 'Z'))
		{
			coeficiente [i] = ('\0');
			variables [numero_variables] = (* ptr);
			coeficientes [numero_variables] = (((float) atof (coeficiente)) * (objetivo.criterio));
			numero_variables ++;
			i = 0;
		}
	}
	variables [numero_variables] = ('\0');
	coeficientes [numero_variables] = ('\0');
	(aux.variables) = variables;
	(aux.coeficientes) = coeficientes;
	free (variables);
	free (coeficiente);
	free (coeficientes);
	return aux;
}

lista obtener_restricciones ()
{
	boolean otra = TRUE;													//Inicializar un booleano en verdadero 
	char i = 0;																//Contador para saber el número de restricciones
	lista restricciones;													//Lista para almacenar todas las restricciones
	restriccion res;														//Nodo que almacenará cada restricción
	Initialize (&restricciones);											//Inicializamos la lista de restricciones
	while (otra != FALSE)
	{
		res = cuerpo_restriccion (i + 1);
		Add (&restricciones, res);
		printf ("\n%cDeseas ingresar otra restricci%cn?\t\t1.Si\t\t0.No\t\t", 168, 162);
		scanf ("%d", &otra);
		i ++;
	}
	return restricciones;
}

restriccion cuerpo_restriccion (char numero_restriccion)
{
	restriccion res;
	char numero_variables;													//Almacena el numero de variables
	char * coeficiente = (char *) malloc (sizeof (char));					//Almacena coeficiente a coeficiente
	float * coeficientes = (float *) malloc (sizeof (float));				//Almacena los coeficientes de la F.O
	char * variables = (char *) malloc (sizeof (char));						//Almacena las variables de la F.O
	char * rest = (char *) malloc (sizeof (char));							//Almacena la restricción ingresada
	char * ptr;																//Apuntador a la restricción para ciclo
	char i;																	//Auxiliar para manejar ciclos
	printf ("\nINGRESA LA RESTRICCION %d: ", numero_restriccion);
	scanf ("%s", rest);
	for (ptr = rest, i = 0, numero_variables = 0; * ptr != '\0'; ptr ++)
	{
		//COEFICIENTES
		if (((* ptr >= '0') && (* ptr <= '9')) || ((* ptr == '.') || (* ptr == '-')))
			coeficiente [i ++] = * ptr;
		//VARIABLES
		if (((* ptr >= 'a') && (* ptr <= 'z')) || (* ptr >= 'A') && (* ptr <= 'Z'))
		{
			coeficiente [i] = '\0';
			variables [numero_variables] = (* ptr);
			coeficientes [numero_variables ++] = (float) atof (coeficiente);
			i = 0;
		}
		//COMPARADOR
		if ((* ptr == '>') || (* ptr == '<'))
			(res.comparador) = (* ptr);
	}
	coeficiente [i] = '\0';
	variables [numero_variables] = '\0';
	coeficientes [numero_variables] = '\0';
	(res.variables) = variables;
	(res.coeficientes) = coeficientes;
	(res.limite) = ((float) atof (coeficiente));
	free (rest);
	free (variables);
	free (coeficiente);
	free (coeficientes);
	return res;
}

void imprimir_problema_inicial (Z objetivo, lista * restricciones)
{
	restriccion r;
	int i, j, numero_restricciones;
	printf ("\n\nFUNCION OBJETIVO:\t");
	for (i = 0; i < strlen (objetivo.variables); i ++)
	{
		if (i == 0)
			printf ("%f%c%c", objetivo.coeficientes [i], 250, objetivo.variables [i]);
		else
		{
			if ((objetivo.coeficientes [i]) >= 0)
				printf ("+%f%c%c", objetivo.coeficientes [i], 250, objetivo.variables [i]);
			else
				printf ("%f%c%c", objetivo.coeficientes [i], 250, objetivo.variables [i]);
		}
	}
	printf ("\n\nRESTRICCIONES:\n");
	numero_restricciones = Size (restricciones);
	if (numero_restricciones > 0)
	{
		for (i = 0; i < numero_restricciones; i ++)
		{
			printf ("\nRestriccion %d:\t", i + 1);
			r = Element (restricciones, i + 1);
			for (j = 0; j < strlen (r.variables); j ++)
			{
				if (j == 0)
					printf ("%f%c%c", r.coeficientes [j], 250, r.variables [j]);
				else
				{
					if ((r.coeficientes [j]) >= 0)
						printf ("+%f%c%c", r.coeficientes [j], 250, r.variables [j]);
					else
						printf ("%f%c%c", r.coeficientes [j], 250, r.variables [j]);
				}
			}
			printf (" %c= %f", r.comparador, r.limite);
		}
	}else
		printf ("\n\nNo existen restricciones\n\n");
	return;
}
