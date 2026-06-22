#include<stdio.h>
#include<string.h>
#include "view.h"
#include "edit.h"
#include "types.h"

//Prototype
OperationType check_operation(char *);
void display_help(void);

int main(int argc,char *argv[]){
    OperationType ret=check_operation(argv[1]);
    if(ret==Operation_view){
        View vInfo;
        if((validate_view_args(argc,argv,&vInfo))==success){
            do_view(&vInfo);
        }
        else{
            printf("Get Help: ./a.out -h\n");
            printf("ERROR: validation error!\n");
        }

    }
    else if(ret==Operation_edit){
        Edit eInfo;
        if((validate_edit_args(argc,argv,&eInfo))==success){
            do_edit(&eInfo,argv);
        }
        else{
            printf("Get Help: ./a.out -h\n");
            printf("ERROR: validation error!\n");
        }

    }
    else if(ret==Operation_help){
        if(argc==2){
            display_help();
        }
        else{
            printf("Get Help: ./a.out -h\n");
            printf("ERROR: validation error!\n");
        }
        
    }
    else if(ret==type_error){
        printf("Get Help: ./a.out -h\n");
        printf("Invalid Operation!\n");
    }
    return 0;
}

OperationType check_operation(char *symbol){
    if(!strcmp(symbol,"-v")){
        return Operation_view;
    }
    if(!strcmp(symbol,"-e")){
        return Operation_edit;
    }
    if(!strcmp(symbol,"-h")){
        return Operation_help;
    }
    return type_error;

}

void display_help(void){
    printf("======================================================\n");
    printf("                  MP3 TAG READER HELP                 \n");
    printf("======================================================\n");
    printf("Options:\n");
    printf("  %-25s %s\n","-v <file>","View MP3 tag information");
    printf("  %-25s %s\n","-e <edit_tag> <file>","Edit MP3 tag");
    printf("\n");
    printf("Tag Options for editing:\n");
    printf("  -t   Edit Title\n");
    printf("  -a   Edit Artist\n");
    printf("  -A   Edit Album\n");
    printf("  -y   Edit Year\n");
    printf("  -m   Edit Genre\n");
    printf("  -c   Edit Comment\n");
    printf("\n");
    printf("Examples:\n");
    printf("  View tags:\n");
    printf("      ./a.out -v sample.mp3\n");
    printf("  Edit title:\n");
    printf("      ./a.out -e -t \"New Song Name\" sample.mp3\n");
    printf("  Edit album:\n");
    printf("      ./a.out -e -A \"New Album Name\" sample.mp3\n");
    printf("=========================================================\n");
}
