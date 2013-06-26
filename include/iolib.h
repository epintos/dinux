#ifndef _IOLIB_
#define _IOLIB_

#define WRITE 4
#define READ 3
#define STDOUT 1
#define STDIN 0

/* Identifies the operation and calls the respective service */
void int_80(int operation, unsigned char *data, int fileDesc);

/* Analizes which is the the data, and according to it, calls print*/
void service_Write(unsigned char *data,int fileDesc);

/* Modifies data with the value in the buffer */
void service_Read(unsigned char *data,int fileDesc);

/* Puts an ascii in the fileDesc, if fileDesc=STDIOUT puts it in the 
 * screen*/
void putc(int fileDesc, unsigned char ascii);

/* Gets a character from the fileDesc, if fileDesc=STDIN, gets a
 * character, from the keyboard's buffer*/
unsigned char getc(int fileDesc);

/* Similar to printf of stdio.h */
int printf(const char* fmt,...);

/* Similar to scanf of stdio.h */
int scanf(const char* fmt,...);

/* Writes a string array of "length" in the fileDesc, if
 * fileDesc=STDOUT, writes it in the screen*/
void write(unsigned char * string, int length, int fileDes);

/* Returns the value in the keyboard buffer */
unsigned char read(int fileDesc);

#endif
