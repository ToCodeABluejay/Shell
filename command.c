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

/*iswhitespace() takes in a character, k
 *and returns true if it is either a space
 *or a tab, and false otherwise
 */
bool iswhitespace(char k){return(k==' '||k=='\t'||k=='\n')?true:false;}

char *getArgv(struct command *k)
{
	static char argv[16384] = {0};
	int i;
	
	for(i=0;strlen(k->argv[i]); i++)
	{
		strcat(argv, k->argv[i]);
		strcat(argv, " ");
	}
	return argv;
}

void getCommands(struct command *k)
/*getCommands() is a very simple command
 *which gets the next string of commands
 *to parse and splits it up into its various
 *components using strtok(), and
 *builds a data-type command, sets the argv
 *data-section equal to the output of the
 *previous parts and the argc section equal
 *to the number of arguments in argv, and
 *then finally sends that command back to
 *get executed by execCmd()
 */
{
	char cmdl[16384];
	fgets(cmdl, 16384, stdin);
	
	long long i=0;
	
	/*Just a quick preliminary measure to make sure
	 *that on each run, we start on a clean slate
	 */
	for(i; i<64; i++) strcpy(k->argv[i], "\0");
	
	const char s[3] = " \n";
	char *tok;

	// Get and copy the first arg
	tok = strtok(cmdl, s);
	strcpy(k->argv[i], tok);

	// Continue to get and copy args until end
	while (true)
	{
		i++;
		tok = strtok(NULL, s);
		
		if (tok)
			strcpy(k->argv[i], tok);
		else
			break;
	}
}

bool ShellCommand(struct command *k)
{
	if (!strcmp(k->argv[0], "quit"))
	{
		exit(0);
		return true;
	}
	else if (!strcmp(k->argv[0], "pwd"))
	{
		printf("%s\n", get_current_dir_name());
		return true;
	}
	else if (!strcmp(k->argv[0], "cd"))
	{
		cd(k);
		return true;
	}
	else if (!strcmp(k->argv[0], "ls"))
	{
		ls(k);
		return true;
	}
	else if (!strcmp(k->argv[0], "help"))
	{
		printf("Shell - MIT License\nThis shell provides the following internal functions.\n\ncd: Allows you to change the working directory\npwd: Prints the current working directory\nquit: Exits the application\nls: Prints the contents of a given directory\n\t- defaults the the current working directory\nhelp: Prints this dialogue\n");
		return true;
	}
	else
		return false;
}

void execCmd(struct command *k)
/*execCmd() takes in a command
 *data-type, and executes it
 */
{
	printf(k->argv[0]);
	if(!ShellCommand(k))
	{
		int c_pid = fork();
		if (c_pid < 0)
		{
			fprintf(stderr, "Failed to create child process! Shell failure");
			exit(1);
		}
		if (!c_pid)
		{
			char exec[768] = "\0";
			//printf("Child PID is %i\n", getpid());
			if(k->argv[0][0]!='/'&&strncmp(k->argv[0], "./", 2))
			{
				if (!isnotsu)
					strcpy(exec, "/sbin/");
				else
					strcpy(exec, "/bin/");
				strcat(exec, k->argv[0]);
			}
			else
				strcpy(exec, k->argv[0]);
			//printf ("[%s]\n", exec);	//Just another...useful debugger! ;)
			if(!execle(exec, getArgv(k), NULL, environ));
			else
				fprintf(stderr, "Execution failure! Error: %i\n", errno);
			exit(0);
		}
		else
			wait(&c_pid);
	}
}

