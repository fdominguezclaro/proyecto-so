// Estructura obtenida del taller 0 de EDD
#pragma once

#include "structs.h"

/** Estructura de un nodo del grafo. Mantiene una referencia al nodo
siguiente y al nodo anterior ademas de mantener un numero */
typedef struct node
{
  // En este caso no podemos referirnos a la estructura como Node ya que aun
  // no esta completo el typedef

  /** Lista de punteros de hijos */
  struct node** childs;
  char* name;
  unsigned char type;
  unsigned int index;
  int count;
} Node;

/** Estructura de n grafo. Referencia a los extremos y mantiene un
contador de nodos en la lista */
typedef struct graph
{
  /** Nodo inicial del grafo */
  Node* root;
  /** Contador de elementos del grafo */
  int count;
  unsigned char* bytemap;
} Graph;


/** Constructor de n grafo. La crea vacia inicialmente */
Graph* graph_init(unsigned char* bytemap);

Node* node_init(Dir_parser* dir_parser);

/** Funcion que agrega un elemento al final del grafo */
void graph_append(Graph* graph, Node* parent, Node* node);

/** Funcion que obtiene el valor del grafo en la posicion dada */
int graph_get(Graph* graph, int position);

/** Funcion que destruyel grafo liberando la memoria utilizada */
void graph_destroy(Graph* graph);
