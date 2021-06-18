/* "main.c" - Gabriel Bauer (@ToCodeABluejay)
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
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>

extern char *reldir(char*);

int main(int argc, char *argv[])
/*ls takes in a directory
 *path and returns a list of
 *its unhidden contents in
 *three parallel rows
 */
{
	#ifdef __OpenBSD__
	pledge("stdio rpath", NULL);
	#endif
	DIR *dir;
	struct dirent *contents;
	if(!argv[1])	//If no argument is given, assume that we are working with the curent active
		dir = opendir("."); // directory
	else
	{
		
		dir = opendir(reldir(argv[1]));
	}
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
	{
		fprintf(stderr, "Could not open directory!\n");
		return 1;
	}
	return 0;
}
