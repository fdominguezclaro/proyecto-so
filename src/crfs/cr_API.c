#include "cr_API.h"
#include "functions/functions.h"
#include "structs/graph.h"

char* DISK_PATH;

void cr_mount(char* diskname)
{
  DISK_PATH = diskname;
}

void cr_bitmap()
{

}

int cr_exists(char* path)
{

}

void cr_ls(char* path)
{
  Graph* graph = load_disk();
  graph_destroy(graph);
}

int cr_mkdir(char *foldername)
{

}

crFILE* cr_open(char* path, char mode)
{

}

int cr_read(crFILE* file_desc, void* buffer, int nbytes)
{

}

int cr_write(crFILE* file_desc, void* buffer, int nbytes)
{

}

int cr_close(crFILE* file_desc)
{

}

int cr_rm(char* path)
{

}

int cr_hardlink(char* orig, char* dest)
{

}

int cr_unload(char* orig, char* dest)
{

}

int cr_load(char* orig)
{

}