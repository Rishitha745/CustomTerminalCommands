Welcome to MyShell

This shell can execute the following command with a few options

1. ls:
        ls [option]
        This gives you information about the files in the current directory.
        It can be executed with the following options.
        1. -l : gives information in long listing format.
        2. -S : sorts the files by their Size
        3. -t : sorts according to last modification time
        4. -u : sorts according to last access time
        5. -r : sorts in reverse order
        This can also work on combination of above options like
        -lS , -Sl, -lt, -lu, -lr etc
2. cp:
        cp [option] source destination
        This command is used to copy and paste source file in destination file or copy multiple     	source files into a destination directory.
	It can be executed with the following options
	1. -v : shows files as their copied
	2. -i : prompts the users before overwriting
	3. -n : will not overwrite the destination
	4. Combinations of the above options like -iv,-vi,-in,-ni,-nv,-vn
3. mv:
	mv [option] source destination
	This command is used to move or rename a file and also to move multiple files into a 		directory.
	It can be executed with the following options
	1. -v : shows files as their moved
	2. -i : prompts the users before overwriting
	3. -n : will not overwrite the destination
	4. Combinations of the above options like -iv,-vi,-in,-ni,-nv,-vn
4.grep:
	grep [option] pattern file
	This command is used to identify a string or pattern in the file
	It can be executed with the following options.
	1. -i : case insensitive search
	2. -v : reverse search
	3. -n : prints line number along with the line
	4. -c : only the count of selected lines is displayed
	5. Combinations of the above options like -iv,-vn,-vc etc
5. ps:
	ps [options]
	It is used to know information of the processes running in the system
	1. -A : gives information of all the processes
	2. -e : same as A
	3. -T : gives SPID along with PID,TTY,CMD,TIME
	4.Combinations of the above options

Type 'exit' to exit Myshell.

To execute Myshell 
	Change the directory to where the files are located.
	Use the command "make" to compile.
	Use the command "./a.out" to execute.

Thanks for Using Myshell
