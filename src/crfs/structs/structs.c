#include "structs.h"


Data* data_init()
{
  Data* data = malloc(sizeof(Data));
  return data;
}

void data_destroy(Data* data){
  free(data);
}

File* file_init()
{
  File* file = malloc(sizeof(File));
  return file;
}

void file_destroy(File* file)
{
  free(file);
}

Directory* directory_init()
{
  Directory* directory = malloc(sizeof(Directory));
  return directory;
}

void directory_destroy(Directory* directory)
{
  free(directory);
}
