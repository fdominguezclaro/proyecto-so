#include <stdlib.h>
#include <stdio.h>

#include "graph.h"
#include "structs.h"

Node* node_init(Dir_parser* dir_parser)
{
  Node* node = malloc(sizeof(Node));

  node -> childs = malloc(sizeof(Node*));
  node -> count = 0;
  node -> type = dir_parser -> type;
  node -> name = dir_parser -> name;
  node -> index = dir_parser -> index;

  return node;
}

/** Inicializa un arbol vacio que sera la estructura que representa los directorios */
Graph* graph_init(unsigned char* bytemap)
{
  Graph* graph = malloc(sizeof(Graph));

  graph -> count = 0;
  graph -> root = NULL;
  graph -> bytemap = bytemap;

  return graph;
}

/** Funcion que libera recursivamente la memoria de la lista ligada */
static void nodes_destroy(Node* node)
{
  if (node)
  {
    for (int i = 0; i < node -> count; i++) {
      nodes_destroy(node -> childs[i]);
    }
    free(node);
  }
}

/** Agrega un nodo (archivo o directorio) */
void graph_append(Graph* graph, Node* parent, Node* node)
{
  // printf("Name: %s\n", node -> name);
  if ((node -> index == 0) & !graph -> root)
  {
    graph -> root = node;
  } else
  {
    parent -> childs[parent -> count] = node;
    parent -> count++;
    parent -> childs = (Node**) realloc(parent -> childs, sizeof(Node*) * (parent -> count + 1));
  }

  // Sumo 1 al numero de nodos
  graph -> count++;
}

/** Funcion que destruye la lista ligada liberando la memoria utilizada */
void graph_destroy(Graph* graph)
{
  nodes_destroy(graph -> root);
  free(graph -> bytemap);
  free(graph);
}

/** imprime arbol de directorios. No implementada todavia*/
void print_graph(Graph* graph)
{

}
