#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "structs/structs.h"
#include "cr_API.h"


int main(int argc, char *argv[])
{
  // Se monta el disco.
  cr_mount("disks/simdiskfilled.bin");
  cr_ls("path");
  // crFILE* file_desc = cr_open("../../files/test.txt", 'w');
  // Suponga que abrio y leyo un archivo desde su computador
  // almacenando su contenido en un arreglo f, de 300 byte. cr_write(file_desc, f, 300);
  // cr_cp("test.txt", "copy.txt");
  // cr_close(file_desc);
  return 0;
}