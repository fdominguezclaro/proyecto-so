#pragma once

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

typedef struct dir_parser
{
  unsigned char type;
  char* name;
  unsigned int index;
} Dir_parser;

/** Representa un bloque de datos */
typedef struct data
{
  uint32_t size;
} Data;

/** Representa un bloque de indice */
typedef struct file
{
  uint32_t size;
  uint32_t n_hardlinks;
  Data* data_pointer;
} File;

/** Representa un bloque de directorio */
typedef struct directory
{
  uint8_t dirty_bit;
  char* name;
  struct directory* dir_pointer;
  File* file_pointer;
  bool is_file;
} Directory;

Data* data_init();

void data_destroy(Data* data);

File* file_init();

void file_destroy(File* file);

Directory* directory_init();

void directory_destroy(Directory* directory);

Dir_parser* dir_parser_init(unsigned char type, char* name, unsigned int index);

void dir_parser_destroy(Dir_parser* dir_parser);