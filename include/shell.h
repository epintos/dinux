#ifndef _SHELL_
#define _SHELL_

#define UP_DISPLAY 1
#define DOWN_DISPLAY 0
#define NONE 2

/* Prints what is in the shell buffer at the moment */
void printShellBuffer();

/* If INT_KEY = 1 then analyzes what's in the keyboard buffer */
void callShell();

/* Puts insertShellBuffer and readShellBuffer in 0 */
void cleanShellBuffer();

/* Reads what's in the shell buffer and leaves a string in
 * stringBuffer */
char* getStringInBuffer();

/*Analyzes the content of the buffer in search for known commands.
 * If a command is recognized, calls the function in charge of executing
 * that command. If not, an error message is printed. Also adds the
 *  command to the commands buffer, if a command is not repetead*/
 void analyzeBuffer();
 
 /* Adds what is in the shell to te commands buffer */
 void addCommand(char *stringBuffer);
 
#endif
