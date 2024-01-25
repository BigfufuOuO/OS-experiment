#ifndef _SCHEDULER_H_
#define _SCHEDULER_H_

#include "../include/task.h"

#define FCFS 0
#define PRIO 1
#define SJF 2
#define RR 3

typedef struct scheduler {
     unsigned int type;
     int preemptiveOrNot;     //if True, the scheduler is preemptive
     myTCB* (*nextTaskFunc)(void);
     void(*enqueueTaskFunc)(myTCB* task);
     void(*dequeueTaskFunc)();
     void(*schedulerInitFunc)(myTCB* task);
     void(*createTaskHook)(myTCB* task, void (*taskBody)(void));    //if set, will be called in createTask (before taskStart)
     void(*tickHook)(void);   //if set, tickHook will be called every tick
}scheduler;

scheduler sysSch;

void SchedulerFCFS(scheduler* sch);
void SchedulerSJF(scheduler* sch);
void SchedulerPrio(scheduler* sch);
void SchedulerRR(scheduler* sch);
void setScheduler(int type);

#endif