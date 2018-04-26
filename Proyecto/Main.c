#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "PL.h"

/*
Compilar y ejecutar:
					1. gcc Lista.c -o Lista.o -c
					2. gcc PL.c -o PL.o -c
					3. gcc Main.c -o Main PL.o Lista.o
					4. ./Main
*/

int main(int argc, char const *argv[])
{

	Limites * lim;
	integrante * poblacion;
	char i;																		//Auxiliar utilizado como contador
	Z funcion_objetivo;															//Estructura para almacenar la F.O
	lista restricciones;														//Lista para almacenar las restricciones
	Condiciones_AG geneticos;													//EStructura para almacenar algunas condiciones del problema
	Initialize (&restricciones);												//Inicializamos la lista de restricciones
	system (clear);
	geneticos = obtener_condiciones_iniciales ();								//Se pide tiempo e iteraciones máximas, bits de precisión e integrantes
	funcion_objetivo = obtener_funcion_objetivo ();								//Obtener la función objetivo del problema
	restricciones = obtener_restricciones ();									//Obtener las restricciones del problema
	imprimir_problema_inicial (funcion_objetivo, &restricciones);
	lim = obtener_limites_variables(&restricciones);
	poblacion = obtener_primera_poblacion (lim, geneticos);
	return 0;
}
