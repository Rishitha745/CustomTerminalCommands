#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <ctype.h>


int ls_function(int long_listing,int reverse_order,int sort_access_time,int sort_modificaton_time,int sort_size,int only_ls,int index_sort_access_time,int index_sort_modification_time,int index_sort_size);
int cp_command(char* source_input,char* destination_input,int not_overwrite,int ask_overwrite, int print_transfer,int number_source_files);
int mv_command(char* source_input,char* destination_input,int not_overwrite,int ask_overwrite, int print_transfer,int number_source_files);
int grep_command(char* string,char* file_name,int print_line_number, int print_number_of_lines,int case_insensitive,int reverse);
int ps_command(int only_ps,int full,int ps_T);
char* get_prompt()
{

   char* prompt=malloc(100);
   char hostname[100];
   char* username = getlogin();
   char cwd[1024];
   getcwd(cwd,1024);
   gethostname(hostname,1024);
   strcpy(prompt,username);
   strcat(prompt,"@");
   strcat(prompt,hostname);
   strcat(prompt,":");
   strcat(prompt,cwd);
   strcat(prompt," $");
   return prompt;
}


int main(int argc,char *argv[])
{
	char *command;
	int check;
	char string1[50]="exit";
	char string2[50]="help";
	char prompt[100];
    char store_command[1024];
    

	while(1){
		command = readline(get_prompt());
		if(strlen(command)!=0)
        {
			int i;
			for(i=0;command[i];i++){
				if(!isspace(command[i])){
					break;
				}
			}
			if(command[i]){
		    add_history(command);
			}
        }
        strcpy(store_command,command);

        char *token = strtok(command," ");
        if(token != NULL)
        {
            if (strcmp(token,"ls")==0)
            {
                int reverse_order=0;
                int long_listing=0;
                int only_ls=0;
                int sort_size=0,index_sort_size=0;
                int sort_access_time=0,index_sort_access_time=0;
                int sort_modificaton_time=0,index_sort_modification_time=0;
                only_ls=1;
                while(token!=NULL)
                {
                    token =strtok(NULL," ");
                    if(token != NULL && token[0] == '-')
                    {
                        for (int i =1;token[i]!= '\0';i++)
                        {
                            if (token[i]=='r')
                            {
                                reverse_order=1;
                                
                            }
                            else if (token[i]=='l')
                            {
                                long_listing=1;
                            }
                            else if (token[i]=='S')
                            {
                                sort_size=1;
                                index_sort_size=i;
                            }
                            else if (token[i]=='t')
                            {
                                sort_modificaton_time=1;
                                index_sort_modification_time=i;
                            }
                            else if(token[i]=='u')
                            {
                                sort_access_time=1;
                                index_sort_access_time=i;
                            }
                        }
                    }
                }
                ls_function(long_listing,reverse_order,sort_access_time,sort_modificaton_time,sort_size,only_ls,index_sort_access_time,index_sort_modification_time,index_sort_size);
            }
            else if (strcmp(token,"cp")==0)
            {
                int ask_overwrite=0;
                int not_overwrite=0;
                int print_transfer=0;
                int index_ask_overwrite=0;
                int index_not_overwrite=0;
                int token_count=0;
                char store_tokens[100][100];
                int check=0;
                int number_source_files=0;
                while(token!=NULL)
                {
                    token =strtok(NULL," ");
                    token_count++;
                    if(token!=NULL)
                    {
                        strcpy(store_tokens[token_count],token);
                    }
                    if(token != NULL && token[0] == '-')
                    {
                        for (int i =1;token[i]!= '\0';i++)
                        {
                            if (token[i]=='v')
                            {
                                print_transfer++;
                                
                            }
                            else if (token[i]=='i')
                            {
                                ask_overwrite++;
                                index_ask_overwrite=i;
                            }
                            else if (token[i]=='n')
                            {
                                not_overwrite++;
                                index_not_overwrite=i;
                            }
                            else
                            {
                            printf("No such file or directory\n");
                            continue;
                            }
                        }
                        check++;                        
                    }
                }
                if(index_ask_overwrite>index_not_overwrite)
                {
                    not_overwrite=0;
                }
                else if(index_ask_overwrite<index_not_overwrite)
                {
                    ask_overwrite=0;
                }
                char destination[100];
                strcpy(destination,store_tokens[token_count-1]);
                if(check)
                {
                    if(token_count<=3)
                    {
                        printf("usage: cp [-R [-H | -L | -P]] [-fi | -n] [-aclpsvXx] source_file target_file\n");
                        printf("       cp [-R [-H | -L | -P]] [-fi | -n] [-aclpsvXx] source_file ... target_directory\n");
                    }
                    else
                    {
                        char source[token_count-3][100];
                        number_source_files=token_count-3;
                        for(int index=0;index<token_count-3;index++)
                        {
                            strcpy(source[index],store_tokens[index+2]);
                            if(cp_command(source[index],destination, not_overwrite, ask_overwrite,print_transfer,number_source_files)==0)
                            {
                                break;
                            }
                        }
                    }
                    
                }
                else if (check==0)
                {
                    if(token_count<=2)
                    {
                        printf("usage: cp [-R [-H | -L | -P]] [-fi | -n] [-aclpsvXx] source_file target_file\n");
                        printf("       cp [-R [-H | -L | -P]] [-fi | -n] [-aclpsvXx] source_file ... target_directory\n");
                    }
                    else
                    {
                        char source[token_count-2][100];
                        number_source_files=token_count-2;
                        for(int index=0;index<token_count-2;index++)
                        {
                            strcpy(source[index],store_tokens[index+1]);
                            if(cp_command(source[index],destination, not_overwrite, ask_overwrite,print_transfer,number_source_files)==0)
                            {
                                break;
                            }
                        } 
                    }                   
                }                          
            
            }
            else if (strcmp(token,"mv")==0)
            {
                int ask_overwrite=0;
                int not_overwrite=0;
                int print_transfer=0;
                int index_ask_overwrite=0;
                int index_not_overwrite=0;
                int token_count=0;
                char store_tokens[100][100];
                int check=0;
                int number_source_files=0;
                while(token!=NULL)
                {
                    token =strtok(NULL," ");
                    token_count++;
                    if(token!=NULL)
                    {
                        strcpy(store_tokens[token_count],token);
                    }
                    if(token != NULL && token[0] == '-')
                    {
                        for (int i =1;token[i]!= '\0';i++)
                        {
                            if (token[i]=='v')
                            {
                                print_transfer++;
                                
                            }
                            else if (token[i]=='i')
                            {
                                ask_overwrite++;
                                index_ask_overwrite=i;
                            }
                            else if (token[i]=='n')
                            {
                                not_overwrite++;
                                index_not_overwrite=i;
                            }
                            else
                            {
                            printf("No such file or directory\n");
                            continue;
                            }
                        }
                        check++;
                        
                    }
                }
                if(index_ask_overwrite>index_not_overwrite)
                {
                    not_overwrite=0;
                }
                else if(index_ask_overwrite<index_not_overwrite)
                {
                    ask_overwrite=0;
                }
                char destination[100];
                strcpy(destination,store_tokens[token_count-1]);
                if(check)
                {
                    if(token_count<=3)
                    {
                        printf("usage: mv [-f | -i | -n] [-hv] source target\n");
                        printf("       mv [-f | -i | -n] [-v] source ... directory\n");
                    }
                    else
                    {
                        char source[token_count-3][100];
                        number_source_files=token_count-3;
                        for(int index=0;index<token_count-3;index++)
                        {
                            strcpy(source[index],store_tokens[index+2]);                           
                            if(mv_command(source[index],destination, not_overwrite, ask_overwrite,print_transfer,number_source_files)==0)
                            {
                                break;
                            }
                        }
                    }
                    
                }
                else if (check==0)
                {
                    if(token_count<=2)
                    {
                        printf("usage: mv [-f | -i | -n] [-hv] source target\n");
                        printf("       mv [-f | -i | -n] [-v] source ... directory\n");
                    }
                    else
                    {
                        char source[token_count-2][100];
                        number_source_files=token_count-2;
                        for(int index=0;index<token_count-2;index++)
                        {
                            strcpy(source[index],store_tokens[index+1]);
                            if(mv_command(source[index],destination, not_overwrite, ask_overwrite,print_transfer,number_source_files)==0)
                            {
                                break;
                            }
                        } 
                    }                   
                }                           
            
            }
            else if (strcmp(token,"grep")==0)
            { 
                int print_line_number=0;
                int print_number_of_lines=0;
                int reverse=0;
                int case_insensitive=0;
                int token_count=0;
                int no_words_string=0;
                char store_tokens[100][100];
                int check=0;
                int check_quotes=0;

                char* location_1=strchr(store_command,'"');
                char* location_2;
                if(location_1!=NULL)
                {
                    check_quotes++;
                    location_2=strchr(location_1+1,'"');
                    if(location_1!=NULL && location_2!= NULL)
                    {
                        check_quotes++;
                    }
                }
                
                if(check_quotes==0 )
                {
                    while(token!=NULL)
                    {
                        token =strtok(NULL," ");
                        token_count++;
                        if(token!=NULL)
                        {
                            strcpy(store_tokens[token_count],token);
                        }
                        if(token != NULL && token[0] == '-')
                        {
                            for (int i =1;token[i]!= '\0';i++)
                            {
                                if (token[i]=='n')
                                {
                                    print_line_number++;
                                    
                                }
                                else if (token[i]=='c')
                                {
                                    print_number_of_lines++;
                                }
                                else if (token[i]=='i')
                                {
                                    case_insensitive++;
                                }
                                else if (token[i]=='v')
                                {
                                    reverse++;
                                }
                                else
                                {
                                printf("No such file or directory\n");
                                }
                            }
                            check++;                        
                        }
                    }

                    char string[256];
                    if(check)
                    {
                        strcpy(string,store_tokens[2]);
                        for(int  index =0;index<token_count-3;index++)
                        {
                            grep_command(string,store_tokens[index+3],print_line_number,print_number_of_lines,case_insensitive,reverse); 
                        }
                    }
                    else if(check==0)
                    {
                        strcpy(string,store_tokens[1]);
                        for(int index=0;index<token_count-2;index++)
                        {
                            grep_command(string,store_tokens[index+2],print_line_number,print_number_of_lines,case_insensitive,reverse);  
                        }
                    }
                }
                else if(check_quotes ==1)
                {
                    printf("usage: grep [options] pattern [file...]\n");
                }

                else
                {
                    token =strtok(NULL," ");
                    if(token != NULL && token[0] == '-')
                    {
                        for (int i =1;token[i]!= '\0';i++)
                        {
                            if (token[i]=='n')
                            {
                                print_line_number++;
                                
                            }
                            else if (token[i]=='c')
                            {
                                print_number_of_lines++;
                            }
                            else if (token[i]=='i')
                            {
                                case_insensitive++;
                            }
                            else if (token[i]=='v')
                            {
                                reverse++;
                            }
                            else
                            {
                            printf("No such file or directory\n");
                            continue;
                            }
                        }
                        check++;
                    }
                    int size_string=location_2-location_1;
                    char string[size_string];
                    memcpy(string,location_1+1,size_string-1);
                    string[size_string-1]='\0';
                     
                    int file_token_size=strlen(store_command)-strlen(location_2);
                    char file_token[file_token_size];
                    memcpy(file_token,location_2+1,file_token_size-1);
                    file_token[file_token_size-1]='\0';
                    
                    char*file_parse=strtok(file_token," ");
                    while(file_parse!=NULL)
                    {
                        strcpy(store_tokens[token_count],file_parse);
                        grep_command(string,file_parse,print_line_number,print_number_of_lines,case_insensitive,reverse);
                        file_parse =strtok(NULL," ");
                        token_count++;
                    }
                }
            }
            else if(strcmp(token,"ps")==0)
            {
                int only_ps=0;
                int full=0;
                int ps_T=0;
                while(token!=NULL)
                {
                    token =strtok(NULL," ");
                    if(token != NULL && token[0] == '-')
                    {
                        for (int i =1;token[i]!= '\0';i++)
                        {
                            if (token[i]=='T')
                            {
                                ps_T++;                                
                            }
                            else if(token[i]=='A'|| token[i]== 'e')
                            {
                                full++;
                            }
                        }
                    }
                    else
                    {
                        only_ps++;
                    }
                }
                ps_command(only_ps,full,ps_T);
            }
            else if(strcmp(token,"exit")==0)
            {
                printf("You are exiting Myshell\n");
	            exit(0);
            }
            else if(strcmp(token,"help")==0)
            {
                printf("help : Displays this page\n");
                printf("exit : To exit myshell\n");
            }
            else
            { 
                printf("Invalid Command \n");
                printf("Refer to readme.txt to know about this shell\n");

            }
        }
    }
    free(command);
	free(get_prompt());
    return 0;
}
