#include<stdio.h>
#include <dirent.h>
#include <sys/stat.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>

int mv_command(char* source_input,char* destination_input,int not_overwrite,int ask_overwrite, int print_transfer,int number_source_files)
{
    char cwd[1024];
    getcwd(cwd,1024);
    char* source_location = (char*) malloc(strlen(source_input) + 10+ strlen(cwd));
    char* destination_location = (char*) malloc(strlen(destination_input) + 3);
    char source_file_name[100];
    char destination_file_name[100];
    int check_source=0;
    int check_destination=0;
    int size_source_file=0;
    int size_destination_file=0;
    char ch;
    char decision;
    int total_length_source_location=strlen(cwd)+strlen(source_input);
    bool destination_is_dir=false;
    int i=0;
    

    struct dirent *dir1;
    DIR* dr1 = opendir(".");
    if(dr1==NULL){
        printf("Couldn't not open current directory");
        return 0;
    }
    while((dir1=readdir(dr1))!=NULL ){
        if(strcmp(source_input,dir1->d_name)==0)
        {
            snprintf(source_location,total_length_source_location+2,"%s/%s",cwd,source_input);
            strcpy(source_file_name,dir1->d_name);
            check_source++;
            i=strlen(source_location);
            source_location[i]='\0';
        }
        if(strcmp(destination_input,dir1->d_name)==0)
        {
            snprintf(destination_location,strlen(destination_input)+3,"./%s",destination_input);
            strcpy(destination_file_name,dir1->d_name);
            check_destination++;
        }
    }
    if(check_source==0)
    {
        strcpy(source_location,source_input);
        i=strlen(source_location);
        source_location[i]='\0';
        struct stat source_info;
        if(stat(source_location,&source_info)!=0)
        {
            printf("mv: %s: No such file or directory\n",source_input);
            return 0;
        }
        int i=0;
        i=strlen(source_location);
        int index =i;
        while(source_location[index]!='/' && index>=0) 
        {
            index--;
        }
        size_source_file=i-index;
        strncpy(source_file_name,source_location+index+1,size_source_file);
        
    }

    if(check_destination==0 && destination_input[0]=='/')
    {
        strcpy(destination_location,destination_input);
        int i=0;
        i=strlen(destination_location);
        int index =i;
        while(destination_location[index]!='/' && index>=0) 
        {
            index--;
        }
        size_destination_file=i-index;
        strncpy(destination_file_name,destination_location+index+1,size_destination_file);
    }

    closedir(dr1);
    
    FILE* source_file = fopen(source_location, "r");
    if (source_file == NULL) {
        printf("Error opening source file.\n");
        return 0;
    }
    struct stat destination_info;
    if (stat(destination_location, &destination_info) == 0 && S_ISDIR(destination_info.st_mode))
    {
        destination_is_dir=true;
        chdir(destination_location);
        struct dirent*dir2;
        int already_there=0;
        DIR*dr2=opendir(destination_location);
        if(dr2==NULL){
        printf("Couldn't not open current directory");
        return 0;
        }
        while((dir2=readdir(dr2))!=NULL)
        {
            if(strcmp(source_file_name,dir2->d_name)==0)
            {
                already_there++;
            }
        }
        if(already_there)
        {
            if(ask_overwrite)
            {
                printf("overwrite %s/%s? (y/n [n])",destination_input,source_file_name);
                scanf("%s",&decision);
                if (decision=='y')
                {
                    FILE* destination_file = fopen(source_file_name, "w");
                    if (destination_file == NULL)
                    {
                        printf("Error opening destination file.\n");
                        fclose(source_file);
                        return 0;
                    }
                    while ((ch = fgetc(source_file)) != EOF) 
                    {
                        fputc(ch, destination_file);
                    }
                    if(print_transfer)
                    {
                    printf("%s -> %s/%s \n",source_input,destination_input,source_file_name);
                    }
                    fclose(source_file);
                    fclose(destination_file);
                    chdir(cwd);
                    return 1;
                }
                else 
                {
                    printf("not overwritten\n");
                    chdir(cwd);
                    return 1;
                }
            }
            else if (not_overwrite)
            {
                printf("shouldn't overwrite\n");
                chdir(cwd);
                return 1;
            }
        }
        FILE* destination_file = fopen(source_file_name, "w");
        if (destination_file == NULL) {
            printf("Error opening destination file.\n");
            fclose(source_file);
            chdir(cwd);
            return 0;
       }
       while ((ch = fgetc(source_file)) != EOF) 
       {
            fputc(ch, destination_file);
       }
        if(print_transfer)
        {
         printf("%s -> %s/%s \n",source_input,destination_input,source_file_name);
        }
        fclose(source_file);
        unlink((const char*)source_location);
        fclose(destination_file);
    }
    else if (stat(destination_location, &destination_info) == 0 && S_ISREG(destination_info.st_mode))
    {
        destination_is_dir=false;
        if(number_source_files>=2)
        {
            printf("mv: %s is not a directory\n",destination_input);
            return 0;
        }

        else
        {   
            if(ask_overwrite)
            {
                printf("overwrite %s? (y/n [n])",destination_file_name);
                scanf("%s",&decision);
                if (decision=='y')
                {
                    if(check_destination==0)
                    {
                        int i=strlen(destination_location);
                        destination_location[i-size_destination_file]='\0';
                    }
                    chdir(destination_location);
                    FILE* destination_file = fopen(destination_file_name, "w");
                    if (destination_file == NULL) {
                        printf("Error opening destination file.\n");
                        fclose(source_file);
                        return 0;
                    }
                while ((ch = fgetc(source_file)) != EOF) 
                {
                        fputc(ch, destination_file);
                }
                if(print_transfer)
                {
                printf("%s -> %s \n",source_input,destination_input);
                }

                fclose(source_file);
                unlink((const char*)source_location);
                fclose(destination_file); 
                }
                else 
                {
                    printf("not overwritten\n");
                }
            }
            if (not_overwrite)
            {
                printf("shouldn't overwrite");
            }
            if(ask_overwrite==0 && not_overwrite==0)
            {
            if(check_destination==0)
                {
                    int i=strlen(destination_location);
                    destination_location[i-size_destination_file]='\0';
                }
                chdir(destination_location);
                FILE* destination_file = fopen(destination_file_name, "w");
                if (destination_file == NULL) {
                    printf("Error opening destination file.\n");
                    fclose(source_file);
                    return 0;
                    }
                while ((ch = fgetc(source_file)) != EOF) 
                {
                    fputc(ch, destination_file);
                }
                if(print_transfer)
                {
                printf("%s -> %s \n",source_input,destination_input);
                }

                fclose(source_file);
                unlink((const char*)source_location);
                fclose(destination_file);  
            }
        }
        
    }
    else if(check_destination==0 && destination_input[0]!='/')
    {
        destination_is_dir=false;
        if(number_source_files>=2)
        {
            printf("mv: %s is not a directory\n",destination_input);
            return 0;
        }
        else
        {

            strcpy(destination_location,".");
            strcpy(destination_file_name,destination_input);
            chdir(destination_location);
            FILE* destination_file = fopen(destination_file_name, "w");

            if (destination_file == NULL) {
                printf("Error opening destination file.\n");
                fclose(source_file);
                return 0;
            }
             while ((ch = fgetc(source_file)) != EOF) 
            {
                fputc(ch, destination_file);
             }
            if(print_transfer)
            {
            printf("%s -> %s \n",source_input,destination_input);
            }

            fclose(source_file);
            unlink((const char*)source_location);
            fclose(destination_file);
        }
    }
    else if(stat(destination_location, &destination_info)!=0)
    {
        if(number_source_files>=2)
        {
            printf("mv: %s is not a directory\n",destination_input);
            return 0;
        }
        else
        {
            int i=strlen(destination_location);
            destination_location[i-size_destination_file]='\0';
            chdir(destination_location);
            FILE* destination_file = fopen(destination_file_name, "w");
            if (destination_file == NULL) {
                printf("Error opening destination file.\n");
                fclose(source_file);
                return 0;
            }
            while ((ch = fgetc(source_file)) != EOF) 
            {
                fputc(ch, destination_file);
            }
            if(print_transfer)
            {
            printf("%s -> %s \n",source_input,destination_input);
            }
            fclose(source_file);
            unlink((const char*)source_location);
            fclose(destination_file);
        }
    }
    chdir(cwd);
    free(source_location);
    free(destination_location);
    return 1;
}

