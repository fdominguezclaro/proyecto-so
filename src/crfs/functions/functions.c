#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "../structs/graph.h"
#include "../structs/structs.h"

unsigned int BLOCK_SIZE = 2048;

extern char *DISK_PATH;

/** Convierte un Byte a Bits, retorna la cantidad de 1's
 * Printea el número binario: Necesario para cr_bitmap()
 */
int byteToBits(unsigned char byte)
{
	// Cuenta la cantidad de 1's
	int ones_counter = 0;

	int aux;
	int counter = 0;
	char *binary_number = malloc(sizeof(unsigned char) * 9);
	for (int bit = (sizeof(unsigned char) * 7); bit >= 0 ; bit--)
	{
		aux = byte >> bit;
		if (aux & 1) {
			binary_number[counter] = 1 + '0';
			ones_counter++;
		}
		else binary_number[counter] = 0 + '0';
		counter++;
	}
	binary_number[counter] = '\0';
	printf("%s", binary_number);
	free(binary_number);
	return ones_counter;
}

/** true si el bloque es valido en el bitmap
 * Aun no esta en uso, pero es mejor dejarla por si acaso
 */
bool valid(int block_index, unsigned char *bytemap)
{
	unsigned char byte = bytemap[block_index / 8];
	int position = 7 - block_index % 8;

	return (byte >> position) & 0x1;
}

/** Carga el bitmap en bytes */
static unsigned char *load_bitmap(void)
{
	FILE *file = fopen(DISK_PATH, "rb");
	fseek(file, BLOCK_SIZE, SEEK_SET);
	unsigned char *bytemap = malloc(sizeof(unsigned char) * BLOCK_SIZE * 4);
	fread(bytemap, sizeof(unsigned char), BLOCK_SIZE * 4, file);
	fclose(file);
	return bytemap;
}

/** Encargado de leer una entrada de un bloque y verificar el tipo de entrada */
static Dir_parser *read_entry(unsigned char *buffer, unsigned char *bytemap)
{
	unsigned char type = buffer[0];
	if (type == (unsigned char) 1) return NULL;

	char *aux_name = (char *) (buffer + 1);
	char *name = malloc(sizeof(char) * (strlen(aux_name) + 1));
	strcpy(name, aux_name);

	unsigned int index = (unsigned int)buffer[30] * 256 + (unsigned int)buffer[31];

	// printf("Reading Entry N° %u, %s, %i\n", type, name, index);
	// if (type == (unsigned char) 2) printf("DIR %s index: %u\n", name, index);
	// else if (type == (unsigned char) 4) printf("FILE %s index: %u\n", name, index);

	return dir_parser_init(type, name, index);
};

/** Lee un bloque de directorio en el indice index
 * Retorna un array de Dir_parser que representan los archivos en este directorio
 */
static Dir_parser **read_dir_block(unsigned int index, unsigned char *bytemap)
{
	// printf("\n---------------------\nREADING BLOCK %i\n", index);
	FILE *file = fopen(DISK_PATH, "rb");
	if (file == NULL) perror("Failed to open path");

	unsigned char *buffer = malloc(sizeof(unsigned char) * 32);

	Dir_parser **dir_parser = malloc(sizeof(Dir_parser *) * 64);

	// Lee un bloque completo
	for (int i = 0; i < 64; i++)
	{
		fseek(file, (32 * i) + ((unsigned int)((index) * BLOCK_SIZE)), SEEK_SET);
		fread(buffer, sizeof(unsigned char), 32, file);
		dir_parser[i] = read_entry(buffer, bytemap);
	};

	free(buffer);
	fclose(file);
	return dir_parser;
};

/** Lee los directorios de manera recursiva DFS */
static void load_dir(Graph *graph, Node *parent)
{
	Dir_parser **dir_entries;
	// Primera llamada crea al root
	if (!parent)
	{
		char *root_name = malloc(sizeof(char) * 5);
		strcpy(root_name, "root");
		dir_entries = read_dir_block(0, graph->bytemap);
		Dir_parser *root_entry = dir_parser_init((unsigned char) 2, root_name, 0);
		parent = node_init(root_entry, NULL);
		free(root_entry);
		// Agrego el nodo raiz
		graph_append(graph, NULL, parent);
	}
	// Llamadas restantes se llaman con un directorio padre
	else dir_entries = read_dir_block(parent->index, graph->bytemap);

	Dir_parser *entry;
	// Recorremos todas las entradas de ese directorio
	for (int i = 0; i < 64; i++)
	{
		// Entrada invalida
		entry = dir_entries[i];
		if (entry == NULL) continue;

		// Creo un nuevo nodo
		Node *node = node_init(dir_entries[i], parent -> path);
		free(dir_entries[i]);

		// Llamado recursivo solo si es directorio y no es el padre
		if (node->type == (unsigned char) 2) load_dir(graph, node);
		// Agregamos la entrada al directorio padre
		if (parent) graph_append(graph, parent, node);
	};
	free(dir_entries);
}

/** Arma el arbol de directorios y lo retorna */
Graph *load_disk(void)
{
	// Cargamos el bitmap
	unsigned char *bytemap = load_bitmap();
	// Creamos el arbol de directorios
	Graph *graph = graph_init(bytemap);
	// Cargamos los directorios al arbol
	load_dir(graph, NULL);
	return graph;
}

/** Recorta un string insertando un caracter de termino
 * Obtenido de: https://stackoverflow.com/questions/27414696/remove-last-four-characters-from-a-string-in-c
*/
void trim_end(char *str, int n)
{
	n = strlen(str) - n;

	if (n < 0)
		n = 0;

	str[n] = '\0';
}