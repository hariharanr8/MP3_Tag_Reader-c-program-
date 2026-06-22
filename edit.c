#include<stdio.h>
#include<string.h>
#include<ctype.h>
#include<stdlib.h>
#include "edit.h"
#include "types.h"

Status validate_edit_args(int argc,char *argv[],Edit *eInfo){
    if(argc!=5){
        return failure;
    }

    if(strlen(argv[2])!=2){
        return failure;
    }

    int len=(strlen(argv[4])-4);
    if(!strcmp(&argv[4][len],".mp3")){
        eInfo->src_name=argv[4];
        return success;
    }

    return failure;
}

Status open_edit_files(Edit *eInfo){
    printf("INFO: Opening required file\n");
    //open source file to read
    eInfo->fsrc_mp3=fopen(eInfo->src_name,"rb");
    if(eInfo->fsrc_mp3==NULL){
        perror("fopen");
        fprintf(stderr, "ERROR: Unable to open file %s\n",eInfo->src_name);
        return failure;
    }
    //open temp file to write
    eInfo->temp_name="temp.mp3";
    eInfo->ftemp_mp3=fopen(eInfo->temp_name,"wb");
    if(eInfo->ftemp_mp3==NULL){
        perror("fopen");
        fprintf(stderr, "ERROR: Unable to open file %s\n",eInfo->temp_name);
        return failure;
    }

    return success;
}

Status tag_store(Edit *eInfo,char *argv[]){
    
    char *ptr_tag[6]={"TPE1","TIT2","TALB","TYER","TCON","COMM"};
    //-a(artist),-t(title),-A(album),-y(year),-m(music),-c(comment)
    char *name[6]={"-a","-t","-A","-y","-m","-c"};
    for(int i=0;i<6;i++){
        if(!strcmp(argv[2],name[i])){
            eInfo->tag=ptr_tag[i];
            //new_data store
            eInfo->new_data=argv[3];
            return success;
        }
    }
    return failure;

}

Status endian_change(Uint *size){
    unsigned char *temp=(unsigned char*)size;
    for(int i=0;i<2;i++){
        temp[i]=temp[i] ^ temp[4-i-1];
        temp[4-i-1]=temp[i] ^ temp[4-i-1];
        temp[i]=temp[i] ^ temp[4-i-1];
    }
    return success;
}

Status copy_remaining_data(Edit *eInfo){
    printf("INFO: Copying Left Over Data\n");
    char buffer[4096]; // Safe 4KB transfer window
    size_t bytes_read;

    // Standard stream replication loop
    while ((bytes_read = fread(buffer, 1, sizeof(buffer), eInfo->fsrc_mp3)) > 0) {
        fwrite(buffer, 1, bytes_read, eInfo->ftemp_mp3);
    }
    return success;
}

Status fetch_edit_details(Edit *eInfo,char *argv[]){
    printf("INFO: Fetch Details\n");
    char head[11];
    //src header copy to temp file
    fread(head,ID3_HEADER_SIZE,1,eInfo->fsrc_mp3);
    fwrite(head,ID3_HEADER_SIZE,1,eInfo->ftemp_mp3);

    //user tag store
    if(tag_store(eInfo,argv)==failure){
        return failure;
    }
    char tag[5];
    //read tag to src file
    while(fread(tag,4,1,eInfo->fsrc_mp3)==1){
        tag[4]='\0';
        if(!isalpha(tag[0]))
        {
            printf("Reached non-frame data\n");
            break;
        }
        //compare tag to update data
        if(!strcmp(tag,eInfo->tag)){
            //write tag to the temp file
            fwrite(tag,4,1,eInfo->ftemp_mp3);
            //length of new data covert to big endian
            Uint tl_size = strlen(eInfo->new_data)+1;
            //convert little to big endian
            Uint tb_size=tl_size;
            endian_change(&tb_size);
            //write big_endian formated size
            fwrite(&tb_size,4,1,eInfo->ftemp_mp3);
            //src file size skip and store in little endian
            Uint s_size;
            fread(&s_size,4,1,eInfo->fsrc_mp3);
            //convert big to little endian
            endian_change(&s_size);
            //read src file-->flag of 2bytes and 1byte for encoding write to temp file
            char flag[3];
            fread(flag,3,1,eInfo->fsrc_mp3);
            fwrite(flag,3,1,eInfo->ftemp_mp3);
            //write new data to temp file(use size)
            fwrite(eInfo->new_data,tl_size-1,1,eInfo->ftemp_mp3);
            //skip old data in src file
            fseek(eInfo->fsrc_mp3,s_size-1,SEEK_CUR);
            //copy remaining data in temp file
            if((copy_remaining_data(eInfo))==success){
                printf("INFO: Done\n");
            }
            fclose(eInfo->fsrc_mp3);
            fclose(eInfo->ftemp_mp3);
            //remove and rename  function write
            remove(eInfo->src_name);
            rename(eInfo->temp_name,eInfo->src_name);
            break;

        }
        else{
            //store tag data in temp file
            fwrite(tag,4,1,eInfo->ftemp_mp3);
            //read size to store temp file and convert to little endian
            Uint size;
            fread(&size,4,1,eInfo->fsrc_mp3);
            fwrite(&size,4,1,eInfo->ftemp_mp3);
            endian_change(&size);
            //read flag 2 and 1bytes for encoding to store temp file
            char flag[3];
            fread(flag,3,1,eInfo->fsrc_mp3);
            fwrite(flag,3,1,eInfo->ftemp_mp3);
            //old data store upto size
            char *data=malloc(size-1);
            if(data != NULL){
                fread(data,size-1,1,eInfo->fsrc_mp3);
                fwrite(data,size-1,1,eInfo->ftemp_mp3);
                free(data);
            } 
        }
    }
    return success;
}

Status do_edit(Edit *eInfo,char *argv[]){
    if((open_edit_files(eInfo))==success){
        printf("INFO: Done\n");
    }

    if((fetch_edit_details(eInfo,argv))==success){
        printf("INFO: Done\n");
    }

    return success;
}