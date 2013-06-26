#ifndef _STRING_
#define _STRING_

#define INT_MAX_VALUE 10

/*Compares s with t, if they are equals returns 1*/
int strcmp(unsigned char *s,char *t);

/*Copies "from" to "to" and returns "to" */
char *strcpy(char *from, char *to);

/* Generates a float with an array of char*/
float toFloat(unsigned char* array, int arraySize);

/* Generates an integer with an array of char*/
int toInteger(unsigned char* array, int arraySize);

#endif
