#include <stdio.h>
#include <dirent.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <grp.h>
#include <pwd.h>
#include <sys/statvfs.h>
#include <math.h>
#include <string.h>

void print_time(struct tm *time)
{
    switch (time->tm_mon+1)
    {
    case 1: printf("Jan  ");
        break;
    case 2: printf("Feb  ");
        break;
    case 3: printf("Mar  ");
        break;
    case 4: printf("Apr  ");
        break;
    case 5: printf("May  ");
        break;
    case 6: printf("Jun  ");
        break;
    case 7: printf("Jul  ");
        break;
    case 8: printf("Aug  ");
        break;
    case 9: printf("Sep  ");
        break;
    case 10: printf("Oct  ");
        break;
    case 11: printf("Nov  ");
        break;
    case 12: printf("Dec  ");
        break;
    default:
        break;
    }
    printf("%d  %02d:%02d  ",time->tm_mday,time->tm_hour,time->tm_min);
}

void print_permission(struct stat file)
{
    if(S_ISREG(file.st_mode))
    {
        printf("-");
    }
    else if (S_ISDIR(file.st_mode))
    {
        printf("d");
    }
    //User permissions
    if(file.st_mode & S_IRUSR)
    {
       printf("r");
    }
    else{
        printf("-");
    }
    if(file.st_mode & S_IWUSR)
    {
       printf("w");
    }
    else{
        printf("-");
    }
    if(file.st_mode & S_IXUSR)
    {
       printf("x");
    }
    else{
        printf("-");
    }
    //group permisssion
    if(file.st_mode & S_IRGRP)
    {
       printf("r");
    }
    else{
        printf("-");
    }
    if(file.st_mode & S_IWGRP)
    {
       printf("w");
    }
    else{
        printf("-");
    }
    if(file.st_mode & S_IXGRP)
    {
       printf("x");
    }
    else{
        printf("-");
    }
    //others permission
    if(file.st_mode & S_IROTH)
    {
       printf("r");
    }
    else{
        printf("-");
    }
    if(file.st_mode & S_IWOTH)
    {
       printf("w");
    }
    else{
        printf("-");
    }
    if(file.st_mode & S_IXOTH)
    {
       printf("x");
    }
    else{
        printf("-");
    }
}

struct file {
    struct stat file_properties;
    char filename[100];
};

int compare_file_name(const void *a, const void *b)
{
    const struct file *file1 = (const struct file *)a;
    const struct file *file2 = (const struct file *)b;

    return strcmp(file1->filename, file2->filename);
}

int compare_size(const void *a, const void *b)
{
    const struct file *file1 = (const struct file *)a;
    const struct file *file2 = (const struct file *)b;

    return (int)(file2->file_properties.st_size - file1->file_properties.st_size);
}

int compare_access_time(const void *a, const void *b)
{
    const struct file *file1 = (const struct file *)a;
    const struct file *file2 = (const struct file *)b;

    return (int)(file2->file_properties.st_atime - file1->file_properties.st_atime);
}

int compare_modification_time(const void *a, const void *b)
{
    const struct file *file1 = (const struct file *)a;
    const struct file *file2 = (const struct file *)b;

    return (int)(file2->file_properties.st_mtime - file1->file_properties.st_mtime);
}

int ls_function(int long_listing,int reverse_order,int sort_access_time,int sort_modificaton_time,int sort_size,int only_ls,int index_sort_access_time,int index_sort_modification_time,int index_sort_size)
{
    struct dirent *dir1,*dir2;
    int max_bytes=0;
    int no_digits=0;
    int i=0;
    int file_number =0;
    DIR*dr1 = opendir(".");
    if(dr1==NULL){
        printf("Couldn't not open current directory");
        return 0;
    }
    while((dir1=readdir(dr1))!=NULL ){
        if( dir1->d_name[0]!='.'){
        i++;
        }
    }
    closedir(dr1);
    struct file file_prop[i];
    //struct stat file_prop[i];
    char filename[i][100];
    //char filename2[i][100];
    struct passwd *usr[i];
    struct group *grp[i];

    DIR*dr2 = opendir(".");
    if(dr2==NULL){
        printf("Couldn't not open current directory");
        return 0;
    }
    while((dir2=readdir(dr2))!=NULL  ){
        if( dir2->d_name[0]!='.'){
        strcpy(file_prop[file_number].filename,dir2->d_name);
        sprintf(filename[file_number],"./%s",dir2->d_name);
        stat(filename[file_number],&file_prop[file_number].file_properties);
        if(max_bytes<file_prop[file_number].file_properties.st_size)
        {
            max_bytes=file_prop[file_number].file_properties.st_size;
        }
        file_number++;
        }
    }
    closedir(dr2);
    int max_no_digits=(int)log10(max_bytes)+1;
    qsort(file_prop, i, sizeof(struct file), compare_file_name);
    if (sort_size==1 && sort_modificaton_time==1)
    {
        if(index_sort_size>index_sort_modification_time)
        {
            qsort(file_prop, i, sizeof(struct file), compare_size);
        }
        else if(index_sort_modification_time>index_sort_size)
        {
            qsort(file_prop, i, sizeof(struct file), compare_modification_time);
        }
    }
    else if (sort_modificaton_time==1)
    {
        qsort(file_prop, i, sizeof(struct file), compare_modification_time);
    }
    else if(sort_size==1)
    {
        qsort(file_prop, i, sizeof(struct file), compare_size);
    }
    else if(sort_access_time==1)
    {
        qsort(file_prop, i, sizeof(struct file), compare_access_time);
    }
    if(long_listing==1)
    {
        if(reverse_order==1)
        {
            for(file_number=i-1;file_number>=0;file_number--)
            {
                print_permission(file_prop[file_number].file_properties);
                printf(" %d  ",file_prop[file_number].file_properties.st_nlink);
                usr[file_number] = getpwuid(file_prop[file_number].file_properties.st_uid);
                grp[file_number] = getgrgid(file_prop[file_number].file_properties.st_gid);
                printf("%s  ",usr[file_number]->pw_name);
                printf("%s  ",grp[file_number]->gr_name);
                if(file_prop[file_number].file_properties.st_size==0)
                {
                    for(int index=0;index<=max_no_digits-1;index++)
                    {
                        printf(" ");
                    }
                } 
                else
                {
                    no_digits=(int)log10(file_prop[file_number].file_properties.st_size)+1;
                    for(int index=0;index<=max_no_digits-no_digits;index++)
                    {
                        printf(" ");
                    }
                }
                
                printf(" %lld ",file_prop[file_number].file_properties.st_size);
                if(sort_access_time==0)
                {
                    struct tm *modification_time = localtime(&file_prop[file_number].file_properties.st_mtime);
                    print_time(modification_time);
                }
                else if (sort_access_time==1 && sort_modificaton_time==0)
                {
                    struct tm *access_time = localtime(&file_prop[file_number].file_properties.st_atime);
                    print_time(access_time);
                }
                printf("%s    ",file_prop[file_number].filename);
                printf("\n");
            }
        }
        else 
        {
            for(file_number=0;file_number<i;file_number++)
            {
                print_permission(file_prop[file_number].file_properties);
                printf(" %d  ",file_prop[file_number].file_properties.st_nlink);
                usr[file_number] = getpwuid(file_prop[file_number].file_properties.st_uid);
                grp[file_number] = getgrgid(file_prop[file_number].file_properties.st_gid);
                printf("%s  ",usr[file_number]->pw_name);
                printf("%s  ",grp[file_number]->gr_name);
                no_digits=(int)log10(file_prop[file_number].file_properties.st_size)+1;
                for(int index=0;index<=max_no_digits-no_digits;index++)
                {
                    printf(" ");
                }
                printf(" %lld ",file_prop[file_number].file_properties.st_size);
                if(sort_access_time==0)
                {
                    struct tm *modification_time = localtime(&file_prop[file_number].file_properties.st_mtime);
                    print_time(modification_time);
                }
                else if (sort_access_time==1 && sort_modificaton_time==0)
                {
                    struct tm *access_time = localtime(&file_prop[file_number].file_properties.st_atime);
                    print_time(access_time);
                }
                printf("%s    ",file_prop[file_number].filename);
                printf("\n");
            }
        }
    }
    else if (long_listing==0)
    {
        for(file_number=0;file_number<i;file_number++)
        {
           printf("%s\t",file_prop[file_number].filename); 
        }
        printf("\n");
    }
    return 0;
}