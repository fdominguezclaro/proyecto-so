#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "../structs/graph.h"
#include "../structs/structs.h"

unsigned int BLOCK_SIZE = 2048;

extern char *DISK_PATH;

static void printBits(unsigned char byte)
{
	for (int bit = (sizeof(unsigned char) * 8); bit > 0; bit--)
	{
		printf("%i ", byte & 0x01);
		byte = byte >> 1;
	}
}

/** true si el bloque es valido en el bitmap */
static bool valid(int block_index, unsigned char *bytemap)
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

	// bool is_valid = valid(0, bytemap);
	// Imprime el bitmap en grupos de a 1 byte
	// printf("%i\n", is_valid);
	// for (int i = 0; i < BLOCK_SIZE * 4; i++)
	// {
	// 	printf("indice: %i - ", i * 8);
	// 	printBits(bytemap[i]);
	// 	printf("\n");
	// }

	return bytemap;
}

Dir_parser *read_entry(unsigned char *buffer, unsigned char *bytemap)
{
	unsigned char type = buffer[0];
	char *name = (char *)(buffer + 1);
	unsigned int index = (unsigned int)buffer[30] * 256 + (unsigned int)buffer[31];

	if (type == (unsigned char)1) return NULL;
	printf("Reading Entry N° %u, %s, %i\n", type, name, index);

	if (type == (unsigned char)2) printf("DIR %s index: %u\n", name, index);
	else if (type == (unsigned char)4) printf("FILE %s index: %u\n", name, index);

	return dir_parser_init(type, name, index);
};

/** Lee un bloque de directorio en el indice index
 * Retorna un array de Dir_parser que representan los archivos en este directorio
 */
Dir_parser **read_dir_block(unsigned int index, unsigned char *bytemap)
{
	printf("\n---------------------\nREADING BLOCK %i\n", index);
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

/** Lee los directorios de manera recursiva DFS*/
void load_dir(Graph *graph, Node *parent)
{
	Dir_parser **dir_entries;
	// Primera llamada crea al root
	if (!parent)
	{
		dir_entries = read_dir_block(0, graph->bytemap);
		Dir_parser *root_entry = dir_parser_init((unsigned char)2, "root", 0);
		parent = node_init(root_entry);
		dir_parser_destroy(root_entry);
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
		Node *node = node_init(dir_entries[i]);
		dir_parser_destroy(dir_entries[i]);

		// Llamado recursivo solo si es directorio y no es el padre
		if (node->type == (unsigned char)2) load_dir(graph, node);
		// Agregamos la entrada al directorio padre
		if (parent) graph_append(graph, parent, node);
	};
	free(dir_entries);
}

/** Arma el arbol de directorios*/
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
