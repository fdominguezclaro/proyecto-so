#include "structs.h"


Data* data_init()
{
  Data* data = malloc(sizeof(Data));
}

void data_destroy(Data* data){
  free(data);
}

File* file_init()
{
  File* file = malloc(sizeof(File));
}

void file_destroy(File* file)
{
  free(file);
}

Directory* directory_init()
{
  Directory* directory = malloc(sizeof(Directory));
}

void directory_destroy(Directory* directory)
{
  free(directory);
}
