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
	scanf ("%hd", &auxiliar.tiempo_maximo);
	printf ("\n\nIngresa el error máximo permitido (porcentaje):\t\t");
	scanf ("%f", &auxiliar.error_maximo);
	auxiliar.error_maximo = (auxiliar.error_maximo * 0.01);
	printf ("\n\nIngresa el número máximo de iteraciones:\t\t");
	scanf ("%hd", &auxiliar.it_max);
	printf ("\n\nIngresa el número de bits de precisión deseados:\t");
	scanf ("%hd", &auxiliar.bits_precision);
	printf ("\n\nIngresa el número de integrantes por población:\t\t");
	scanf ("%hd", &auxiliar.integrantes);
	return auxiliar;
}

void imprimir_condiciones_iniciales (Condiciones_AG condiciones)
{
	printf ("\n\nTiempo máximo: %d", condiciones.tiempo_maximo);
	printf ("\nPorcentaje de error: %f", condiciones.error_maximo);
	printf ("\nNúmero máximo de iteraciones: %d", condiciones.it_max);
	printf ("\nNúmero de bits de precisión: %d", condiciones.bits_precision);
	printf ("\nNúmero de integrantes por población: %d", condiciones.integrantes);
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
/*Funcion: obtener_limites_variables
Recibe: Lista de restricciones, funcion obejetivo
Retorna: Array con los limites de cada variable*/
Limites * obtener_limites_variables (lista * restricciones, Z FO)
{
	lista res;
	int i, j,s=0;
	restriccion r;
	Limites * aux = (Limites*) malloc (sizeof (Limites));
	Initialize (&res);
	for (i = 0; i < strlen(FO.variables); i ++)
	{
		res = obtener_restricciones_dependientes(restricciones,FO.variables[i]);
		aux[s++]= obtener_valores_limites(&res,FO.variables[i]);
	}
	printf ("\n\nLimite de las variables:\n\n");
	for (i = 0; i < s; i ++)
	{
		printf("Varible: %c\n", ((aux [i]).variable));
		printf("Limite superior: %f\n", ((aux [i]).superior));
		printf("Limite inferior: %f\n", ((aux [i]).inferior));
		printf("\n");
	}
	return aux;
}

/*Funcion:invertir_restriccion
Recibe: restriccion r
Retorna: restriccion r invertida*/
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

/*Funcion:obtener_restricciones_dependientes
Recibe: lista de restricciones, variable 
Retorna: lista con las restricciones donde aparece variable*/
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

/*Funcion: obtener_valores_limites
Recibe: lista de restricciones dependientes l, variable 
Retorna: Los limites superior e inferior de la variable 'var'*/
Limites obtener_valores_limites (lista *l, char var)
{
	Limites lim;
	restriccion r;
	int i, j, tam = 0;
	float *aux = (float*) malloc (sizeof (float));
	for (i = 0; i < Size (l); i ++)
	{
		r = Element (l, i + 1);
		for (j = 0; j < strlen (r.variables); j ++)
		{
			if (r.variables [j] == var)
				aux [tam ++] = (r.limite / (r.coeficientes [j]));
		}
	}
	shell_sort (aux, tam);
	lim.inferior = 0;
	lim.superior = (aux [tam - 1]);
	lim.variable = var;
	free (aux);
	return lim;
}

char * cruzar_vectores (char * vector1, char * vector2)
{
	int i, cromosomas_vector1;
	int tam = strlen (vector1);
	srand (time (NULL));
	//Se obtiene aleatoriamente el número de bits del vector 1
	cromosomas_vector1 = (rand () % tam);
	//Se copian los bits restantes del vector 2 al resultado
	for (i = cromosomas_vector1; i < tam; i ++)
		vector1 [i] = vector2 [i];
	vector1 [i] = '\0';
	return vector1;
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

integrante ** obtener_primera_poblacion (Z funcion_objetivo, Limites * variables, Condiciones_AG condiciones)
{
	int i, j, k, num_bits, columnas, filas, superior, inferior, aux;
	filas = (condiciones.integrantes);
	columnas = strlen (funcion_objetivo.variables);
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
			aux = ((superior - inferior) * (pow (10, condiciones.bits_precision)));
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
			/*while (((poblacion [i][j]).decimal) > superior)
			{
				for (k = 0; k < num_bits; k ++)
				{
					if ((rand () % 2))
						((poblacion [i][j]).binario [k]) = '1';
					else
						((poblacion [i][j]).binario [k]) = '0';
				}
				((poblacion [i][j]).binario [k]) = '\0';
				((poblacion [i][j]).decimal) = binario_to_decimal ((poblacion [i][j]).binario);
			}*/
			((poblacion [i][j]).momentum) = (inferior + (((poblacion [i][j]).decimal) * ((superior - inferior) / (pow (2, num_bits) - 1))));
		}
	}
	print_poblacion (poblacion, filas, columnas);
	return poblacion;
}

void print_poblacion (integrante ** poblacion, int filas, int columnas)
{
	int i, j, k;
	printf ("\n\nPOBLACIÓN BINARIO\n\n");
	for (i = 0; i < filas; i ++)
	{
		for (j = 0; j < columnas; j ++)
		{
			for (k = 0; k < strlen (poblacion [i][j].binario); k ++)
				printf ("%c", ((poblacion [i][j]).binario [k]));
			printf ("\t");
		}
		printf ("\n");
	}
	printf ("\n\nPOBLACIÓN DECIMAL\n\n");
	for (i = 0; i < filas; i ++)
	{
		for (j = 0; j < columnas; j ++)
			printf ("%d\t", ((poblacion [i][j]).decimal));
		printf ("\n");
	}
	printf ("\n\nPOBLACIÓN MOMENTUM\n\n");
	for (i = 0; i < filas; i ++)
	{
		for (j = 0; j < columnas; j ++)
			printf ("%f\t", ((poblacion [i][j]).momentum));
		printf ("\n");
	}
}

integrante ** matriz_poblacion (int filas, int columnas)
{
	int i;
	integrante ** matriz = (integrante **) malloc (sizeof (integrante *) * filas);
	for (i = 0; i < filas; i ++)
		matriz [i] = (integrante *) malloc (sizeof (integrante) * columnas);
	return matriz;
}

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
