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

Directory* directory_init(void)
{
  Directory* directory = malloc(sizeof(Directory));
  return directory;
}

void directory_destroy(Directory* directory)
{
  free(directory);
}

Dir_parser* dir_parser_init(unsigned char type, char* name, unsigned int index)
{
  Dir_parser* dir_parser = malloc(sizeof(Dir_parser));

  dir_parser -> type = type;
  dir_parser -> name = name;
  dir_parser -> index = index;

  return dir_parser;
}

void dir_parser_destroy(Dir_parser* dir_parser)
{
  free(dir_parser);
}