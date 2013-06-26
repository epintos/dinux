#include "../include/iolib.h"
#include "../include/commands.h"
#include "../include/shell.h"
#include "../include/buffer.h"

int insertShellBuffer = 0;
int readShellBuffer = 0;
unsigned char shellBuffer[MAX_BUFFER];
static unsigned char stringBuffer[MAX_BUFFER];
extern unsigned char commandsBuffer[10][MAX_BUFFER];
extern int commandToInsert;
extern int commandBufferActive;
extern int INT_KEY;
extern int cursorPos;
extern int UP_ARROW;
extern int DOWN_ARROW;
extern int commandToRead;
extern int commandsOverflow;
extern int prev;

void printShell() {
	printf("%s","Dinux:$ ");
}

void callShell(){
   while(1){
	 if(INT_KEY==1){
		 INT_KEY=0;
		 if( UP_ARROW== 1){
			UP_ARROW=0;
			displayCommand(UP_DISPLAY );
		 }else if( DOWN_ARROW == 1){
			DOWN_ARROW=0;
			displayCommand(DOWN_DISPLAY );
		 }else{
			 unsigned char aux=read(STDIN);
			 if( aux=='\n' || insertShellBuffer==MAX_BUFFER){
				 putc(STDOUT,'\n');
				 analyzeBuffer();
				 cleanShellBuffer();
				 printShell();
				 INT_KEY=0; // For getc
			 }else if( aux=='\b'){
				 if(insertShellBuffer>0){
						write(&aux,1,STDOUT); // Erases
						insertShellBuffer--;
				 }
			 }else{
				 if( insertShellBuffer<MAX_BUFFER){
					shellBuffer[insertShellBuffer++]=aux;
						putc(STDOUT,aux);
				}
			 }
		 }
	 }
  } 
}

void printShellBuffer(){
	int i=0;
	for (i; i<insertShellBuffer;i++){
		putc(STDOUT,shellBuffer[i]);
	}
		
}

void analyzeBuffer() {
	getStringInBuffer(); // Reads whats in shell buffer
	cleanShellBuffer();
	if(strcmp(&stringBuffer,"getcTest")){	
		getcTest();
		putc(STDOUT,'\n');
	}else if(strcmp(&stringBuffer,"getCPUSpeed")){
		getCPUSpeed();
		putc(STDOUT,'\n');
	}else if(strcmp(&stringBuffer,"info")){
		info();
	}else if(strcmp(&stringBuffer, "clear")) {
		k_clear_screen();
		cursorPos=0;
	} else if(strcmp(&stringBuffer, "scanfTest")) {
		scanfTest();
	}else if( strcmp(&stringBuffer,"commands")){
		commands();
	}else if( strcmp(&stringBuffer,"floatTest")){
		floatTest();
	}else if( strcmp(&stringBuffer,"help")){
		help();
	}else if( !strcmp(&stringBuffer,"")){
		printf("%s%s",stringBuffer,": comando invalido\n");
	}
	
	/* Adds command to the command buffer if it's not
	* already there*/
	if(!strcmp(&stringBuffer,"")){
		int add=1;
		int i=0;
		for(i; i< commandsBuffer[i][0]!='\0' && i<MAX_COMMAND_BUFFER &&
														   add==1; i++){
			if(strcmp(stringBuffer,commandsBuffer[i])){
				add=0;
			}
		}
		if(add==1)
			addCommand(stringBuffer);
		else
			commandToRead=i-1;
	}
}

void addCommand(char* stringBuffer){
	int i=0;
	commandBufferActive=1; // Actives the command display
	for(i; stringBuffer[i]!='\0' && i<MAX_BUFFER; i++){
		commandsBuffer[commandToInsert][i]=stringBuffer[i];
	}
	if(i<=(MAX_BUFFER-1))
		commandsBuffer[commandToInsert][i]='\0';
		
	commandToInsert++;
	commandToRead=commandToInsert-1; // Leaves the cursor to read before
									 // the last added
	
	if(commandToInsert==MAX_COMMAND_BUFFER){
		commandToInsert=0; // Starts inserting from the beginning again
		commandsOverflow=1; // This flag let's the cursorToRead move
							// without problem when the buffer is full
	}
	prev=NONE;
}

char* getStringInBuffer() {
	if(insertShellBuffer==0) {
		stringBuffer[0] = '\0';
		return stringBuffer;
	}
		int i=0;
		for(i; i < insertShellBuffer; i++) {
			stringBuffer[i] = shellBuffer[i];
		}
		stringBuffer[i] = '\0';
	
	return stringBuffer;
}

void cleanShellBuffer(){
	insertShellBuffer=0;
	readShellBuffer=0;
}
