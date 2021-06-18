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
