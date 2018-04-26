#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "PL.h"

Condiciones_AG obtener_condiciones_iniciales ()
{
	Condiciones_AG auxiliar;												//Estructura para almacenar las condiciones del problema
	system (clear);
	printf ("\n______________________________ALGORITMOS GENÉTICOS______________________________\n");
	printf ("\n\nIngresa el tiempo máximo de procesamiento (minutos):\t");
	scanf ("%hhd", &auxiliar.tiempo_maximo);
	printf ("\n\nIngresa el error máximo permitido (porcentaje):\t");
	scanf ("%f", &auxiliar.error_maximo);
	auxiliar.error_maximo = (auxiliar.error_maximo * 0.01);
	printf ("\n\nIngresa el número máximo de iteraciones:\t");
	scanf ("%d", &auxiliar.it_max);
	printf ("\n\nIngresa el número de bits de precisión deseados:\t");
	scanf ("%hhd", &auxiliar.bits_precision);
	printf ("\n\nIngresa el número de integrantes por población:\t");
	scanf ("%hhd", &auxiliar.integrantes);
}

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
	printf ("\n\nSelecciona el criterio a aplicar:\t1. Maximizar\t\t2.Minimizar\n\n");
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
	system (clear);
	printf ("\nINGRESA LA FUNCIÓN OBJETIVO:\t");
	scanf ("%s", funcion_objetivo);
	//Se recorre la función objetivo en busca de números (coeficientes) o letras (variables)
	for (ptr = funcion_objetivo, i = 0, numero_variables = 0; * ptr != '\0'; ptr ++)
	{
		//COEFICIENTES (Con punto decimal y negativos)
		if ((((* ptr) >= '0') && ((* ptr) <= '9')) || ((* ptr) == '.') || ((* ptr) == '-'))
			coeficiente [i ++] = (* ptr);
		//VARIABLES (Letras mayúsculas y/o minúsculas)
		if ((((* ptr) >= 'a') && ((* ptr) <= 'z')) || (((* ptr) >= 'A') && ((* ptr) <= 'Z')))
		{
			//Si se encuentra una variable, ya terminó el coeficiente de dicha variable
			coeficiente [i] = ('\0');
			//En las variables se almacena la variable leída
			variables [numero_variables] = (* ptr);
			//En los coeficientes se almacena el número leído convertido a flotante
			coeficientes [numero_variables ++] = (((float) atof (coeficiente)) * (objetivo.criterio));
			//Se comienza a almacenar un nuevo coeficiente
			i = 0;
		}
	}
	//Se inicializan los apuntadores de la estructura de la F.O dependiendo el número de variables leídas
	(aux.variables) = (char * ) malloc (sizeof (char) * numero_variables);
	(aux.coeficientes) = (float * ) malloc (sizeof (float) * numero_variables);
	//Se guarda el valor de los coeficientes y la variable correspondiente en la estructura de la F.O
	for (i = 0; i < numero_variables; i ++)
	{
		(aux.variables [i]) = variables [i];
		(aux.coeficientes [i]) = coeficientes [i];
	}
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
		//Se obtienen los coeficientes y las variables de la restricción i-ésima
		res = cuerpo_restriccion (i + 1);
		//Se almacena esa restricción en una lista de restricciones
		Add (&restricciones, res);
		printf ("\n¿Deseas ingresar otra restricción?\t\t1.Si\t\t0.No\t\t");
		scanf ("%hhd", &otra);
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
	printf ("\nINGRESA LA RESTRICCIÓN %d: ", numero_restriccion);
	scanf ("%s", rest);
	//Se recorre la restricción en busca de números (coeficientes) o letras (variables)
	for (ptr = rest, i = 0, numero_variables = 0; * ptr != '\0'; ptr ++)
	{
		//COEFICIENTES (Con punto decimal y negativos)
		if ((((* ptr) >= '0') && ((* ptr) <= '9')) || ((* ptr) == '.') || ((* ptr) == '-'))
			coeficiente [i ++] = * ptr;
		//VARIABLES (Letras mayúsculas y/o minúsculas)
		if ((((* ptr) >= 'a') && ((* ptr) <= 'z')) || ((* ptr) >= 'A') && ((* ptr) <= 'Z'))
		{
			//Si se encuentra una variable, ya terminó el coeficiente de dicha variable
			coeficiente [i] = '\0';
			//En las variables se almacena la variable leída
			variables [numero_variables] = (* ptr);
			//En los coeficientes se almacena el número leído convertido a flotante
			coeficientes [numero_variables ++] = (float) atof (coeficiente);
			//Se comienza a almacenar un nuevo coeficiente
			i = 0;
		}
		//COMPARADOR (mayor/menor, el signo igual está implícito)
		if (((* ptr) == '>') || ((* ptr) == '<'))
			(res.comparador) = (* ptr);
		if (((* ptr) == '=') && (((* ptr - 1) != '<') || ((* ptr - 1) != '>')))
			(res.comparador) = (* ptr);
	}
	coeficiente [i] = '\0';
	//Se inicializan los apuntadores de la estructura de la restricción dependiendo el número de variables leídas
	(res.limite) = ((float) atof (coeficiente));
	(res.variables) = (char * ) malloc (sizeof (char) * numero_variables);
	(res.coeficientes) = (float * ) malloc (sizeof (float) * numero_variables);
	//Se guarda el valor de los coeficientes y la variable correspondiente en la estructura de la restricción
	for (i = 0; i < numero_variables; i ++)
	{
		(res.variables [i]) = variables [i];
		(res.coeficientes [i]) = coeficientes [i];
	}
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
	printf ("\n\nFUNCIÓN OBJETIVO:\t");
	for (i = 0; i < strlen (objetivo.variables); i ++)
	{
		if (i == 0)
			printf ("%f%c", objetivo.coeficientes [i], objetivo.variables [i]);
		else
		{
			if ((objetivo.coeficientes [i]) >= 0)
				printf ("+%f%c", objetivo.coeficientes [i], objetivo.variables [i]);
			else
				printf ("%f%c", objetivo.coeficientes [i], objetivo.variables [i]);
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
					printf ("%f%c", r.coeficientes [j], r.variables [j]);
				else
				{
					if ((r.coeficientes [j]) >= 0)
						printf ("+%f%c", r.coeficientes [j], r.variables [j]);
					else
						printf ("%f%c", r.coeficientes [j], r.variables [j]);
				}
			}
			printf (" %c= %f", r.comparador, r.limite);
		}
		printf ("\n\n");
	}else
		printf ("\n\nNo existen restricciones\n\n");
	return;
}
/*Obtiene las restricciones de una variable*/
lista obtener_restricciones_dependientes(lista *restricciones,char var){
	lista restric;
	restriccion r;
	restriccion aux;
	int i,j;
	int tam=0;
	Initialize(&restric);
	for (i = 0; i < Size(restricciones) ; i++)
	{
		r = Element(restricciones, i+1);
		for (j = 0; j < strlen(r.variables);j++)
		{
			/*if(r.comparador == '>'){
				aux = invertir_restriccion(r);
				Add(&restric,aux);
			}*/
			if(r.variables[j] == var){
				Add(&restric,r);
			}
		}
	}
	//printR(&restric);
	return restric;
}

//Invierte una restriccion
restriccion invertir_restriccion(restriccion r){
	restriccion aux;
	int i;
	aux.limite = (r.limite*(-1));
	aux.comparador = '<';
	for (i = 0; i < strlen(r.variables); i++)
	{
		aux.coeficientes[i] = (r.coeficientes[i]*(-1));
		aux.variables[i] = r.variables[i];
	}
	return aux;
}

//Funcion de prueba 
void printR(lista* restricciones){
	int i,j;
	restriccion r;
	for ( i = 0; i < Size(restricciones);i++)
	{
		r = Element(restricciones, i+1);
		for (j = 0; j < strlen(r.variables);j++)
		{
			printf("%f%c\n",r.coeficientes[j],r.variables[j] );
		}
		
	}

}

/*Funcion comparadora general, para usar en qsort*/
int comp(const void * a, const void * b){
	if(*(float*)a < *(float*)b) return -1;
	if(*(float*)a == *(float*)b) return 0;
	if(*(float*)a > *(float*)b) return 1;
}


void shell_sort(float *A, int n){
	int gap = n/2;  //Se obtiene el gap dividiendo el tamaño de arreglo entre dos
  	int inner, outer, swap; //Variables auxiliares

  	while (gap > 0) { //Mientras gap sea mayor que zero entonces:
	    for(outer = gap; outer < n; outer++){ // Para outer igual a gap, siempre que outer sea menor a n, outer aumentara su valor en uno
	      	inner = outer; // inner se iguala al valor de outer
	      	swap = A[inner]; // Swap se iguala a la posiscion inner de A
	      	while (inner > gap - 1 && A[inner - gap] > swap ) {  // Mientras inner sea mayor que gap menos 1 y que A en su posicion inner menos gap sea mayor a Swap
	        	A[inner] = A[inner - gap]; //La posicion inner de A tomara como nuevo valor la posicion inner menos  gap de A
	        	inner -= gap; //inner decrementa su valor en gap veces
	      	}
	      	A[inner] = swap; //La posicion inner de A tomo como nuevo valor swap
    	}
    	gap /=2; // se divide a gap entre dos
  	}
}

/*Obtiene los valores de los limites de una variable*/
Limites obtenerValoresLimites(lista *l,char var){
	Limites lim;
	restriccion r;
	int i,j,tam=0;
	float *aux = malloc(Size(l) *sizeof * aux);
	for (i = 0; i < Size(l); i++)
	{
		r = Element(l,i+1);
		for (j = 0; j < strlen(r.variables); j++)
		{
			if(r.variables[j] == var){
				aux[tam++] = (r.limite/r.coeficientes[j]);
				}
		}
	}
	//qsort(aux,sizeof(aux)/sizeof(*aux)+1,sizeof(float),comp);
	shell_sort(aux,tam);
	lim.inferior = 0;
	lim.superior = aux[tam-1];
	lim.variable = var;

	return lim;
}

Limites* obtener_limites_variables(lista *restricciones, Z fo){
	
	lista res;
	int i,j,s=0;
	restriccion r;
	Limites * aux = (Limites*)malloc(sizeof(Limites)*strlen(fo.variables));
	Initialize(&res);
	for (i = 0; i < Size(restricciones); i++)
	{
		r = Element(restricciones,i+1);
		for (j = 0; j <strlen(fo.variables);j++)
		{
			res = obtener_restricciones_dependientes(restricciones,fo.variables[j]);
			aux[s++] = obtenerValoresLimites(&res,r.variables[j]);
		}
	}

	printf("Limite de las variables:\n\n");
	for (i = 0; i < s/4;i++)
	{
		printf("Varible: %c\n",aux[i].variable);
		printf("Limite superior: %f\n",aux[i].superior);
		printf("Limite inferior: %f\n",aux[i].inferior);
		printf("\n");
	}
	return aux;
}
