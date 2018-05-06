#include <sys/resource.h>
#include <sys/time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>
#include "PL.h"

//FUNCIONES PARA OBTENER LOS VALORES PARA ARRANCAR EL ALGORITMO

//Regresa las condiciones iniciales del problema y las condiciones de paro del mismo
Condiciones_AG obtener_condiciones_iniciales ()
{
	Condiciones_AG auxiliar;												//Estructura para almacenar las condiciones del problema
	system (clear);
	printf ("\n______________________________ALGORITMOS GENÉTICOS______________________________\n");
	printf ("\n\nIngresa el tiempo máximo de procesamiento (minutos):\t");
	scanf ("%d", &auxiliar.tiempo_maximo);
	printf ("\n\nIngresa el error máximo permitido (porcentaje):\t\t");
	scanf ("%f", &auxiliar.error_maximo);
	auxiliar.error_maximo = (auxiliar.error_maximo * 0.01);
	printf ("\n\nIngresa el número máximo de iteraciones:\t\t");
	scanf ("%d", &auxiliar.it_max);
	printf ("\n\nIngresa el número de bits de precisión deseados:\t");
	scanf ("%hd", &auxiliar.bits_precision);
	printf ("\n\nIngresa el número de integrantes por población:\t\t");
	scanf ("%d", &auxiliar.integrantes);
	return auxiliar;
}

//Retorna el criterio del problema (maximizar/minimizar)
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

//Retorna la función objetivo
Z obtener_funcion_objetivo ()
{
	Z funcion_objetivo;
	//Obtener el criterio de optimización
	funcion_objetivo.criterio = criterio_funcion_objetivo ();
	//Obtener los coeficientes y variables de la F.O según el criterio
	funcion_objetivo = cuerpo_funcion_objetivo (funcion_objetivo);
	return funcion_objetivo;
}

//Obtiene por separado las variables y coeficientes de la función objetivo
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

//Retorna la lista de restricciones del problema
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
		if (res.comparador == '>')
			res = invertir_restriccion (res);
		//Se almacena esa restricción en una lista de restricciones
		Add (&restricciones, res);
		printf ("\n¿Deseas ingresar otra restricción?\t\t1.Si\t\t0.No\t\t");
		scanf ("%hhd", &otra);
		i ++;
	}
	return restricciones;
}

//Obtiene por separado las variables, coeficientes, criterio y límite de cada restricción
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



//FUNCIONES PARA OBTENER LOS LÍMITES DE CADA UNA DE LAS VARIABLES A PARTIR DE LAS RESTRICCIONES

//Obtiene los límites de cada variable de la función objetivo
Limites * obtener_limites_variables (lista * restricciones, Z funcion_objetivo)
{
	lista res;
	int i, j,s = 0;
	restriccion r;
	Limites * aux = (Limites*) malloc (sizeof (Limites));
	Initialize (&res);
	for (i = 0; i < strlen (funcion_objetivo.variables); i ++)
	{
		res = obtener_restricciones_dependientes (restricciones, funcion_objetivo.variables [i]);
		aux [s++] = obtener_valores_limites (&res, funcion_objetivo.variables [i]);
	}
	return aux;
}

//Invierte una restricción de tipo ">"
restriccion invertir_restriccion (restriccion r)
{
	restriccion aux;
	int i;
	aux.coeficientes = (float *) malloc (sizeof (float) * (strlen (r.variables)));
	aux.variables = (char *) malloc (sizeof (char) * (strlen (r.variables)));
	aux.limite = (r.limite * (-1));
	aux.comparador = '<';
	for (i = 0; i < strlen (r.variables); i ++)
	{
		(aux.coeficientes [i]) = ((r.coeficientes [i]) * (-1));
		(aux.variables [i]) = (r.variables [i]);
	}
	(aux.variables [i]) = '\0';
	return aux;
}

//Regresa la lista de restricciones que involucran a la variable 'variable'
lista obtener_restricciones_dependientes (lista * restricciones, char variable)
{
	lista variables;
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
	return variables;
}

//Algortimo de ordenamiento shell
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

//Obtiene los límites inferior y superior de cada variable a partir de la lista
Limites obtener_valores_limites (lista * restricciones, char variable)
{
	Limites lim;
	restriccion r;
	int i, j, tam = 0;
	float *aux = (float*) malloc (sizeof (float));
	for (i = 0; i < Size (restricciones); i ++)
	{
		r = Element (restricciones, i + 1);
		for (j = 0; j < strlen (r.variables); j ++)
		{
			if (r.variables [j] == variable)
				aux [tam ++] = (r.limite / (r.coeficientes [j]));
		}
	}
	shell_sort (aux, tam);
	lim.inferior = 0;
	lim.superior = (aux [tam - 1]);
	lim.variable = variable;
	free (aux);
	return lim;
}



//FUNCIONES PARA OBTENER LA PRIMERA POBLACIÓN DEL PROBLEMA

//Retorna la matriz población con valor binario, decimal y genotipo
integrante ** obtener_primera_poblacion (Z funcion_objetivo, Limites * variables, Condiciones_AG geneticos, lista * restricciones)
{
	int i, j, k, num_bits, columnas, filas, superior, inferior, aux;
	filas = (geneticos.integrantes);
	columnas = strlen (funcion_objetivo.variables);
	float * valores = (float *) malloc (sizeof (float) * columnas);
	//Población de integrantes (filas) para cada variable (columnas)
	integrante ** poblacion = matriz_poblacion (filas, columnas);
	//Se calcula el número de bits de cada integrante de la población
	srand (time (NULL));
	for (i = 0; i < filas; i ++)
	{
		for (j = 0; j < columnas; j ++)
		{
			superior = round ((variables [j]).superior);
			inferior = round ((variables [j]).inferior);
			aux = ((superior - inferior) * (pow (10, geneticos.bits_precision)));
			num_bits = ceil ((log10 (aux)) / (log10 (2)));
			((poblacion [i][j]).binario) = (char *) malloc (sizeof (char) * num_bits);
			for (k = 0; k < num_bits; k ++)
			{
				if ((rand () % 2))
					((poblacion [i][j]).binario [k]) = '1';
				else
					((poblacion [i][j]).binario [k]) = '0';
			}
			((poblacion [i][j]).binario [k]) = '\0';
			((poblacion [i][j]).decimal) = binario_to_decimal ((poblacion [i][j]).binario);
			((poblacion [i][j]).momentum) = obtener_genotipo (poblacion [i][j].decimal, variables [j], num_bits);
		}
		for (j = 0; j < columnas; j ++)
			valores [j] = ((poblacion [i][j]).momentum);
		if (!evaluar_restricciones (valores, restricciones, funcion_objetivo))
			i --;
	}
	return poblacion;
}

//Retorna una matriz población vacía de n filas por m columnas
integrante ** matriz_poblacion (int filas, int columnas)
{
	int i;
	integrante ** matriz = (integrante **) malloc (sizeof (integrante *) * filas);
	for (i = 0; i < filas; i ++)
		matriz [i] = (integrante *) malloc (sizeof (integrante) * columnas);
	return matriz;
}

//Retorna el genotipo de una variable de un vector
float obtener_genotipo (int decimal, Limites variable, int num_bits)
{
	int inferior, superior;
	superior = round (variable.superior);
	inferior = round (variable.inferior);
	return (inferior + (decimal * ((superior - inferior) / (pow (2, num_bits) - 1))));
}

//Convierte una cadena de 0's y 1's (número binario) en un número decimal
int binario_to_decimal (char * binario)
{
	int i, numero = 0, flag = 0;
	char * pt = binario;
	char * ptr = binario;
	for (; * pt != '\0'; pt ++); pt--;
	for (; pt >= ptr; pt--, flag ++)
		if (* pt == '1')
			numero += pow (2, flag);
	return numero;
}



//FUNCIONES PARA EVALUAR FUNCIONES (FUNCIÓN OBJETIVO Y RESTRICCIONES)

//Evalua al integrante 'integrante' de la población en la función objetivo
float evaluar_funcion_objetivo (integrante ** poblacion, Z funcion_objetivo, int integrante)
{
	int j, columnas;
	float resultado = 0;
	columnas = strlen (funcion_objetivo.variables);
	for (j = 0; j < columnas; j ++)
		resultado += (funcion_objetivo.coeficientes [j] * ((poblacion [integrante][j]).momentum));
	return resultado;
}

//Evalúa un vector completo de la población en todas las restricciones para verificar que todas se cumplan
boolean evaluar_restricciones (float * valores, lista * restricciones, Z funcion_objetivo)
{
	int i, j, columnas, numero_restricciones;
	restriccion r;
	float resultado;
	columnas = strlen (funcion_objetivo.variables);
	numero_restricciones = Size (restricciones);
	for (i = 0; i < numero_restricciones; i ++)
	{
		resultado = 0;
		r = Element (restricciones, i + 1);
		for (j = 0; j < columnas; j ++)
			resultado += (r.coeficientes [j] * (valores [j]));
		if (resultado > r.limite)
			return FALSE;
	}
	return TRUE;
}



//FUNCIONES PARA RESOLVER EL PROBLEMA, COMIENZA EL ALGORITMO

//Función maestra para llamar al algoritmo n veces dependiendo de las condiciones iniciales (tiempo, error e iteraciones)
void solve (integrante ** poblacion, Z funcion_objetivo, Condiciones_AG geneticos, Limites * variables, lista * restricciones)
{
	int i, j;
	char * fuerte1 = (char *) malloc (sizeof (char));
	double tiempo_inicial, tiempo_final;
	tomar_tiempo (&tiempo_inicial);
	for (i = 0; i < geneticos.it_max; i ++)
	{
		FireFly (poblacion, funcion_objetivo, geneticos, (i + 1));
		poblacion = generar_nueva_poblacion (poblacion, funcion_objetivo, geneticos, variables, restricciones);
		tomar_tiempo (&tiempo_final);
		if ((tiempo_final - tiempo_inicial) >= (geneticos.tiempo_maximo * 60))
		{
			printf ("\n\n\n\n\n\tCRITERIO DE FINALIZACIÓN: TIEMPO MÁXIMO ALCANZADO\n\n\n\n");
			break;
		}
	}
	printf ("\n\n\n\n\n\tCRITERIO DE FINALIZACIÓN: ITERACIONES MÁXIMAS ALCANZADAS\n\n\n\n");
}

//Algoritmo genético utilizado con una población P, una función objetivo Z y unas condiciones iniciales G
void FireFly (integrante ** poblacion, Z funcion_objetivo, Condiciones_AG geneticos, int iteracion)
{
	int i, j;
	float prob, sumaZ, pAcum, auxiliar;
	int * fuertes = (int *) malloc (sizeof (int) * (geneticos.integrantes));
	float * valor_funcion_objetivo = malloc (sizeof (float) * (geneticos.integrantes));
	float * aleat = (float *) malloc (sizeof (float) * (geneticos.integrantes));
	float * pAcumE = (float *) malloc (sizeof (float) * (geneticos.integrantes));
	srand (time (NULL));
	for (i = 0, sumaZ = 0, pAcum = 0; i < geneticos.integrantes; i ++)
	{
		valor_funcion_objetivo [i] = evaluar_funcion_objetivo (poblacion, funcion_objetivo, i);
		sumaZ += valor_funcion_objetivo [i];
	}
	imprimir_valores (poblacion, funcion_objetivo, geneticos, valor_funcion_objetivo, iteracion);
	for (i = 0; i < geneticos.integrantes; i ++)
	{
		aleat [i] = ((rand() % 10000)/10000.0);
		prob = (valor_funcion_objetivo [i] / sumaZ);
		pAcum += prob;
		pAcumE [i] = pAcum;
	}
	i = 0; j = 0;
	while (i < geneticos.integrantes)
	{
		auxiliar = (pAcumE [j ++] - aleat [i]);
		if (auxiliar >= 0)
		{
			fuertes [i ++] = (j - 1);
			j = 0;
		}
	}
	for (i = 0; i < geneticos.integrantes; i ++)
		(poblacion [i][0]).apariciones = 0;
	for (i = 0; i < geneticos.integrantes; i ++)
	{
		j = fuertes [i];
		(poblacion [j][0]).apariciones ++;
	}
}

//Retorna una nueva población con nuevos vectores mejorados (cruza o mutaciones)
integrante ** generar_nueva_poblacion (integrante ** poblacion, Z funcion_objetivo, Condiciones_AG geneticos, Limites *variables, lista *restricciones)
{
	int i, j, k, columnas, filas, num_bits, maximo1 = 0, maximo2 = 0;
	int fuerte1 = -1;										//Posición del vector más fuerte
	int fuerte2 = -1;										//Posición del 2do vector más fuerte (no necesariamente hay uno)
	filas = (geneticos.integrantes);
	columnas = strlen (funcion_objetivo.variables);
	float * valores = (float *) malloc (sizeof (float) * columnas);
	for (i = 0; i < filas; i ++)
	{
		if ((poblacion [i][0]).apariciones > 0)
		{
			if ((poblacion [i][0]).apariciones >= maximo1)
			{
				fuerte2 = fuerte1;
				fuerte1 = i;
				maximo1 = (poblacion [i][0]).apariciones;
			}else
			{
				if ((poblacion [i][0]).apariciones >= maximo2)
				{
					fuerte2 = i;
					maximo2 = (poblacion [i][0]).apariciones;
				}
			}
		}
	}
	for (i = 0; i < filas; i ++)
	{
		if ((i == fuerte1) || (i == fuerte2))
			continue;
		for (j = 0; j < columnas; j ++)
		{
			num_bits = strlen (poblacion [0][j].binario);
			char * nuevo_vector = (char *) malloc (sizeof (char) * num_bits);
			if (fuerte2 < 0)
				nuevo_vector = mutar_vector ((poblacion [fuerte1][j]).binario);
			else
				nuevo_vector = cruzar_vectores ((poblacion [fuerte1][j]).binario, (poblacion [fuerte2][j]).binario);
			for (k = 0; k < num_bits; k ++)
				((poblacion [i][j]).binario [k]) = nuevo_vector [k];
			((poblacion [i][j]).binario [k]) = '\0';
			((poblacion [i][j]).decimal) = binario_to_decimal ((poblacion [i][j]).binario);
			((poblacion [i][j]).momentum) = obtener_genotipo (poblacion [i][j].decimal, variables [j], num_bits);
		}
		for (j = 0; j < columnas; j ++)
			valores [j] = ((poblacion [i][j]).momentum);
		if (!evaluar_restricciones (valores, restricciones, funcion_objetivo))
			i --;
	}
	return poblacion;
}

//Retorna la cruza de 2 vectores
char * cruzar_vectores (char * vector1, char * vector2)
{
	int i, cromosomas_vector1;
	int tam = strlen (vector1);
	srand (time (NULL));
	cromosomas_vector1 = (rand () % (tam - 1)) + 1;
	srand (time (NULL));
	i = (rand () % 2);
	if (i)
	{
		for (i = cromosomas_vector1; i < tam; i ++)
			vector1 [i] = vector2 [i];
		vector1 [i] = '\0';
		return vector1;
	}else
	{
		for (i = cromosomas_vector1; i < tam; i ++)
			vector2 [i] = vector1 [i];
		vector2 [i] = '\0';
		return vector2;
	}
}

//Retorna un vector mutado en un bit
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

//Toma la hora actual en walltime
void tomar_tiempo (double * walltime)
{
	double mega = 1.0e-6;
	struct rusage buffer;
	struct timeval tp;
	struct timezone tzp;
	getrusage(RUSAGE_SELF, &buffer);
	gettimeofday(&tp, &tzp);
	*walltime = (double) tp.tv_sec + 1.0e-6 * tp.tv_usec; 
}

void imprimir_valores (integrante ** poblacion, Z funcion_objetivo, Condiciones_AG geneticos, float * valor_funcion_objetivo, int iteracion)
{
	FILE * archivo;
	int i, j;
	int filas = (geneticos.integrantes);
	int columnas = strlen (funcion_objetivo.variables);
	archivo = fopen ("Valores.txt", "a");
	if (archivo == NULL)
	{
		printf ("\n\nError al escribir los valores en el archivo.\n\n");
		exit (0);
	}
	for (i = 0; i < 70; i ++)
		fprintf (archivo, "_");
	fprintf (archivo, "ITERACIÓN %d", iteracion);
	for (i = 0; i < 70; i ++)
		fprintf (archivo, "_");
	fprintf (archivo, "\n\n");
	for (i = 0; i < filas; i ++)
	{
		for (j = 0; j < columnas; j ++)
			fprintf (archivo, "Valor de %c%d:\t%f\n", (funcion_objetivo.variables [j]), j, (poblacion [i][j].momentum));
		fprintf (archivo, "Valor de F.O:\t%f\n\n", (valor_funcion_objetivo [i]));
	}
	fprintf (archivo, "\n\n");
	fclose (archivo);
}
