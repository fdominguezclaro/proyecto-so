#pragma once
#include "../structs/graph.h"


////////////////////////////////////
//        Public Functions        //
////////////////////////////////////

/** Convierte un Byte a Bits, retorna la cantidad de 1's
 * Printea el número binario: Necesario para cr_bitmap()
 */
int byteToBits(unsigned char byte);

Graph *load_disk(void);

/** Recorta un string insertando un caractre de termino
 * Obtenido de: https://stackoverflow.com/questions/27414696/remove-last-four-characters-from-a-string-in-c
*/
void trim_end(char *str, int n);

int next_free_block(unsigned char *bytemap);

/** Busca la siguiente entrada valida en un bloque de directorio */
int next_free_entry(unsigned int index);

/** Escribe un bloque de directorio en el disco */
void write_dir_block(unsigned int index, Dir_parser* dir);

/** Escribe en el bitmap el estado de un bloque dado */
void write_bitmap(unsigned int index, unsigned int value);