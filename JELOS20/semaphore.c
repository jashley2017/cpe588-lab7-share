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
}

void OS_Sem_Post(SEM_t *s)
{
}

void OS_Sem_Pend(SEM_t *s)
{
}
#endif
