#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
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

Limites * obtener_limites_variables (lista * restricciones)
{
	lista res;
	int i, j;
	restriccion r;
<<<<<<< HEAD
	restriccion aux;
	int i,j;
	int tam=0;
	Initialize(&restric);
	for (i = 0; i < Size(restricciones) ; i++)
=======
	Limites * aux = (Limites*) malloc (sizeof (Limites));
	Initialize (&res);
	for (i = 0; i < Size (restricciones); i ++)
>>>>>>> d5cfe0b71bbdb14fbcd986b177eaad944b7ddbf5
	{
		r = Element (restricciones, i + 1);
		for (j = 0; j < strlen (r.variables); j++)
		{
<<<<<<< HEAD
			/*if(r.comparador == '>'){
				aux = invertir_restriccion(r);
				Add(&restric,aux);
			}*/
			if(r.variables[j] == var){
				Add(&restric,r);
			}
=======
			res = obtener_restricciones_dependientes (restricciones, r.variables [j]);
			aux [i] = obtener_valores_limites (&res, r.variables [j]);
>>>>>>> d5cfe0b71bbdb14fbcd986b177eaad944b7ddbf5
		}
	}
	printf ("Limite de las variables:\n\n");
	for (i = 0; i < Size (restricciones); i ++)
	{
		printf("Varible: %c\n", ((aux [i]).variable));
		printf("Limite superior: %f\n", ((aux [i]).superior));
		printf("Limite inferior: %f\n", ((aux [i]).inferior));
		printf("\n");
	}
	return aux;
}

<<<<<<< HEAD
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
=======
lista obtener_restricciones_dependientes (lista * restricciones, char variable)
{
	lista variables;
>>>>>>> d5cfe0b71bbdb14fbcd986b177eaad944b7ddbf5
	restriccion r;
	int i, j;
	Initialize (&variables);
	for (i = 0; i < Size (restricciones); i ++)
	{
		r = Element (restricciones, i + 1);
		for (j = 0; j < strlen (r.variables); j ++)
		{
			if (r.variables [j] == variable)
				Add (&variables, r);
		}
	}
	//printR(&variables);
	return variables;
}

//Funcion de prueba 
void printR (lista * restricciones)
{
	int i, j;
	restriccion r;
	for (i = 0; i < Size (restricciones); i ++)
	{
		r = Element(restricciones, i + 1);
		for (j = 0; j < strlen (r.variables); j ++)
			printf ("%f%c\n", r.coeficientes [j], r.variables [j]);
	}
}

/*Funcion comparadora general, para usar en qsort*/
int comp (const void * a, const void * b)
{
	if ((*(float*) a) < (*(float*) b))
		return -1;
	else if ((*(float*) a) == (*(float*) b))
		return 0;
	else if ((*(float*) a) > (*(float*) b))
		return 1;
}


void shell_sort (float * numeros, int n)
{
	int i, j, k = n / 2;
	float temp;
	while (k > 0)
	{
		for (i = k; i < n; i ++)
		{
			temp = numeros [i];
			j = i - k;
			while (j >= 0 && numeros [j] > temp)
			{
				numeros [j + k] = numeros [j];
				j -= k;
			}
			numeros [j + k] = temp;
		}
		k /= 2;
	}
	return;
}

/*Obtiene los valores de los limites de una variable*/
Limites obtener_valores_limites (lista *l, char var)
{
	Limites lim;
	restriccion r;
<<<<<<< HEAD
	int i,j,tam=0;
	float *aux = malloc(Size(l) *sizeof * aux);
	for (i = 0; i < Size(l); i++)
=======
	int i, j, tam = 0;
	float *aux = (float*) malloc (sizeof (float));
	for (i = 0; i < Size (l); i ++)
>>>>>>> d5cfe0b71bbdb14fbcd986b177eaad944b7ddbf5
	{
		r = Element (l, i + 1);
		for (j = 0; j < strlen (r.variables); j ++)
		{
<<<<<<< HEAD
			if(r.variables[j] == var){
				aux[tam++] = (r.limite/r.coeficientes[j]);
				}
=======
			if (r.variables [j] == var)
				aux [tam ++] = (r.limite / (r.coeficientes [j]));
>>>>>>> d5cfe0b71bbdb14fbcd986b177eaad944b7ddbf5
		}
	}
	//qsort(aux,sizeof(aux)/sizeof(*aux)+1,sizeof(float),comp);
<<<<<<< HEAD
	shell_sort(aux,tam);
	lim.inferior = 0;
	lim.superior = aux[tam-1];
=======
	shell_sort (aux, tam);

	//printf("\n");

	//for (i = 0; i < sizeof(aux)/sizeof(*aux) ;i++)
	//{
	//	printf("%f\n",aux[i]);
	//}

	lim.inferior = (aux [0]);
	lim.superior = (aux [tam - 1]);
>>>>>>> d5cfe0b71bbdb14fbcd986b177eaad944b7ddbf5
	lim.variable = var;

	return lim;
}

<<<<<<< HEAD
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
=======
char * cruzar_vectores (char * vector1, char * vector2)
{
	int i, cromosomas_vector1;
	int tam = strlen (vector1);
	char * vector_resultado = (char *) malloc (sizeof (char) * tam);
	srand (time (NULL));
	//Se obtiene aleatoriamente el número de bits del vector 1
	cromosomas_vector1 = (rand () % tam);
	//Se copian los n bits del vector 1 al resultado
	for (i = 0; i < cromosomas_vector1; i ++)
		vector_resultado [i] = vector1 [i];
	//Se copian los bits restantes del vector 2 al resultado
	for (; i < tam; i ++)
		vector_resultado [i] = vector2 [i];
	vector_resultado [i] = '\0';
	return vector_resultado;
}

char * mutar_vector (char * vector)
{
	//Se obtiene el número de cromosomas del vector
	int tam = strlen (vector);
	int bit;
	srand (time (NULL));
	//Se genera aleatoriamente el bit en el que se realizará la mutación
	bit = (rand () % tam);
	//Se cambia el cromosoma por 0 o 1 según corresponda
	if (vector [bit] == '0')
		vector [bit] = '1';
	else if (vector [bit] == '1')
		vector [bit] = '0';
	else
		printf ("\n\nError al mutar el vector '%s'\n\n", vector);
	return vector;
}

integrante * obtener_primera_poblacion (Limites variables, Condiciones_AG condiciones)
{
	int i, num_bits;
	integrante * poblacion = (integrante *) malloc (sizeof (integrante));
	for (i = 0; i < )
>>>>>>> d5cfe0b71bbdb14fbcd986b177eaad944b7ddbf5
}
