/* "dir.c" - Gabriel Bauer (@ToCodeABluejay)
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

/*char *absdir(char *s)
//TODO: Get this function to be able to be a viable alternative to realpath() w/support for path's relative to homedir
{
	static char absdir[PATH_MAX];
	
	if(!strncmp(s, "~", 1)) // In this context '~' is used to mean the users home directory
	{	//So this sections builds a relative pathway from the starting point of the given home directory
			int i=0;	//In place of the beginning '~'
			i+=strlen(s);	//And so that '~/Documents' would be equivalent to '/home/jdoe/Documents'
		
			char end[i];
			for (i=0; i<strlen(end); i++) end[i]=s[i+1];
			end[i+1]='\0';
		
			strcpy(absdir, getenv("HOME"));
			strcat(absdir, end);
			
			return absdir;
	}
	else
		return s;
}*/

#ifndef __linux__
char *get_current_dir_name(void)	//For all other UNIX-like platforms, we can just roll our own. It's fairly easy...
{
	static char cwdir[PATH_MAX];
	getcwd(cwdir, sizeof(cwdir));
}
#endif

void cd(struct command *k)
{
	int i;
	char d[PATH_MAX];
	printf(k->argv[1]);
	if(!k->argv[1])
	{
		strcpy(d, getenv("HOME"));
		i = chdir(d);
	}
	else
	{
		char *p = realpath(k->argv[1], NULL);
		if(!p)
		{
			fprintf(stderr, "Could change directory to [%s]!\n", k->argv[1]);
		}
		else
		{
			i = chdir(p);
		}
		free(p);
	}
	if (i)
		fprintf(stderr, "Could not change directory to %s!\n", k->argv[1]);
}

void ls(struct command *k)
{
	DIR *dir;
	struct dirent *contents;
	char path[PATH_MAX];
	/*if(!strlen(k->argv[1]))	//If no argument is given, assume that we are working with the curent active
		dir = opendir("."); // directory
	else*/
		dir = opendir(realpath(k->argv[1], path));	//We can uncomment the above if-else statement when
	if (dir)	// we can replace realpath() w/absdir()
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

