//  OS Main 
//  Author: James Lumpp
//  Date: 2/3/2020
//
// Pre-processor Directives
#include <stdio.h>  
#include <stdint.h> 
#include "UART.h"
#include "PLL.h"
#include "jelos.h"

#include <stdbool.h>
#include "inc/hw_memmap.h"
#include "driverlib/debug.h"
#include "driverlib/gpio.h"
#include "driverlib/rom.h"
#include "driverlib/sysctl.h"

#include "semaphore.h"


//  Global Declarations section

unsigned char task_zero_stack[MIN_STACK_SIZE]; // Declare a seperate stack 
unsigned char task_one_stack[MIN_STACK_SIZE];  // for each task
unsigned char task_two_stack[MIN_STACK_SIZE];
unsigned char task_shell_stack[1024];

SEM_t UART_Sem;  // declare a semaphore for the UART

// Function Prototypes
void shell(void);

void spin_delay(unsigned int d){
	unsigned int i=0;
	while(i<d)   // spin delay to waste processor clocks
		i++;
}

void slowprint(char *p){
 while (*p != 0){    // Print until null terminator is reached
	spin_delay(2000);  // print slowly
  putchar(*p++);
 }
}

void Zero(void)
{
	while(1) {
		OS_Sem_Pend(&UART_Sem);
		slowprint("The  0   task  zero.\n");
		OS_Sem_Post(&UART_Sem);
		
		OS_Delay(400000);
	}
  //tasks should not end
}

void One(void)
{
	while(1){
		
		OS_Sem_Pend(&UART_Sem);
		slowprint("TASK 1 USES ALL CAPS\n");
		OS_Sem_Post(&UART_Sem);
		
		OS_Delay(50000);
	}
} 
	
void Two(void)
{
	while (1){
		OS_Sem_Pend(&UART_Sem);
		slowprint("Task 2 now running..\n");
		OS_Sem_Post(&UART_Sem);
		
		OS_Delay(100000);
}
  //tasks should not end
	} 
	
void SysTick_Init(){
	ROM_SysTickEnable();
	ROM_SysTickPeriodSet(1668000);  // pick some reasonable timeout for context switches
	ROM_SysTickIntEnable();         // SysTick_Handler is in jelos.asm
}
	

// main
int main(void) {
	
	ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);  // Initialize PortF LEDs
	ROM_SysCtlDelay(1);
	ROM_GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_3 |GPIO_PIN_2| GPIO_PIN_1);
	
	PLL_Init();     // 50 MHz (SYSDIV2 == 7, defined in pll.h)
  UART_Init();    // initialize UART
	                //115,200 baud rate (assuming 50 MHz UART clock),
                  // 8 bit word length, no parity bits, one stop bit, FIFOs enabled
  SysTick_Init();
	
	OS_Sem_Init(&UART_Sem,1);  // initialize semaphore
	
	ROM_GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1,
                   GPIO_PIN_1); // Red LED on
	
  puts("\n\nWelcome to the CPE588s20 Operating System...\n\n");  
	puts("\nBy default SW1 will generate an edge interrupt for context switch\n");

	// Create tasks that will run (these are functions that do not return)
	
	puts("\nCreating Tasks...");
	CreateTask(shell, task_shell_stack, sizeof (task_shell_stack));
	CreateTask(Zero, task_zero_stack, sizeof (task_zero_stack));
	CreateTask(One, task_one_stack, sizeof (task_one_stack));
	CreateTask(Two, task_two_stack, sizeof (task_two_stack));
	
	puts("\nStarting Scheduler...");
	
	StartScheduler();  //Start the OS Scheduling, does not return
	
	while(1) // should never get here, but just in case...
		;
} //main





