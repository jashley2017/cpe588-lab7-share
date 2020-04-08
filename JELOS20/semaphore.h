// semaphore.h
// jelos
// James Lumpp
// 4/6/2020


typedef volatile int SEM_t;

void OS_Sem_Init(SEM_t *s, int count);
void OS_Sem_Post(SEM_t *s);
void OS_Sem_Pend(SEM_t *s);

void DisableInterrupts(void); // Disable interrupts
void EnableInterrupts(void);  // Enable interrupts
unsigned int StartCritical (void);    // previous I bit, disable interrupts
void EndCritical(unsigned int sr);    // restore I bit to previous value
void WaitForInterrupt(void);  // low power mode
