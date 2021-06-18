/* "string.c" - Gabriel Bauer (@ToCodeABluejay)
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

//This will store our current working directory
char cwdir[2048];

/*Declare a constant equal to the size of a character
 *data type. This will be visible to the entire program
 */
const size_t s_char = sizeof(char);

/* *cargv[] will technichally be a part of the code, but
 * it will not be included in "shell.h", so should only
 *be visible and utilized by code inside of this C file
 */


/*All functions declared here are open to use by any
 *file that is a part of the project, as they will all
 *be included in "shell.h"
 */

long long len(char *s)
/* len(char *s) is a very simple function which takes
 *in an array of characters and returns the length of
 *the array as a long long
 */
{
	long long i;
	for(i=0;*(s+i);i++);
	return i;
}

void empty(char *s, long long n)
/*Takes in two parameters, a character array char *s
 *and a long long, n, and will, starting from the
 *position passed to the function by n, will
 *empty the remaining portion of the string
 */
{
	long long i;
	if (n<strlen(s))
		for(i=n; s[i]; i++) s[i] = '\0';
}

bool iswhitespace(char k){return(k==' '||k=='\t')?true:false;}
/*iswhitespace() takes in a character, k
 *and returns true if it is either a space
 *or a tab, and false otherwise
 */

char **wordify(char *s)
/* wordify() is a  char **  function, that is to say
 *it is a function which returns an array of character
 *arrays.  It returns the an array of words seperated
 *by spaces and tabs
 */
{
	static char *cargv[] = {0};
	long long i=0, j=0, k=0;
	char *word = (char *)malloc(s_char * (k+1));
	
	/*Just a quick preliminary measure to make sure
	 *that on each run, we start on a clean slate
	 */
	for(i=0;cargv[i];i++)
		cargv[i] = NULL;
	
	for (i=0; iswhitespace(s[i]); i++);
	while (true)
	{
		if (iswhitespace(s[i]) || s[i] == '\0')
		{
			if (i>0 && !iswhitespace(s[i-1]) || s[i] == '\0')
			{
				*(cargv+j) = realloc(*(cargv+j), s_char * (k+1));
				strcpy(*(cargv+j), word);
				
				//printf("Position of words[j]: %d\n", &cargv[j]);		//Debug statements
				//printf("Length of words[j]: %i\n", len(cargv[j]));	//Can be
				//printf("Contents of words[j]: %s\n", cargv[j]);		//Very useful ;)
				
				/*k represents the number of characters in the current working word
				 *j represents the index of the current working word in the array
				 *i represents the current index position in the original string
				 */
				k=0, j++, i++;
				if (i >= strlen(s))
				{
					return cargv;
				}
				else
					word = realloc(word, s_char * k);
			}
			else
				i++;
		}
		else
		{
			word = realloc(word, s_char * (k+1));
			empty(word, k);
			word[k] = s[i];
			k++, i++;
		}
	}	
}
