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