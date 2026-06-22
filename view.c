#include<stdio.h>
#include<string.h>
#include "view.h"
#include "types.h"

Status validate_view_args(int argc,char *argv[],View *vInfo){
    if(argc!=3){
        return failure;
    }
    int len=(strlen(argv[2])-4);
    if(!strcmp(&argv[2][len],".mp3")){
        vInfo->file_name=argv[2];
        return success;
    }
    return failure;
}

Status open_files(View *vInfo){
    printf("INFO: Opening required file\n");
    vInfo->fptr_mp3=fopen(vInfo->file_name,"r");
    if(vInfo->fptr_mp3==NULL){
        perror("fopen");
        fprintf(stderr, "ERROR: Unable to open file %s\n",vInfo->file_name);
        return failure;
    }
    return success;
}

Status little_endian(Uint *size){
    unsigned char *temp=(unsigned char*)size;
    for(int i=0;i<2;i++){
        temp[i]=temp[i] ^ temp[4-i-1];
        temp[4-i-1]=temp[i] ^ temp[4-i-1];
        temp[i]=temp[i] ^ temp[4-i-1];
    }
    return success;
}

Status tag_data_size(FILE *fptr,Uint *size){
    fread(size,4,1,fptr);
    little_endian(size);
    return success;
}

Status fetch_details(View *vInfo){
    printf("INFO: Fetch Details\n");
    fseek(vInfo->fptr_mp3,ID3_HEADER_SIZE,SEEK_SET); //skip mp3 header 
    Uint size;
    char str[5];
    int tag=0,found=0;
    char *ptr_tag[6]={"TPE1","TIT2","TALB","TYER","TCON","COMM"};
    char *name[6]={vInfo->artist_name,vInfo->title,vInfo->album,vInfo->year,vInfo->content_type,vInfo->composer};
    //read tag name to copy string
    while(tag<6 && fread(str,4,1,vInfo->fptr_mp3)==1){
        found=0;
        str[4]='\0';
        tag_data_size(vInfo->fptr_mp3,&size);
        fseek(vInfo->fptr_mp3,3,SEEK_CUR); //skip flag in mp3 file
        for(int j=0;j<6;j++){
            if(!(strcmp(ptr_tag[j],str))){
                fread(name[j],size-1,1,vInfo->fptr_mp3);//collect data size times
                name[j][size-1]='\0';
                tag++;
                found=1;
                break;
            }
        }
        //unwanted tag data skip
        if(!found){
            fseek(vInfo->fptr_mp3,size-1,SEEK_CUR);
        }

    }
    fclose(vInfo->fptr_mp3);
    return success;
}

Status view_details(View *vInfo){
    printf("INFO: Selected View Details\n");
    printf("----------------------------------------------------------------------------------\n");
    printf("MP3 TAG READER AND EDITOR FOR ID3v2\n");
    printf("----------------------------------------------------------------------------------\n");
    printf("TITLE   : %s\n",vInfo->title);
    printf("ARTIST  : %s\n",vInfo->artist_name);
    printf("ALBUM   : %s\n",vInfo->album);
    printf("YEAR    : %s\n",vInfo->year);
    printf("MUSIC   : %s\n",vInfo->content_type);
    printf("COMMENT : %s\n",vInfo->composer);
    printf("----------------------------------------------------------------------------------\n");
    return success;
}

Status do_view(View *vInfo){
    if((open_files(vInfo))==success){
        printf("INFO: Done\n");
    }
    else{
        return failure;
    }
    if((fetch_details(vInfo))==success){
        printf("INFO: Done\n");
    }
    if((view_details(vInfo))==success){
        printf("INFO: DONE\n");
    }
    return success;
}