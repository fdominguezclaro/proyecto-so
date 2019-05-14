#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "../structs/graph.h"
#include "../structs/structs.h"

#define BLOCK_SIZE 2048

extern char* DISK_PATH;

static void printBits(unsigned char byte)
{
   for(int bit=(sizeof(unsigned char) * 8);bit>0; bit--)
   {
      printf("%i ", byte & 0x01);
      byte = byte >> 1;
   }
}

/** true si el bloque es valido en el bitmap */
static bool valid(int block_index, unsigned char* bytemap)
{
		unsigned char byte = bytemap[block_index  / 8];
		int position = 7 - block_index % 8;

    return (byte >> position) & 0x1;
}

/** Carga el bitmap en bytes */
static unsigned char* load_bitmap(void)
{
	FILE* file = fopen(DISK_PATH, "rb");
	fseek(file, BLOCK_SIZE, SEEK_SET);
	unsigned char* bytemap = malloc(sizeof(unsigned char) * BLOCK_SIZE * 4);
	fread(bytemap, sizeof(unsigned char), BLOCK_SIZE * 4, file);
	fclose(file);

	bool is_valid = valid(0, bytemap);
	printf("%i", is_valid);
	for (int i = 0; i < BLOCK_SIZE * 4; i++) {
		printf("indice: %i - ", i * 8);
		printBits(bytemap[i]);
		printf("\n");
	}

	return bytemap;
}

Dir_parser* read_entry(unsigned char *buffer, unsigned char* bytemap)
{
	fflush(stdout);

	unsigned char type = buffer[0];
  char* name = (char*) (buffer + 1);
  unsigned int index = (unsigned int)buffer[30] * 256 + (unsigned int)buffer[31];

	bool is_valid = valid(index, bytemap);

	if (type == (unsigned char) 1) {
		// puts("Invalid entry");
		return NULL;
	} else if (is_valid) {
		if (type == (unsigned char) 2) {
			printf("DIR %s index: %u\n", name, index);
		} else if (type == (unsigned char) 4){
			printf("FILE %s index: %u\n", name, index);
		};
		return dir_parser_init(type, name, index);
	};

	return NULL;
};

Dir_parser** read_dir_block(unsigned int index, unsigned char * bytemap)
{
	printf("READING %i\n", index);
  FILE* file = fopen(DISK_PATH, "rb");
	if (file == NULL) {
		perror("Failed to open path");
	}

	unsigned char *buffer = malloc(sizeof(unsigned char ) * 32 );

	Dir_parser** dir_parser = malloc(sizeof(Dir_parser*) * 64);

	// Lee un bloque completo
	for (int i = 0; i < 64; i++) {
		fseek(file, (32 * i) + (index * BLOCK_SIZE), SEEK_SET);
		fread(buffer, sizeof(unsigned char), 32, file);
		dir_parser[i] = read_entry(buffer, bytemap);
	};

	free(buffer);
	fclose(file);
	return dir_parser;
};

/** Lee los directorios de manera recursiva DFS*/
void load_dir(Graph* graph, Node* parent)
{
	Dir_parser** dir_entries;
	if (!parent) dir_entries = read_dir_block(0, graph -> bytemap);
	else dir_entries = read_dir_block(parent -> index, graph -> bytemap);

	/**
	 * ERROR: EL NODO PADRE NO SE ESTA CREANDO
	 *
	 *
	 *
	 *
	 */

	// Recorremos todas las entradas de ese directorio
	for (int i = 0; i < 64; i++) {
		// Entrada invalida
		if (!dir_entries[i]) continue;
  	Node* node = node_init(dir_entries[i]);
		dir_parser_destroy(dir_entries[i]);
		// Llamado recursivo solo si es directorio
		if (node -> type == (unsigned char)2) load_dir(graph, node);
		// Agregamos la entrada al directorio padre
		graph_append(graph, parent, node);
	};
	free(dir_entries);
}

/** Arma el arbol de directorios*/
Graph* load_disk(void)
{
	// Cargamos el bitmap
	unsigned char* bytemap = load_bitmap();
  Graph* graph = graph_init(bytemap);
	// Creamos el directorio root
	load_dir(graph, NULL);
	return graph;
}
