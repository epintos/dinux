#include "../include/iolib.h"
#include "../include/buffer.h"
#include "../include/prints.h"
#include "../include/string.h"
#include <stdarg.h>

int cursorPos = 0;
extern int insertShellBuffer;
extern int readShellBuffer;
extern int INT_KEY;
extern unsigned char shellBuffer[];

void int_80(int operation, unsigned char *data, int fileDesc){
	if(operation==WRITE){
		service_Write(data,fileDesc);
		
	}
	else if(operation==READ)
		service_Read(data,fileDesc);
}

void service_Write(unsigned char *data, int fileDesc){
	if(fileDesc==STDOUT){
			if((*data) == 0x08){ // Backspace
				unsigned char c=' ';
				print(&c,cursorPos-2);
				cursorPos-=2;
				cursorPos=(cursorPos<0?0:cursorPos);
			}else if((*data) == '\n'){ // Enter
				unsigned char c=' ';
				cursorPos=(cursorPos+(160-(cursorPos%160)));
				print(&c,cursorPos);
			}else if((*data) != 0x09){ //Tab
				print(data,cursorPos);
				cursorPos+=2;
			}
	}
}			

void service_Read(unsigned char *data,int fileDesc) {
	if(fileDesc==STDIN){
		(*data)=getKey();
	}
}

void write(unsigned char *string, int length, int fileDes){
	int i = 0;
	for( i; i<length; i++){
		_wrapper_int_80(WRITE,string+i,fileDes);
	}
}

unsigned char read(int fileDesc){
	unsigned char c;
	_wrapper_int_80(READ,&c,fileDesc);
	return c;
}

void putc(int fileDesc, unsigned char ascii){
	write(&ascii,1,fileDesc);
}

unsigned char getc(int fileDesc){
	unsigned char c;
	if(insertShellBuffer==readShellBuffer){
		while((c=read(fileDesc)) == EOF);
	do {
		if((c != EOF) && (c != '\n')) {
			if(c=='\b'){
				if(insertShellBuffer>0){	
					insertShellBuffer--;
					putc(STDOUT,c);
				}
			}else{
				shellBuffer[insertShellBuffer++]=c;
				putc(STDOUT,c);
			}
			
		}
	}
	while((c != '\n') && ((c=read(fileDesc))!='\n') &&
										(insertShellBuffer<MAX_BUFFER));
	if(insertShellBuffer==MAX_BUFFER){
		putc(STDOUT,'\n');
		putc(STDOUT,'\b');
	}
	if(c=='\n')
		shellBuffer[insertShellBuffer++]=c;
		putc(STDOUT,c);
	}
	return shellBuffer[readShellBuffer++];
}

int printf(const char* fmt,...){
	/* Pointer to variable arguments list*/
	va_list list; 
	
	/*Indicates list where does the variable arguments starts*/
	va_start(list,fmt);	
	
	char c;
	int i=0;
	int qty=6;
	unsigned char qtyDigits[MAX_BUFFER];
	int qtySize=0;
	for(i;(c=fmt[i])!='\0';i++){
		if(c=='%'){
			char next=fmt[++i];
			if(next=='.'){
				next=fmt[++i];
				if((next-'0')>=0 && (next-'0')<=9){
					qtyDigits[qtySize++]=next;
					while(((next=fmt[++i])-'0')>=0 && (next-'0')<=9){
						qtyDigits[qtySize++]=next;
					}
					qty=toInteger(qtyDigits,qtySize);
				}else{
					qty=0;
					if(next!='f')
						putc(STDOUT,'.');
				}
				
			}
			switch(next)
			{
				/* Integer with sign, decimal base*/
				case 'd': printInteger(va_arg(list,int)); break;
				/* Integer with sign, decimal base*/
				case 'i': printInteger(va_arg(list,int)); break;
				/* Integer in octal base without sign*/
				case 'o':printOct(va_arg(list,long)); break;
				/* Integer in hexadecimal base with sign*/
				case 'x': printHex(va_arg(list,long)); break;
				/* Integer in hexadecimal base with sign*/
				case 'X': printHex(va_arg(list,long)); break;
				/* Integer without sign, decimal base*/
				case 'u':
				printUnsignedInteger(va_arg(list,unsigned int)); break;
				/* Character*/
				case 'c': putc(STDOUT,va_arg(list,int)); break;
				/* String*/
				case 's': printString(va_arg(list,char*)); break;
				/* Float without exponent*/
				case 'f': printFloat(va_arg(list,double),qty); break;
				/*Float with exponent*/
				case 'e': printExponent(va_arg(list,double)); break;
				/*Float with exponent*/
				case 'E': printExponent(va_arg(list,double)); break;
				/* % symbol*/
				case '%': putc(STDOUT,'%'); break;
			}
		}else{
			if (c == '\t'){
				putc(STDOUT,' ');putc(STDOUT,' ');
				putc(STDOUT,' ');putc(STDOUT,' ');
			}
			else {
				putc(STDOUT,c);
			}
		}
					
	}
	
	/* Frees list*/
	va_end(list);
	return i;
}

int scanf(const char* fmt,...){
	
	/* Pointer to variable arguments list*/
	va_list list; 
	
	/*Indicates list where does the variable arguments starts*/
	va_start(list,fmt);	

	char c;
	unsigned char type;
	int i=0;
	int qtySize=0;
	int qty=1;
	unsigned char qtyDigits[MAX_BUFFER];
	unsigned char inserted[MAX_BUFFER]={'\0'};
	char* auxS;
	char* auxC;
	int* auxD;
	float *auxF;
	readShellBuffer=0;
	for(i; (c=fmt[i])!='\0'; i++){
		int withSize=0;
		qty=0;
		qtySize=0;
		if(c=='%'){
			char next=fmt[++i];
			if((next-'0')>=0 && (next-'0')<=9){
				qtyDigits[qtySize++]=next;
				while(((next=fmt[++i])-'0')>=0 && (next-'0')<=9){
					qtyDigits[qtySize++]=next;
				}
				qty=toInteger(qtyDigits,qtySize);
				type=fmt[i];
				withSize=1;
			}else{
				type=next;
			}
			cleanShellBuffer();
			inserted[0]=getc(STDIN);
			if( type=='d' && withSize == 1 && inserted[0]=='-')
				qty++;
			int pos=1;
			if(withSize==1){
				while((--qty)>0 && readShellBuffer<insertShellBuffer){
					if(shellBuffer[readShellBuffer]!='\n')
					inserted[pos++]=shellBuffer[readShellBuffer++];
				}
			}
			else{
				while(readShellBuffer<insertShellBuffer){
					inserted[pos++]=shellBuffer[readShellBuffer++];
				}
			}
			switch(type){
				/* Character*/
				auxC=va_arg(list,char*);
				*auxC=inserted[0];
				case 'c':*va_arg(list,char*)=inserted[0]; break;
				/* Integer with sign, decimal base*/
				case 'd':
				if(inserted[0]=='\n')
					inserted[0]='0';
				auxD=va_arg(list,int*);
				*auxD=toInteger(inserted,pos); break;
				/* Float without exponent*/
				case 'f':
				auxF=va_arg(list,float*);
				*auxF=toFloat(inserted,pos); break;
				/* String*/
				case 's':
				auxS=va_arg(list,char*);
				strcpy(inserted,auxS);break;	
			}
			cleanShellBuffer();
		}
	}
	/* Frees list*/
	va_end(list);
	return 1;
}
