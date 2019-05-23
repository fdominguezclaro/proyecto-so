#include <stdio.h>
#include <errno.h>

#include "cr_API.h"
#include "functions/functions.h"
#include "structs/graph.h"

extern int errno;
int errnum;

char* DISK_PATH;

/////////////////////////////////////
//        Private Functions        //
/////////////////////////////////////

static crFILE *crFILE_init(Dir_parser *directory, Index_block *iblock, unsigned char mode, char *path)
{
  crFILE *cr_file = malloc(sizeof(crFILE));
  
  
  cr_file -> mode = mode;
  cr_file -> iblock = iblock;
  cr_file -> path = malloc(sizeof(char) * (strlen(path) + 1));
  strcpy(cr_file -> path, path);

  cr_file -> directory = directory;
  char *dir_name = strrchr(cr_file -> path, '/');
  dir_name++;
  directory -> name = dir_name;

  return cr_file;
}

////////////////////////////////////
//        Public Functions        //
////////////////////////////////////

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
  // Si ya existe le directorio
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
    graph_destroy(graph);
    cr_mkdir(dir_copy);
    graph = load_disk();
    parent = graph_search(graph -> root, dir_copy);
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
  if (mode != 'r' && mode != 'w') {
    fprintf(stderr, "Invalid char '%c' for mode: must be 'r' or 'w'\n", mode);
    return NULL;
  }

  crFILE *cr_file;
  Index_block *iblock;
  Dir_parser *directory;
  Graph* graph = load_disk();
  Node *entry = graph_search(graph -> root, path);

  if (!entry) {
    if (mode == 'w') {  // Create a crFILE
      char dir_copy[1000];
      strcpy(dir_copy, path);
      char *dir_name = strrchr(path, '/');
      dir_name++;  // Le saco el / del comienzo
      int len_name = strlen(dir_name) - 1;
      trim_end(dir_copy, len_name + 2);

      Node *parent = graph_search(graph -> root, dir_copy);
      if (!parent) {
        errno = 2;
        fprintf(stderr, "Error opening file: %s\n", strerror(errno));
        graph_destroy(graph);
        return 0;
      } else if (parent -> type == (unsigned char) 4) {
        errnum = ENOTDIR;
        fprintf(stderr, "Error reading: %s\n", strerror(errnum));
        graph_destroy(graph);
        return 0;
      }

      unsigned int index = next_free_block(graph -> bytemap);
      if (index == 0) {
        graph_destroy(graph);
        return 0;
      }
      directory = dir_parser_init(4, dir_name, index, 0);
      write_dir_block(parent -> index, directory);
      write_bitmap(index, 1);
      iblock = read_index_block(directory -> index);

    } else {  // Cannot read an inexisting file
      errno = 2;
      fprintf(stderr, "Error opening file: %s\n", strerror(errno));
      graph_destroy(graph);
      return NULL;
    }

  } else if (entry -> type != (unsigned char) 4) {  // Cannot open a directory
    fprintf(stderr, "Error opening file: %s is a directory, not a file\n", path);
    graph_destroy(graph);
    return NULL;

  } else {  // Find the FILE and ínstance a crFILE
    directory = malloc(sizeof(Dir_parser));
    directory -> type = entry -> type;
    directory -> index = entry -> index;
    directory -> offset = entry -> offset;
    iblock = read_index_block(entry -> index);
  }

  cr_file = crFILE_init(directory, iblock, (unsigned char) mode, path);
  graph_destroy(graph);
  return cr_file;
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

  Node *entry = graph_search(graph -> root, orig);
  Node *aux_err = graph_search(graph -> root, dest);

  // El destino ya existe
  if (aux_err) {
    errno = EEXIST;
    fprintf(stderr, "Error creating hardlink: %s\n", strerror(errno));
    graph_destroy(graph);
    return 0;
  }
  // No existe el origen
  if (!entry) {
    errno = 2;
    fprintf(stderr, "Error creating hardlink: %s\n", strerror(errno));
    graph_destroy(graph);
    return 0;
  }
  // El origen es un directorio
  else if (entry -> type == (unsigned char) 2) {
    errno = EISDIR;
    fprintf(stderr, "Error creating hardlink: %s\n", strerror(errno));
    graph_destroy(graph);
    return 0;
  }
  else {
    // Copia del str para poder modificarlo
    char dir_copy[1000];
    strcpy(dir_copy, dest);

    // Nombre del nuevo directorio
    char *dir_name = strrchr(dest, '/');
    dir_name++; // Le saco el / del comienzo
    int len_name = strlen(dir_name) - 1;

    // Se guarda path del padre en dir_copy
    trim_end(dir_copy, len_name + 2);

    Node *dest_parent = graph_search(graph -> root, dir_copy);

    // Si existen carpetas que aun no se han creado las creo recursivamente
    if (!dest_parent) {
      graph_destroy(graph);
      cr_mkdir(dir_copy);
      graph = load_disk();
      dest_parent = graph_search(graph -> root, dir_copy);
      entry = graph_search(graph -> root, orig);
      aux_err = graph_search(graph -> root, dest);
    }

    // Nuevo hardlink
    Dir_parser* hl_block = dir_parser_init(4, dir_name, entry -> index, 0);
    write_dir_block(dest_parent -> index, hl_block);

    // Se lee el bloque indice
    Index_block *iblock = read_index_block(entry -> index);
    // Sumarle 1 a los hardlinks
    write_4bytes(entry -> index, 4, iblock -> n_hardlinks + 1);

    iblock_destroy(iblock);

    dir_parser_destroy(hl_block);
    graph_destroy(graph);
    return 1;
  }
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

////////////////////////////
//          Frees         //
////////////////////////////

void crFILE_destroy(crFILE *cr_file)
{
  iblock_destroy(cr_file -> iblock);
  free(cr_file -> path);
  dir_parser_destroy(cr_file -> directory);
  free(cr_file);
}

///////////////////////////////
//          Printers         //
///////////////////////////////

void crFILE_printer(crFILE *cr_file)
{
  if (!cr_file) {
    puts("--------");
    puts("No file");
    puts("--------");
    return;
  }
  puts("--------------");
  printf("Path: %s | Mode: %c\n", cr_file->path, cr_file->mode);
  puts("Directory");
  printf("Index: %u | Name: %s | Offset: %u | Type: %u\n",
    cr_file->directory->index, cr_file->directory->name,
    cr_file->directory->offset, cr_file->directory->type);
  
  puts("iblock");
  printf("DP: %u | IB: %u | nh: %u | Size: %u",
    cr_file->iblock->data_pointers, cr_file->iblock->indirect_blocks,
    cr_file->iblock->n_hardlinks, cr_file->iblock->size);

  puts("\n--------------");
}