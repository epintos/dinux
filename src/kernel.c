#include "../include/kasm.h"
#include "../include/defs.h"
#include "../include/shell.h"
#include "../include/iolib.h"

DESCR_INT idt[0x81];	/* IDT with 129 entries*/
IDTR idtr;				/* IDTR */
extern int ENTER;
extern int INT_KEY;
unsigned long long int tickpos=0;
extern int cursorPos;

void int_08() {
  tickpos++;    
}

/**********************************************
kmain() 
Punto de entrada de cóo C.
*************************************************/

kmain() 
{

        int i,num;
        
/* Borra la pantalla y cursor */ 

	k_clear_screen();
	cursorOFF();


/* CARGA DE IDT CON LA RUTINA DE ATENCION DE IRQ0    */

   setup_IDT_entry (&idt[0x08], 0x08, (dword)&_int_08_hand, ACS_INT, 0);
		
/* CARGA DE IDT CON LA RUTINA DE ATENCION DE IRQ1    */

   setup_IDT_entry (&idt[0x09], 0x08, (dword)&_int_09_hand, ACS_INT, 0);
		
/*	CARGA LA INT 80H AL IDT */		
   setup_IDT_entry (&idt[0x80], 0x08, (dword)&_int_80_hand, ACS_INT, 0);
	
/* Loads IDTR */

	idtr.base = 0;  
	idtr.base +=(dword) &idt;
	idtr.limit = sizeof(idt)-1;
	
	_lidt (&idtr);	

	_Cli();
	
/* Enables timer tick and keyboard interruption */
	
	_mascaraPIC1(0xFC);
        _mascaraPIC2(0xFF);
       
        
	_Sti();
	welcome();
	while(ENTER!=1);
	INT_KEY=0; 
	
	printf("%f",2.5); // To solve the FPU/Math coprocessor problem
	
	k_clear_screen();
	cursorPos=0;
	printShell();
    callShell();
	
}
