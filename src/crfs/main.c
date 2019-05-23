#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "structs/structs.h"
#include "cr_API.h"


int main(int argc, char *argv[])
{
  if (argc != 2)
  {
    printf("Modo de uso: ./crfs simdisk.bin\n");
    return 0;
  }

  // Se monta el disco.
  cr_mount(argv[1]);
  cr_ls("/root");
  crFILE *file2 = cr_open("/root/Program in C.mkv", 'r');
  
  printf("Directory:\n");
  printf("Name: %s | type: %u | index: %u | offset: %u\n", 
  file2->directory->name,
  file2->directory->type,
  file2->directory->index,
  file2->directory->offset);

  printf("iblock:\n");
  printf("Size: %u | nhls: %u | dp: %u | ib: %u\n", 
  file2->iblock->size,
  file2->iblock->n_hardlinks,
  *file2->iblock->data_pointers,
  *file2->iblock->indirect_blocks);

  crFILE_destroy(file2);
  // cr_bitmap();

  /** DONE
   * cr_mount(char *diskname);
   * cr_bitmap();
   * cr_ls("path");
   * cr_exists(char *path);
   * cr_mkdir(char *foldername);
   * cr_rm(char *path);
   * cr_hardlink(char *orig, char *dest);
  */

  /** TO DO
   * cr_open(char *path, char mode);
   * cr_read(crFILE *file_desc, void *buffer, int nbytes);
   * cr_write(crFILE *file_desc, void *buffer, int nbytes);
   * cr_close(crFILE *file_desc);
   * cr_unload(char *orig, char *dest);
   * cr_load(char *orig);
  */

  // crFILE* file_desc = cr_open("../../files/test.txt", 'w');
  // Suponga que abrio y leyo un archivo desde su computador
  // almacenando su contenido en un arreglo f, de 300 byte. cr_write(file_desc, f, 300);
  // cr_cp("test.txt", "copy.txt");
  // cr_close(file_desc);
  return 0;
}