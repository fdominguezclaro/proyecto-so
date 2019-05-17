#include <stdio.h>

#include "cr_API.h"
#include "functions/functions.h"
#include "structs/graph.h"

char* DISK_PATH;

/** Monta el disco */
void cr_mount(char* diskname)
{
  DISK_PATH = diskname;
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
  /** Work Here */
  graph_destroy(graph);
}

/** Recibe un path de la forma "/carpeta/file_or_directory" */
void cr_ls(char* path)
{
  Graph* graph = load_disk();
  // graph_printer(graph);
  Node *entry = graph_search(graph -> root, path);
  if (!entry) printf("Path inexistente\n");
  else {
    if (entry -> type == (unsigned char) 4) printf("%s es un archivo, no un directorio\n", path);
    else {
      printf("Directorios y archivos en %s:\n", path);
      for (int i = 0; i < entry -> count; i++)
      {
        if (i != entry -> count - 1) printf("%s, ", entry->childs[i] -> name);
        else printf("%s\n", entry->childs[i] -> name);
      }
    }
  }
  graph_destroy(graph);
}

int cr_mkdir(char *foldername)
{
  Graph* graph = load_disk();
  // graph_printer(graph);
  /** Work Here */
  graph_destroy(graph);
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
  // graph_printer(graph);
  /** Work Here */
  graph_destroy(graph);
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
