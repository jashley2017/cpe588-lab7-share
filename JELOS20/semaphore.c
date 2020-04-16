// semaphore.c
// jelos
// James Lumpp
// 4/6/2020

#include "semaphore.h"
#include "jelos.h"
#include "tm4c123gh6pm.h"


#define NO_SEM


#ifdef NO_SEM

void OS_Surrender(void) { 
	NVIC_INT_CTRL_R = 0x04000000; // trigger the systick with a "software" interrupt
	// does not restart the timer, just triggers a context switch
}
void OS_Sem_Init(SEM_t *s, int count)
{
	unsigned int prev_state; 
	prev_state = StartCritical(); 
	*s = count; 
	EndCritical(prev_state);
}

void OS_Sem_Post(SEM_t *s)
{
	unsigned int prev_state; 
	prev_state = StartCritical();
	*s = *s + 1; 
	UnblockTask(s);
	EndCritical(prev_state);
}

void OS_Sem_Pend(SEM_t *s)
{
	DisableInterrupts();
	while(*s==0){
		BlockCurrentTask(s);
		EnableInterrupts(); // allow things to happen so s could change
		OS_Surrender();
		DisableInterrupts();
	}
	DisableInterrupts(); // this is in case the OS_Surrender context switch causes interrupts to be reenabled.
	*s = *s - 1; 
	EnableInterrupts();
}
#endif

