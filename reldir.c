#include "shell.h"

char *reldir(char *s)
{
	static char reldir[2048];
	
	if(!strncmp(s, "~", 1)) // In this context '~' is used to mean the users home directory
	{	//So this sections builds a relative pathway from the starting point of the given home directory
			int i=0, j=0;	//In place of the beginning '~'
			i+=strlen(getenv("HOME"));	//So that '/home/jdoe' can be represented as '~'
			j+=strlen(s);	//And so that '~/Documents' would be equivalent to '/home/jdoe/Documents'
		
			char end[j-1];
			for (j=0; j<strlen(end); j++) end[j]=s[j+1];
			end[j+1]='\0';
		
			strcpy(reldir, getenv("HOME"));
			strcat(reldir, end);
			
			return reldir;
	}
	else
		return s;
}
