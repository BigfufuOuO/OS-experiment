#ifndef _SCHEDULER_H_
#define _SCHEDULER_H_

#include "task.h"

#define FCFS 0
#define SJF 1
#define PRIORITY0 2
#define RR 3


typedef struct scheduler {
    unsigned long type; // 调度器类型
    int preemptive_or_not; //if True, the scheduler is preemptive
    myTCB* (*nextTsk_func)(void);//get the next task to run
    void (*enqueueTsk_func)(myTCB *tsk);//put a task into the ready queue
    void (*dequeueTsk_func)();//remove a task from the ready queue
    void (*schedulerInit_func)(myTCB* idleTsk);//调度器初始化
    int (*createTsk_hook)(myTCB *task, void (*taskBody)(void));//创建任务钩子
    void (*tick_hook)(void); //if set, tick_hook will be called every tick
}scheduler;

scheduler systemSch;

#endif