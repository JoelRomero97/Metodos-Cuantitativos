#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "PL.h"

int main(int argc, char const *argv[])
{
	Limites * lim;
	integrante ** poblacion;
	char i;																		//Auxiliar utilizado como contador
	Z funcion_objetivo;															//Estructura para almacenar la F.O
	lista restricciones;														//Lista para almacenar las restricciones
	Condiciones_AG geneticos;													//EStructura para almacenar algunas condiciones del problema
	Initialize (&restricciones);												//Inicializamos la lista de restricciones
	system (clear);
	geneticos = obtener_condiciones_iniciales ();								//Se pide tiempo e iteraciones máximas, bits de precisión e integrantes
	funcion_objetivo = obtener_funcion_objetivo ();								//Obtener la función objetivo del problema
	restricciones = obtener_restricciones ();									//Obtener las restricciones del problema
	//imprimir_condiciones_iniciales (geneticos);
	//imprimir_problema_inicial (funcion_objetivo, &restricciones);
	lim = obtener_limites_variables(&restricciones, funcion_objetivo);
	poblacion = obtener_primera_poblacion (funcion_objetivo, lim, geneticos);
	for (i = 0; i < geneticos.integrantes; i ++)
	{
		float final = evaluar_funcion_objetivo (poblacion, funcion_objetivo, geneticos, i);
		printf ("\n\nEvaluación para integrante %d: %f", (i + 1), final);
	}
	printf ("\n\n");
	return 0;
}
