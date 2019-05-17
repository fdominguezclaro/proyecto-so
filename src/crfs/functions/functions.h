#pragma once

#include "../structs/graph.h"

int read_dir_block(char* path);

void load_dir(Graph* graph, Node* parent);

Graph* load_disk(void);

/** Convierte un Byte a Bits y retorna la cantidad de 1's */
int byteToBits(unsigned char byte);
