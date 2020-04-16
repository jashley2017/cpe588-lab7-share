// semaphore.c
// jelos
// James Lumpp
// 4/6/2020

#include "semaphore.h"
#include "tm4c123gh6pm.h"


#define NO_SEM


#ifdef NO_SEM
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
	EndCritical(prev_state);
}

void OS_Sem_Pend(SEM_t *s)
{
	DisableInterrupts();
	while(*s==0){
		EnableInterrupts(); // allow things to happen so s could change
		DisableInterrupts();
	}
	*s = *s - 1; 
	EnableInterrupts();
}
#endif

