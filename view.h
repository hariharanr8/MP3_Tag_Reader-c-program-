#ifndef VIEW_H
#define VIEW_H
#include<stdio.h>
#include "types.h"

typedef struct Mp3View{
    char artist_name[300];
    char title[300];
    char album[300];
    char year[300];
    char content_type[300];
    char composer[300];

    FILE *fptr_mp3;
    char *file_name;

} View;

Status validate_view_args(int argc,char *argv[],View *vInfo);

Status open_files(View *vInfo);

Status fetch_details(View *vInfo);

Status tag_data_size(FILE* fptr,Uint *size);

Status little_endian(Uint *size);

Status view_details(View *vInfo);

Status do_view(View *vInfo);


#endif