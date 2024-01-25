#ifndef __TASK_H__
#define __TASK_H__

#ifndef USER_TASK_NUM
#include "../../userApp/userApp.h"
#endif

#define TASK_NUM (2 + USER_TASK_NUM)   // at least: 0-idle, 1-init

#define initTaskBody myMain         // connect initTask with myMain

#define STACK_SIZE 512            // definition of STACK_SIZE

#define TASK_READY 0     //表示当前进程已经进入就绪队列中
#define TASK_WAIT -1     //表示当前进程还未进入就绪队列中
#define TASK_RUNNING 1   //表示当前进程正在运行
#define TASK_NONE 2      //表示进程池中的TCB为空未进行分配

#define NULL (void*)0


//option for setTaskPara()/getTaskPara
#define PRIORITY 1
#define EXETIME 2
#define ARRTIME 3
#define SCHED_POLICY 4


void initTaskBody(void);

void CTX_SW(void* prev_stackTop, void* next_stackTop);

// struct for taskPara
typedef struct taskPara {
     unsigned int priority;
     unsigned int exeTime;
     unsigned int arrTime;
} taskPara;

//#error "TODO: 为 myTCB 增补合适的字段"
typedef struct myTCB {
     unsigned long* stackTop;        /* 栈顶指针 */
     unsigned long stack[STACK_SIZE];      /* 开辟了一个大小为STACK_SIZE的栈空间 */
     unsigned long TASK_State;   /* 进程状态 */
     unsigned long TASK_ID;      /* 进程ID */
     taskPara paras;
     void (*task_entrance)(void);  /*进程的入口地址*/
     struct myTCB* readyQueueNextTCB;      /*就绪队列中下一个TCB*/
     struct myTCB* waitingQueueNextTCB;      /*等待队列中下一个TCB*/
} myTCB;

extern myTCB tcbPool[TASK_NUM];    //进程池的大小设置

extern myTCB* idleTask;            /* idle 任务 */
extern myTCB* currentTask;         /* 当前任务 */
extern myTCB* firstFreeTask;       /* 下一个空闲的 TCB */

//extern scheduler sch;

//任务队列的结构体
typedef struct taskQueue {
     myTCB* head;
     myTCB* tail;
     myTCB* idleTask;
} taskQueue;


taskQueue readyQueue;
taskQueue waitingQueue;

void stack_init(unsigned long** stack, void (*task)(void));
int createTask(void (*taskBody)(void), taskPara paras);
void schedule(void);
void TaskManagerInit(void);
void initTaskPara(taskPara* buffer);
void setTaskPara(unsigned int option, unsigned int value, taskPara* buffer);
unsigned int getTaskPara(unsigned int option, taskPara* buffer);


#endif
