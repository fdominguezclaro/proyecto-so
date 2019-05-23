#include <stdio.h>
#include <errno.h>

#include "cr_API.h"
#include "functions/functions.h"
#include "structs/graph.h"

extern int errno;
int errnum;

char* DISK_PATH;

/** Monta el disco */
void cr_mount(char* diskname)
{
  FILE * pf;
  pf = fopen (diskname, "rb");
  if (pf == NULL) {
    errnum = errno;
    fprintf(stderr, "Error mounting disk: %s\n", strerror(errnum));
  } else {
    DISK_PATH = diskname;
    fclose (pf);
  }
}

/** Printea el bitmap, la cantidad de 1's y 0's */
void cr_bitmap()
{
  Graph* graph = load_disk();
  int count_ones = 0;
  for (int i = 0; i < 8192; i++) {
    count_ones += byteToBits(graph -> bytemap[i]);
  }
  printf("\n%i\n", count_ones);
  printf("%i\n", 8192 * 8 - count_ones);
  graph_destroy(graph);
}

int cr_exists(char* path)
{
  Graph* graph = load_disk();
  // graph_printer(graph);
  Node *entry = graph_search(graph -> root, path);
  if (!entry) {
    errno = 2;
    fprintf(stderr, "Error opening file: %s\n", strerror(errno));
    graph_destroy(graph);
    return 0;
  }
  else {
    graph_destroy(graph);
    return 1;
  }
}

/** Recibe un path de la forma "/carpeta/file_or_directory" */
void cr_ls(char* path)
{
  Graph* graph = load_disk();
  // graph_printer(graph);
  Node *entry = graph_search(graph -> root, path);
  if (entry) {
    if (entry -> type == (unsigned char) 4) {
      errnum = ENOTDIR;
      fprintf(stderr, "Error reading %s: %s\n", path, strerror(errnum));
    }
    else {
      printf("Directorios y archivos en %s:\n", path);
      for (int i = 0; i < entry -> count; i++)
      {
        if (i != entry -> count - 1) printf("%s, ", entry->childs[i] -> name);
        else printf("%s\n", entry->childs[i] -> name);
      }
    }
  } else {
    errno = 2;
    fprintf(stderr, "Error opening file: %s\n", strerror(errno));
  }
  graph_destroy(graph);
}

int cr_mkdir(char *foldername)
{
  Graph* graph = load_disk();

  Node *aux = graph_search(graph -> root, foldername);
  if (aux) {
    errnum = EEXIST;
    fprintf(stderr, "Error: %s: %s\n", foldername, strerror(errnum));
    graph_destroy(graph);
    return 0;
  }

  // Copia del str para poder modificarlo
  char dir_copy[1000];
	strcpy(dir_copy, foldername);

  // Nombre del nuevo directorio
  char *dir_name = strrchr(foldername, '/');
  dir_name++; // Le saco el / del comienzo
  int len_name = strlen(dir_name) - 1;

  // Se guarda path del padre en dir_copy
  trim_end(dir_copy, len_name + 2);

  // Verifica si existe el path
  Node *parent = graph_search(graph -> root, dir_copy);

  if (!parent) {
    errno = 2;
    fprintf(stderr, "Error opening file: %s\n", strerror(errno));
    graph_destroy(graph);
    return 0;
  }

  // Si el arbol encuentra un archivo y no un directorio
  if (parent -> type == (unsigned char) 4) {
    errnum = ENOTDIR;
    fprintf(stderr, "Error reading: %s\n", strerror(errnum));
  }
  else {
    // Busco el siguiente bloque libre
    unsigned int index = next_free_block(graph -> bytemap);
    if (index == 0) return 0;

    // Escribo el directorio en el disco
    Dir_parser* dir_block = dir_parser_init(2, dir_name, index, 0);
    write_dir_block(parent -> index, dir_block);
    write_bitmap(index, 1);

    dir_parser_destroy(dir_block);

    graph_destroy(graph);

    return 1;
  }

  graph_destroy(graph);

  return 0;
}

crFILE* cr_open(char* path, char mode)
{
  Graph* graph = load_disk();
  // graph_printer(graph);
  /** Work Here */
  graph_destroy(graph);
}

int cr_read(crFILE* file_desc, void* buffer, int nbytes)
{
  Graph* graph = load_disk();
  // graph_printer(graph);
  /** Work Here */
  graph_destroy(graph);
}

int cr_write(crFILE* file_desc, void* buffer, int nbytes)
{
  Graph* graph = load_disk();
  // graph_printer(graph);
  /** Work Here */
  graph_destroy(graph);
}

int cr_close(crFILE* file_desc)
{
  Graph* graph = load_disk();
  // graph_printer(graph);
  /** Work Here */
  graph_destroy(graph);
}

int cr_rm(char* path)
{
  Graph* graph = load_disk();

  Node *entry = graph_search(graph -> root, path);
  // El archivo no existe
  if (!entry) {
    errno = 2;
    fprintf(stderr, "Error opening file: %s\n", strerror(errno));
    graph_destroy(graph);
    return 0;
  }

  Index_block *iblock = read_index_block(entry -> index);

  // Su entrada queda invalida
  write_byte(entry -> parent -> index, entry -> offset, (unsigned char) 1);

  // Si tiene mas hardlinks
  printf("HARDLINKS: %i\n", iblock -> n_hardlinks);

  if (iblock -> n_hardlinks > 1) {
    // Restamos un hardlink
    write_4bytes(entry -> index, 4, iblock -> n_hardlinks - 1);
  } else {
    // Lo borro del bitmap
    write_bitmap(entry -> index, 0);
    // Borramos el directorio y los bloques de datos
    for (int i = 0; i < 500; i++) write_bitmap(iblock -> data_pointers[i], 0);
    for (int i = 0; i < 10; i++) write_bitmap(iblock -> indirect_blocks[i], 0);
  }

  iblock_destroy(iblock);
  graph_destroy(graph);

  return 1;
}

int cr_hardlink(char* orig, char* dest)
{
  Graph* graph = load_disk();
  // graph_printer(graph);
  /** Work Here */
  graph_destroy(graph);
}

int cr_unload(char* orig, char* dest)
{
  Graph* graph = load_disk();
  // graph_printer(graph);
  /** Work Here */
  graph_destroy(graph);
}

int cr_load(char* orig)
{
  Graph* graph = load_disk();
  // graph_printer(graph);
  /** Work Here */
  graph_destroy(graph);
}
