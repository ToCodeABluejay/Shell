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

void execCmd(command current)
/*execCmd() takes in a command
 *data-type, and executes it
 */
{
	if (!strcmp(current.argv[0], "quit"))
		exit(0);
	else if (!strcmp(current.argv[0], "cd"))
	{
		#ifdef __OpenBSD__
		pledge("stdio rpath", NULL);
		#endif
		int i;
		char *d;
		if(!current.argv[1])
		{
			d = getenv("HOME");
			i = chdir(d);
		}
		else
		{
			i = chdir(reldir(current.argv[1]));
		}
		if(i)
			fprintf(stderr, "Could not change directory to %s!\n", d);
	}
	else if (!strcmp(current.argv[0], "pwd"))
	{
		#ifdef __OpenBSD__
		pledge("stdio rpath", NULL);
		#endif
		getcwd(cwdir, sizeof(cwdir));
		printf("%s\n", cwdir);
	}
	else if (!strcmp(current.argv[0], "help"))
	{
		#ifdef __OpenBSD__
		pledge("stdio", NULL);
		#endif
		printf("Shell - MIT License\nThis shell provides the following internal functions.\n\ncd: Allows you to change the working directory\npwd: Prints the current working directory\nquit: Exits the application\nls: Prints the contents of a given directory\n\t- defaults the the current working directory\nhelp: Prints this dialogue\n");
	}
	else
	{
		int c_pid = fork();
		if (c_pid < 0)
		{
			fprintf(stderr, "Failed to create child process! Shell failure");
			exit(1);
		}
		if (!c_pid)
		{
			char *exec;
			//printf("Child PID is %i\n", getpid());
			if(current.argv[0][0]!='/'&&strncmp(current.argv[0], "./", 2))
			{
				exec = (char*) malloc(s_char * (strlen(current.argv[0])+5));
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
