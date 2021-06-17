/* "command.c" - Gabriel Bauer (@ToCodeABluejay)
 * 
 *Copyright (c) 2021 Gabriel Bauer (@ToCodeABluejay)
 *
 *Permission is hereby granted, free of charge, to any person obtaining a copy
 *of this software and associated documentation files (the "Software"), to deal
 *in the Software without restriction, including without limitation the rights
 *to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 *copies of the Software, and to permit persons to whom the Software is
 *furnished to do so, subject to the following conditions:
 *
 *The above copyright notice and this permission notice shall be included in all
 *copies or substantial portions of the Software.
 *
 *THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 *IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 *FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 *AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 *LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 *OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 *SOFTWARE.
 */
#include "shell.h"

void ls(char **args)
/*ls() takes in a directory
 *path and returns a list of
 *its unhidden contents in
 *three parallel rows
 */
{
	DIR *dir;
	struct dirent *contents;
	if(!args[1])	//If no argument is given, assume that we are working with the curent active
		dir = opendir("."); // directory
	else if(!strncmp(args[1], "~", 1)) // In this context '~' is used to mean the users home directory
	{	//So this sections builds a relative pathway from the starting point of the given home directory
		int i=0, j=0;	//In place of the beginning '~'
		i+=len(getenv("HOME"));	//So that '/home/jdoe' can be represented as '~'
		j+=len(args[1]);	//And so that '~/Documents' would be equivalent to '/home/jdoe/Documents'
		
		char reldir[i+j];
		
		strcpy(reldir, getenv("HOME"));
		for(i, j=1;j<=len(args[1]);i++,j++) reldir[i]=args[1][j];
		
		dir = opendir(reldir);
	}
	else
		dir = opendir(args[1]);
	if (dir)
	{
		int i=0;	//This is a tracker to keep a tab on how many items have been printed
		while ((contents = readdir(dir)))
		{
			if(strncmp(contents->d_name, ".",1))
			{
				if((!(i%3))&&i)	//This is to set a max to the amount of items printed on a line to three
					putchar('\n');	//Sometimes names can get long, and three might still be tough when they get too long
				printf("%30s  ", contents->d_name);	//But I though this would be nice to keep everything
				i++;	//From getting too crowded
			}
		}
		closedir(dir);
		putchar('\n');
	}
	else
		fprintf(stderr, "Could not open directory!\n");
}

char *getNxtScrpt()
/*getNxtScrpt() is almost line a scanf()
 *or other input function built for this
 *specific application in mind
 *
 *it reads user input character by character
 *and supports multiline command inputs
 *by using the '\' character at the end of
 *a line of input
 */
{
	int inchar, prevchar, len = 1;
	bool multiline = false;
	char *cmd;
	cmd = (char *)malloc(s_char * len);
	
	while (true)
	{
		switch (inchar = getchar())
		{
			case '\b' :
				len--;
				cmd = (char *)realloc(cmd, s_char * len);
				break;
			case '\\' :
				multiline = true;
				if (prevchar == '\\')
					multiline = false;
				prevchar = inchar;
				break;
			case ' ' :
				cmd[len-1] = (char) inchar;
				len++;
				cmd = (char *)realloc(cmd, s_char * len);
				prevchar = inchar;
				break;
			case '\n' :
				if (multiline)
				{
					printf("> ");
					multiline = false;
				}
				else
				{
					len++;
					cmd = (char *)realloc(cmd, s_char * len);
					cmd[len-1] = (char) inchar;
					
					return cmd;
				}
				prevchar = inchar;
				break;
			default :
				multiline = false;
				cmd[len-1] = (char) inchar;
				len++;
				cmd = (char *)realloc(cmd, s_char * len);
				prevchar = inchar;
				break;
		}
	}
}

command getCommands()
/*getCommands() is a very simple command
 *which calls getNxtScrpt() in order to
 *get the next string of commands to parse,
 *and sends it into wordify() in order to
 *split it up into its various components,
 *builds a data-type command, sets the argv
 *data-section equal to the output of the
 *previous part and the argc section equal
 *to the number of arguments in argv, and
 *then finally send that command back to
 *get executed by execCmd()
 */
{
	command commands;
	
	commands.argv = &(*wordify(getNxtScrpt()));
	for(commands.argc=0; (void*)commands.argv[commands.argc]; commands.argc++);
	
	return commands;
}

int ShellCommands(char **args)
/*ShellCommands() determines whether
 *the command returned to execCmd()
 *from getCommands() is an internal
 *command of the shell, or another
 *application. If it is internal, it
 *then runs the corresponding command
 */
{
	if (!strcmp(args[0], "cd"))
	{
		int i;
		char *d;
		if(!args[1])
		{
			d = getenv("HOME");
			i = chdir(d);
		}
		else
		{
			if(!strcmp(args[1], "~"))
				d = getenv("HOME");
			else
				d = args[1];
			i = chdir(d);
		}
		if(i)
			printf("Could not change directory to %s!\n", d);
	}
	else if (!strcmp(args[0], "pwd"))
	{
		getcwd(cwdir, sizeof(cwdir));
		printf("%s\n", cwdir);
	}
	else if (!strcmp(args[0], "quit"))
		exit(0);
	else if (!strcmp(args[0], "ls"))
		ls(args);
	else if (!strcmp(args[0], "help"))
		printf("Shell - Final Project Submission\nThis shell provides the following internal functions.\n\ncd: Allows you to change the working directory\npwd: Prints the current working directory\nquit: Exits the application\nls: Prints the contents of a given directory\n\t- defaults the the current working directory\nhelp: Prints this dialogue\n");
	else
		return false;
	return true;
}

void execCmd(command current)
/*execCmd() takes in a command
 *data-type, and executes it
 */
{
	if (!ShellCommands(current.argv))
	{
		int c_pid = fork();
		if (c_pid < 0)
		{
			printf("Failed to create child process! Shell failure");
			exit(1);
		}
		if (!c_pid)
		{
			char *exec;
			//printf("Child PID is %i\n", getpid());
			if(current.argv[0][0]!='/'&&strncmp(current.argv[0], "./", 2))
			{
				exec = (char*) malloc(s_char * (len(current.argv[0])+5));
				if (!isnotsu)
					strcat(exec, "/sbin/");
				else
					strcat(exec, "/bin/");
				strcat(exec, current.argv[0]);
				current.argv[0] = exec;
			}
			else
				exec = current.argv[0];
			if(!execve(exec, current.argv, environ));
			else
				printf("Execution failure! Error: %i\n", errno);
			exit(0);
		}
		else
			wait(&c_pid);
	}
}
