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
#include "shell.h"

bool isnotsu;

int main()
{
	#ifdef __OpenBSD__
	pledge("stdio rpath proc exec", NULL);
	#endif
	
	struct command *k = malloc(sizeof(struct command));
	//printf("%lu\n", sizeof(*k));

	isnotsu = geteuid(); // Check if the user is a super-user or not
	while (1)
	//Infinite loop to keep the program going until exited
	{	
		char p;
		if (!isnotsu)
			p = '#';
		else
			p = '$';
    		printf("%s%c ", getenv("HOME"), p); //Prints the prompt
    		getCommands(k);
    		execCmd(k); //Gets and executes the commands
	}
}
