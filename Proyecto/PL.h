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
	char tiempo_maximo;					//Almacena el tiempo máximo de resolución (minutos)
	float error_maximo;					//Almacena el error máximo permitido (porcentaje)
	int it_max;							//Almacena el número de iteraciones máximas del problema
	char bits_precision;				//Almacena el número de bits de precisión
	char integrantes;					//Almacena el número de integrantes de la población
}Condiciones_AG;

//Estructura para almacenar la Función Objetivo
typedef struct Z
{
	float * coeficientes;				//Almacena los coeficientes de la F.O
	char * variables;					//Almacena las variables de la F.O
	short criterio;						//'1' para maximizar y '-1' para minimizar
}Z;

Z obtener_funcion_objetivo ();
int criterio_funcion_objetivo ();
Z cuerpo_funcion_objetivo (Z objetivo);
lista obtener_restricciones ();
restriccion cuerpo_restriccion (char numero_restriccion);
Condiciones_AG obtener_condiciones_iniciales ();
void imprimir_problema_inicial (Z objetivo, lista * restricciones);
