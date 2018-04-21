#define TRUE	1
#define FALSE	0

//Estructura elemento (Modificable por el usuario)
typedef struct elemento
{
	float * coeficientes;				//Almacena los coeficientes de la restricción
	char * variables;					//Almacena las variables de la restricción
	char comparador;					//Almacena el operador '>', '<' o '='
	float limite;						//Almacena el límite de la restricción
}restriccion;

//Tipo de dato boolean (Modelado con un char)
typedef unsigned char boolean;

//Estructura de un NODO DOBLEMENTE LIGADO(No modificar)
typedef struct nodo
{
	restriccion r;
	struct nodo *adelante;	
	struct nodo *atras;
}nodo;

//Estructura lista
typedef struct lista
{
	nodo *frente;
	nodo *final;
	int tam;
}lista;

//Definición de una posición
typedef nodo* posicion;

//Operaciones de construcción
void Initialize(lista *l);
void Destroy(lista *l);

//Operaciones de posicionamiento y busqueda
posicion Final(lista *l);
posicion First(lista *l);
posicion Following(lista *l, posicion p);
posicion Previus(lista *l, posicion p);
posicion Search(lista *l, restriccion e);

//Operaciones de consulta
restriccion Position(lista *l, posicion p);
boolean ValidatePosition(lista *l, posicion p);
restriccion Element(lista *l, int n);
posicion ElementPosition(lista *l, int n);
int Size(lista *l);
boolean Empty(lista *l);

//Operaciones de modificación
void Insert(lista *l, restriccion e, posicion p,boolean b);
void Add(lista *l,restriccion e);
void Remove(lista *l,posicion p);
void Replace(lista *l,posicion p, restriccion e);
