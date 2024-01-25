#ifndef _TASKQUEUE_H_
#define _TASKQUEUE_H_
#include "task.h"

void readyQueueInit(myTCB* idleTask);
void waitingQueueInit(myTCB* idleTask);
int readyQueueIsEmpty(void);
int waitingQueueIsEmpty(void);
myTCB* readyQueueNextTask(void);
myTCB* waitingQueueNextTask(void);

#endif