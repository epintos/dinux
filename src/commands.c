#include "../include/commands.h"
#include "../include/shell.h"
#include "../include/buffer.h"
#include "../include/iolib.h"

extern tickpos;
unsigned char commandsBuffer[MAX_COMMAND_BUFFER][MAX_BUFFER]={'\0'};
extern unsigned char shellBuffer[];
int commandToRead=0;
int commandToInsert=0;
int commandBufferActive=0;
extern int insertShellBuffer;
static int printKind;
int commandsOverflow=0;
int prev=NONE;
static int outOfBuffer=0;

void getCPUSpeed() {
	
	float speeds[3]={0};
	float result1=0;
	float result2=0;
	float result=0;
	int i=0;
	int j=1;
	printf("Calculating CPU speed...\n");
	
	//Method 1
	for(i; i<3; i++){
		speeds[i]=calculateSpeedMethod1();
		printf("Partial %d ",j++);
		printSpeed(speeds[i]);
		putc(STDOUT,'\n');
		result1+=speeds[i];
	}
	result1/=3;
	printf("Averiage %d ",1);
	printSpeed(result1);
	putc(STDOUT,'\n');
	
	//Method 2
	for(i=0; i<3; i++){
		speeds[i]=calculateSpeedMethod2();
		printf("Partial %d ",j++);
		printSpeed(speeds[i]);
		putc(STDOUT,'\n');
		result2+=speeds[i];
	}
	result2/=3;
	printf("Averiage %d ",2);
	printSpeed(result2);
	
	//Total averiage
	result=(result1+result2)/2;
	printf("\n\nCalculation finished\n");
	printSpeed(result);
}

float calculateSpeedMethod1(){
	unsigned int rdtsc1, rdtsc2, tick1, tick2;
	unsigned int result, tickRes;
	float ans;
	
	rdtsc1 = getRDTSC();
	tickpos=0;
	
	while(tickpos<10);
	
	rdtsc2 = getRDTSC();
	tick2 = tickpos;
	
	result = rdtsc2 - rdtsc1;
	tickRes = tick2;
	
	ans = ((result)/(tickRes*55))*1000;
	return convertSpeed(ans);
}

float calculateSpeedMethod2(){
	unsigned int rdtsc1, rdtsc2, tick1, tick2;
	unsigned int result, tickRes;
	float ans;
	
	rdtsc1 = getRDTSC();
	tickpos=0;
	int i=0;
	
	while(i++<100000000);
	
	rdtsc2 = getRDTSC();
	tick2 = tickpos;
	
	result = rdtsc2 - rdtsc1;
	tickRes = tick2;
	
	ans = ((result)/(tickRes*55))*1000;
	return convertSpeed(ans);
}

float convertSpeed(float speed){
	if(speed >=(1024*1024*1024)){
		printKind=Ghz;
		return speed/(1024*1024*1024);
	}else if(speed >= (1024*1024)){
		printKind=Mhz;
		return speed/(1024*1024);
	}else{
		printKind=Hz;
		return speed;
	}
}

void printSpeed(float result){
	printf("%s","CPU Speed: ");
	if(printKind==Ghz){
		printf("%.1f Ghz",result);
	}else if(printKind==Mhz){
		printf("%.1f Mhz", result);
	}else{
		printf("%.1f Hz",result);
	}
}
unsigned int getRDTSC() {
	unsigned int aux = CPU_Speed();
	return aux;
}

void getcTest(){
	char c1=getc(STDIN);
	putc(1,'|');
	putc(1,c1);
	putc(1,'|');
}

void scanfTest(){
	char day[10]={'\0'};
	int number;
	int year;
	char month[10]={'\0'};
	char initial;
	printf("%s","Ingrese su inicial: ");
	scanf("%1c",&initial);
	printf("%s","Ingrese que dia de la semana es hoy: ");
	scanf("%9s",day);
	printf("%s","Ingrese el numero: ");
	scanf("%2d",&number);
	printf("%s","Ingrese el nombre del mes: ");
	scanf("%9s",month);
	printf("%s","Ingrese el anio: ");
	scanf("%4d",&year);
	printf("%c: Hoy es %s %d de %s del %d\n",initial,day,number,
															month,year);
}

void displayCommand(int arrow){
	if ( commandBufferActive==1){
		// Erases what is in the actual line
		while((insertShellBuffer--)>0)
			putc(STDOUT,'\b');
		cleanShellBuffer();
		int i=0;
		if(arrow==DOWN_DISPLAY ){
			if (commandToRead==0){
				outOfBuffer=1;
				return;
			}
			else if ( commandToRead>0){
				commandToRead--;
				if(prev==UP_DISPLAY && outOfBuffer==0)
					commandToRead--;
				if(commandToRead<0){
					outOfBuffer=1;
					return;
				}
			}
			outOfBuffer=0;
			prev=DOWN_DISPLAY;
		}
		else{
			if((commandToRead== MAX_COMMAND_BUFFER) ||
								(commandToRead==commandToInsert
											&& commandsOverflow==0)){
				outOfBuffer=1;
				prev=UP_DISPLAY;
				return;
			}
			if(prev==DOWN_DISPLAY && outOfBuffer==0){
				commandToRead++;
				if((commandToRead== MAX_COMMAND_BUFFER) ||
								(commandToRead==commandToInsert)){
				outOfBuffer=1;
				prev=UP_DISPLAY;
				return;
				}
			}
	
			outOfBuffer=0;
			prev=UP_DISPLAY;
		}
		cleanShellBuffer();
		
		/*Reads command and puts it in the shell buffer*/
		for(i;commandsBuffer[commandToRead][i]!='\0' && i<MAX_BUFFER;
																++i){
			shellBuffer[insertShellBuffer++]=
									   commandsBuffer[commandToRead][i];
		}
		if (arrow==UP_DISPLAY){
			commandToRead++;
		}
		printShellBuffer();
	}
}

void commands(){
	printf("%s\t%s\t%s\t%s\t%s\t%s\t%s\n%s\n","info","scanfTest",
	"getCPUSpeed","commands","getcTest","floatTest","help","clear");
}

void info(){
	printf(" %s, release %.1f\n","Dinux",1.0);
	printf(" Developed by:\n %s\n %s\n %s\n","Federico Di Nucci",
		"Matias De Santi", "Esteban Pintos");
}

void floatTest(){
		float f;
		printf("1) %f\n",-0.006545);
		printf("2) %.4f\n",2342.02346);
		printf("3) %.2f\n",-3453.0);
		printf("4) %.3f\n",000.006545);
		printf("5) %e\n",1341.005745);	
		printf("6) %e\n",-0.005745);
		printf("7) %e\n",-3.005745);
		printf("8) %e\n",-2.455745);
		printf("9) %e\n",-2345.005745);
		
		printf("%s","Ingrese un float: ");
		scanf("%f",&f);
		printf("Float ingresado con dos dicimales : %.2f\n",f);
}

void help(){
	printf("\n\tinfo\t%s\n\n","information of system and developers");
	printf("\tscanfTest\t%s\n\n","small program to test scanf\
 functions");
	printf("\tgetCPUSpeed\t%s\n\n","prints the microprocessor speed");
	printf("\tcommands\t%s\n\n","shows the available commands in the\
 system");
	printf("\tgetcTest\t%s\n\n","small program to test getc function");
	printf("\tfloatTest\t%s\n\n","small program to test printf and\
 scanf with float");
	printf("\thelp\t%s\n\n","system commands help");
	printf("\tclear\t%s\n\n","clears screen");
}

void welcome(){
	printf("\n\n\n\t\t\t\tDDDDDDDDDDDDD");
	printf("\n\t\t\t\tD::::::::::::DDD");
	printf("\n\t\t\t\tD:::::::::::::::DD");
	printf("\n\t\t\t\tDDD:::::DDDDD:::::D");                     
	printf("\n\t\t\t\t  D:::::D    D:::::D xxxxxxx      xxxxxxx");
	printf("\n\t\t\t\t  D:::::D     D:::::D x:::::x    x:::::x");
	printf("\n\t\t\t\t  D:::::D     D:::::D  x:::::x  x:::::x");
	printf("\n\t\t\t\t  D:::::D     D:::::D   x:::::xx:::::x");
	printf("\n\t\t\t\t  D:::::D     D:::::D    x::::::::::x");
	printf("\n\t\t\t\t  D:::::D     D:::::D     x::::::::x");
	printf("\n\t\t\t\t  D:::::D     D:::::D     x::::::::x");
	printf("\n\t\t\t\t  D:::::D    D:::::D     x::::::::::x");
	printf("\n\t\t\t\tDDD:::::DDDDD:::::D     x:::::xx:::::x");
	printf("\n\t\t\t\tD:::::::::::::::DD     x:::::x  x:::::x");
	printf("\n\t\t\t\tD::::::::::::DDD      x:::::x    x:::::x");
	printf("\n\t\t\t\tDDDDDDDDDDDDD        xxxxxxx      xxxxxxx");
	printf("\n\n\t\t\t\t\t\t Press enter to continue\n\n");
}
