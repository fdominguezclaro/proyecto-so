#pragma once
#include "structs/structs.h"

//////////////////////////
//        Structs       //
//////////////////////////

/** Representa un archivo abierto */
typedef struct FILE {
    Dir_parser *directory;
    Index_block *iblock;
    // if mode == 0 -> read mode, else write mode
    unsigned char mode;
    char *path;
} crFILE;

////////////////////////////////////
//        Public Functions        //
////////////////////////////////////

/** Monta el disco */
void cr_mount(char* diskname);

/** Printea el bitmap, la cantidad de 1's y 0's */
void cr_bitmap();

int cr_exists(char* path);

void cr_ls(char* path);

int cr_mkdir(char *foldername);

crFILE* cr_open(char* path, char mode);

int cr_read(crFILE* file_desc, void* buffer, int nbytes);

int cr_write(crFILE* file_desc, void* buffer, int nbytes);

int cr_close(crFILE* file_desc);

int cr_rm(char* path);

int cr_hardlink(char* orig, char* dest);

int cr_unload(char* orig, char* dest);

int cr_load(char* orig);

void crFILE_printer(crFILE *cr_file);

void crFILE_destroy(crFILE *cr_file);
