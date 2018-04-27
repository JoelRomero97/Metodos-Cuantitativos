#include "Lista.h"
#ifdef __linux__
#define clear "clear"
#endif
#ifdef __MINGW32__
#define clear "cls"
#endif
#define IT_MAX 100						//Número máximo de iteraciones
#define ERROR_MAXIMO 0.05				//Porcentaje máximo de error (5%)
#define TIEMPO_MAXIMO 5					//Tiempo máximo de procesamiento en minutos

//Estructura para almacenar algunas condiciones iniciales del problema
typedef struct Condiciones_AG
{
	short tiempo_maximo;				//Almacena el tiempo máximo de resolución (minutos)
	float error_maximo;					//Almacena el error máximo permitido (porcentaje)
	short it_max;						//Almacena el número de iteraciones máximas del problema
	short bits_precision;				//Almacena el número de bits de precisión
	short integrantes;					//Almacena el número de integrantes de la población
}Condiciones_AG;

//Estructura para almacenar la Función Objetivo
typedef struct Z
{
	float * coeficientes;				//Almacena los coeficientes de la F.O
	char * variables;					//Almacena las variables de la F.O
	short criterio;						//'1' para maximizar y '-1' para minimizar
}Z;

//Estructura para almacenar los limites de las variables
typedef struct limites
{
	float inferior;						//Almacena el limite inferior
	float superior;						//Almacena el limite superior
	char variable;						//Almacena la variable asociada a los limites
}Limites;

//Estructura para almacenar los valores binario y decimal de cada integrante de la población
typedef struct integrante
{
	char * binario;
	int decimal;
}integrante;

Condiciones_AG obtener_condiciones_iniciales ();
void imprimir_condiciones_iniciales (Condiciones_AG condiciones);
Z obtener_funcion_objetivo ();
int criterio_funcion_objetivo ();
Z cuerpo_funcion_objetivo (Z objetivo);
lista obtener_restricciones ();
restriccion cuerpo_restriccion (char numero_restriccion);
void imprimir_problema_inicial (Z objetivo, lista * restricciones);
Limites * obtener_limites_variables (lista * restricciones);
restriccion invertir_restriccion (restriccion r);
lista obtener_restricciones_dependientes (lista * restricciones, char variable);
void printR (lista * restricciones);
int comp (const void * a, const void * b);
void shell_sort (float * numeros, int n);
Limites obtener_valores_limites (lista *l, char var);
char * cruzar_vectores (char * vector1, char * vector2);
char * mutar_vector (char * vector);
integrante ** obtener_primera_poblacion (Z funcion_objetivo, Limites * variables, Condiciones_AG condiciones);
integrante ** matriz_poblacion (int filas, int columnas);
int binario_to_decimal (char * binario);
void print_poblacion (integrante ** poblacion, int filas, int columnas);
