/* "shell.h" - Gabriel Bauer (@ToCodeABluejay)
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
#ifndef SHELL_H
#define SHELL_H
#define _GNU_SOURCE
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <errno.h>
#include <dirent.h>

extern char **environ;
extern int errno;

extern const size_t s_char;
extern FILE *input;

extern bool isnotsu; // Tracks super-user status
extern char cwdir[2048];

/* Typedef defines a new datatype called 'command' which is composed of a 'struct',
 * in other words, a 'structure', which is composed of an integer
 * 'argc' which keeps track of how many arguments are passed to the command, and an
 * array of character arrays 'argv' which holds the arguments that are to be passed
 * to the command;
 */

typedef struct
{
	int argc;
	char **argv;
} command;

/*Here are all of the functions that are required in more than one file*/
long long len(char*);
char **wordify(char*);
command getCommands();
void execCmd(command current);

#endif
