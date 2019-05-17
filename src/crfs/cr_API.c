#include "cr_API.h"
#include "functions/functions.h"
#include "structs/graph.h"

char* DISK_PATH;

void cr_mount(char* diskname)
{
  DISK_PATH = diskname;
  Graph* graph = load_disk();
  graph_printer(graph);
  graph_destroy(graph);
}

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

void cr_ls(char* path)
{
  Graph* graph = load_disk();
  // graph_printer(graph);
  /** Work Here */
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
