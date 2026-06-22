#ifndef EDIT_H
#define EDIT_H
#include<stdio.h>
#include "types.h"

typedef struct Mp3Edit{
    char *new_data;
    char *tag;

    FILE *fsrc_mp3;
    char *src_name;

    FILE *ftemp_mp3;
    char *temp_name;

} Edit;

Status validate_edit_args(int argc,char *argv[],Edit *eInfo);

Status open_edit_files(Edit *eInfo);

Status fetch_edit_details(Edit *eInfo,char *argv[]);

Status tag_store(Edit *eInfo,char *argv[]);

Status endian_change(Uint *size);

Status copy_remaining_data(Edit *eInfo);

Status do_edit(Edit *eInfo,char *argv[]);

#endif