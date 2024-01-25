#ifndef __TASK_H__
#define __TASK_H__

#ifndef USER_TASK_NUM
#include "taskPara.h"
#include "../../userApp/userApp.h"

#endif

#define TASK_NUM (2 + USER_TASK_NUM)   // at least: 0-idle, 1-init

#define initTskBody myMain         // connect initTask with myMain

#define STACK_SIZE 512            // definition of STACK_SIZE

void initTskBody(void);

void CTX_SW(void*prev_stkTop, void*next_stkTop);
void stack_init(unsigned long **stk, void (*task)(void));

//#error "TODO: 为 myTCB 增补合适的字段"
typedef struct myTCB {
     unsigned long *stkTop;        /* 栈顶指针 */
     unsigned long stack[STACK_SIZE];      /* 开辟了一个大小为STACK_SIZE的栈空间 */  
     unsigned long TSK_State;   /* 进程状态 */
     unsigned long TSK_ID;      /* 进程ID */ 
     void (*task_entrance)(void);  /*进程的入口地址*/
     tskPara *para;//进程的参数
     struct myTCB * nextTCBready;           /*下一个readyTCB*/
     struct myTCB * nextTCBwaiting;         /*下一个waitingTCB*/
} myTCB;

myTCB tcbPool[TASK_NUM];//进程池的大小设置

myTCB * idleTsk;                /* idle 任务 */
myTCB * currentTsk;             /* 当前任务 */
myTCB * firstFreeTsk;           /* 下一个空闲的 TCB */

void TaskManagerInit(void);
void schedule(void);
int createTsk(void (*tskBody)(void), tskPara para);
//int createTsk(void (*tskBody)(void));

//就绪队列的结构体
typedef struct taskQueue{
     myTCB * head;
     myTCB * tail;
     myTCB * idleTsk;
}taskQueue;

taskQueue readyQueue;
taskQueue waitingQueue;

#define NULLptr (myTCB *)0
#define TSK_RDY 0        //表示当前进程已经进入就绪队列中
#define TSK_WAIT -1      //表示当前进程还未进入就绪队列中
#define TSK_RUNING 1     //表示当前进程正在运行
#define TSK_NONE 2       //表示进程池中的TCB为空未进行分配

#include "scheduler.h"

void schedulerFCFS(scheduler *sch);
void schedulerPriority0(scheduler *sch);
void schedulerRR(scheduler *sch);
void schedulerSJF(scheduler *sch);
void setScheduler(int type);

void myTsk0(void);
void myTsk1(void);
void myTsk2(void);
void myTsk3(void);
#endif
