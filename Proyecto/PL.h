#include "Lista.h"

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
void imprimir_problema_inicial (Z objetivo, lista * restricciones);
