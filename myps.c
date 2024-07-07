#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <string.h>
#include <pwd.h>
#include <grp.h>
#include <sys/stat.h>
#include <ctype.h>
#include <unistd.h>
#include <fcntl.h>
#include <time.h>

#define MAX_PATH_LEN 1024

int ps_command(int only_ps,int ps_T,int full)
{
   char *my_tty = ttyname(STDIN_FILENO);
   char my_tty_copy[256];
   strcpy(my_tty_copy,my_tty);
   char cmd_file[1024];
   char cmd_line[1000];
   DIR*dir;
   struct dirent *direc;
   dir=opendir("/proc");
   FILE *file;
   pid_t pid;
   if(only_ps!=0 || ps_T==0)
   {
      printf("%4s\t%s \t %5s \t%s\n","PID","TTY","TIME","CMD");
   }

   else
   {
      printf("%4s\t %4s\t %s\t %5s\t   %s\n","PID","SPID","TTY","TIME","CMD");
   }
   while((direc=readdir(dir))!=NULL)
   {
       if(direc->d_type != DT_DIR || !isdigit(*direc->d_name))
       { continue;
       }

       //To get PID
       pid_t pid=atoi(direc->d_name);       
              
       // To get SPID
       pid_t spid=getsid(pid);
        
        //To get PPID
        char ppid_path[1024];
        sprintf(ppid_path,"/proc/%d/stat",pid);   
        FILE *file_ppid=fopen(ppid_path,"r");
         if(file_ppid==NULL)
         {
            printf("file cannot be opened for ppid\n");
            continue;
         }
           pid_t ppid;
           fscanf(file_ppid,"%*d %*s %*c %d",&ppid);
           fclose(file_ppid);
       
       //To get cmd 
       sprintf(cmd_file, "/proc/%s/cmdline", direc->d_name);
       
       FILE *file_cmd = fopen(cmd_file, "r");
       if (file_cmd == NULL) 
      {
        perror("Failed to open cmdline file");
        exit(EXIT_FAILURE);
      }
      fgets(cmd_line, sizeof(cmd_line), file_cmd);
    
      char *cmd_name = strrchr(cmd_line, '/');
      if (cmd_name == NULL) {
         cmd_name = cmd_line;
      } else {
         cmd_name++;
      }
      fclose(file_cmd);
            // To get time
      time_t t = time(NULL);
      struct tm tm = *localtime(&t);
      char tz[32];
      strftime(tz, sizeof(tz), "%Z", &tm);
      setenv("TZ", tz, 1);
         
      FILE *file_time=fopen(ppid_path,"r");
      if (file_cmd == NULL) 
      {
        perror("Failed to open timeline file");
        exit(EXIT_FAILURE);
      }
      char buf[1024];
      fgets(buf, sizeof(buf), file_time);

      time_t utime, stime;
      sscanf(buf, "%*d %*s %*c %*d %*d %*d %*d %*d %*u %*u %*u %*u %lu %lu", &utime, &stime);
      time_t cpu_time_us = (time_t)utime + (time_t)stime;
      time_t cpu_time = (time_t)(cpu_time_us)/sysconf(_SC_CLK_TCK) ;
      time_t time_hour,time_minute,time_second;
      time_second=cpu_time%60;
      time_minute=(cpu_time/60)%60;
      time_hour=(cpu_time/3600)%3600;

      //To get tty
      char path[1024];
      snprintf(path,MAX_PATH_LEN,"/proc/%s/fd/0",direc->d_name);
      char *tty = ttyname(open(path,O_RDONLY));
       if(tty == NULL)
       {
            tty = "??";
            
            if(full)
            {
               printf("%s\t",direc->d_name);
               if(ps_T)
               {
                  printf("%d\t",spid);
               }
               printf("%s\t",tty);
               printf("%02ld:%02ld:%02ld \t",time_hour,time_minute,time_second);
         		printf("%s\n",cmd_name);
            }
        }
        else if(strcmp(tty,my_tty_copy)==0)
        {
      		printf("%s\t",direc->d_name);
      		if(ps_T)
            {
               printf("%d\t",spid); 
            }
      		printf("%s\t",tty+5);
      		printf("%02ld:%02ld:%02ld \t",time_hour,time_minute,time_second);
      		printf("%s\n",cmd_name);
		  }
   }

}       
