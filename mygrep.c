#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <dirent.h>
#include <unistd.h>
#include <stdlib.h>

int grep_command(char* string,char* file_name,int print_line_number, int print_number_of_lines,int case_insensitive,int reverse)
{
    char cwd[1024];
    getcwd(cwd,1024);
    int check_file=0;
    int no_lines=0;
    char* file_location = (char*)malloc(strlen(file_name)+strlen(cwd)+10);
    int total_length_file_location=strlen(cwd)+strlen(file_name);
    struct dirent *dir;
    DIR* dir1=opendir(".");
    if(dir1==NULL)
    {
        printf("Couldn't not open current directory");
    }
    while((dir=readdir(dir1))!=NULL)
    {
        if(strcmp(dir->d_name,file_name)==0)
        {
            check_file++;
            snprintf(file_location,total_length_file_location+2,"%s/%s",cwd,file_name);
        }
    }

    if(check_file==0)
    {
        strcpy(file_location,file_name);
    }
    struct stat file_info;
    if(stat(file_location,&file_info)!=0)
    {
        printf("grep: %s: No such file or directory\n",file_name);
    }
    else if(stat(file_location,&file_info)==0 && S_ISDIR(file_info.st_mode))
    {
        printf("grep: (standard input): Is a directory\n");
    }
    else
    {
        FILE *file = fopen(file_name,"r");
        if(file == NULL)
        {
            printf("Error opening file %s\n",file_name);
            return 1;
        }

        printf("In the file %s\n",file_name);
        char line[256];
        int line_number=1;
        while(fgets(line,sizeof(line),file))
        {
            if(case_insensitive==0)
            {
                if(reverse!=0)
                {
                    if(strstr(line,string)==NULL)
                    {
                        if(print_number_of_lines==0)
                        {
                            if(print_line_number)
                            {
                                printf("%d: %s",line_number,line);  
                            }
                            else
                            {
                                printf("%s",line);
                            } 
                        }
                        no_lines++;         
                    } 
                }
                else
                {
                if(strstr(line,string))
                {
                        if(print_number_of_lines==0)
                        {
                            if(print_line_number)
                            {
                            printf("%d; %s",line_number,line);  
                            }
                            else
                            {
                            printf("%s",line);
                            }
                        }
                        no_lines++;
                    }
                }
                line_number++;
            }
            else
            {
                if(reverse!=0)
                {
                    if(strcasestr(line,string)==NULL)
                    {
                        if(print_number_of_lines==0)
                        {
                            if(print_line_number)
                            {
                                printf("%d: %s",line_number,line);  
                            }
                            else
                            {
                                printf("%s",line);
                            } 
                        }
                        no_lines++;         
                    } 
                }
                else
                {
                if(strcasestr(line,string))
                {
                        if(print_number_of_lines==0)
                        {
                            if(print_line_number)
                            {
                            printf("%d; %s",line_number,line);  
                            }
                            else
                            {
                            printf("%s",line);
                            }
                        }
                        no_lines++;
                    }
                }
                line_number++;

            }
        }
        if(print_number_of_lines)
        {
           printf("%d\n",no_lines);
        }
        fclose(file);
    }
    return 0;
}
