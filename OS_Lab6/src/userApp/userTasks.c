#include "../myOS/include/wallClock.h"
#include "../myOS/include/task.h"
#include "shell.h"
#include "../myOS/include/scheduler.h"

#define WHITE 0x7
extern void taskEnd(void);
//extern int createTask(void (*taskBody)(void));
extern int myPrintf(int color, const char* format, ...);

char* message1 = "********************************\n";

void myTask0(void) {
	myPrintf(WHITE, message1);
	myPrintf(WHITE, "*     Task0: HELLO!     *\n");
	int times = 0, temp = 0;
	while (times < 10) {
		if (temp != tick_number) {
			if (tick_number % 10 == 0) {
				myPrintf(WHITE, "%d ", tick_number);
				times++;
			}
		}
		temp = tick_number;
	}
	myPrintf(WHITE, "\n");
	myPrintf(WHITE, message1);

	taskEnd();   //the task is end
}

void myTask1(void) {
	myPrintf(0x3, message1);
	myPrintf(0x3, "*     Task1: HELLO!     *\n");
	int times = 0, temp = 0;
	while (times < 10) {
		if (temp != tick_number) {
			if (tick_number % 10 == 0) {
				myPrintf(0x3, "%d ", tick_number);
				times++;
			}
		}
		temp = tick_number;
	}
	myPrintf(WHITE, "\n");
	myPrintf(0x3, message1);
	taskEnd();   //the task is end
}

void myTask2(void) {
	myPrintf(0x4, message1);
	myPrintf(0x4, "*     Task2: HELLO!     *\n");
	int times = 0, temp = 0;
	while (times < 10) {
		if (temp != tick_number) {
			if (tick_number % 10 == 0) {
				myPrintf(0x4, "%d ", tick_number);
				times++;
			}
		}
		temp = tick_number;
	}
	myPrintf(WHITE, "\n");
	myPrintf(0x4, message1);

	taskEnd();  //the task is end
}

void myTask3(void) {
	myPrintf(0x5, message1);
	myPrintf(0x5, "*     Task3: HELLO!     *\n");
	int times = 0, temp = 0;
	while (times < 10) {
		if (temp != tick_number) {
			if (tick_number % 10 == 0) {
				myPrintf(0x5, "%d ", tick_number);
				times++;
			}
		}
		temp = tick_number;
	}
	myPrintf(WHITE, "\n");
	myPrintf(0x5, message1);

	taskEnd();  //the task is end
}

int testFCFS(int argc, unsigned char ** argv)
{
	setScheduler(FCFS);
	setTickHook(sysSch.tickHook);
	//tskPara para0 = {0,0,2};
	//createTsk(myTsk3, &(tskPara){2,1,2});
	taskPara para1 = {3,1,5};
	createTask(myTask0, para1);
	taskPara para2 = {4,3,4};
	createTask(myTask1, para2);
	taskPara para3 = {5,13,3};
	createTask(myTask2, para3);
}

//查看Readyqueue元素
int check_ReadyQueue(int argc, unsigned char ** argv)
{
	myTCB *p = readyQueue.head;
	while (p)
	{
		myPrintf(0x2, "task:%d, %d, %d, %d, %d\n", p->TASK_ID, p->TASK_State, p->paras.priority, p->paras.arrTime, p->paras.exeTime);
		p = p->readyQueueNextTCB;
	}
	
}

//查看waitingqueue元素
int check_waitingQueue(int argc, unsigned char ** argv)
{
	myTCB *p = waitingQueue.head;
	while (p)
	{
		myPrintf(0x2, "task: %d, %d, %d, %d, %d\n", p->TASK_ID, p->TASK_State, p->paras.priority, p->paras.arrTime, p->paras.exeTime);
		p = p->waitingQueueNextTCB;
	}
	
}

int check_currentTask(int argc, unsigned char ** argv)
{
	myPrintf(0x2, "current task:%d, %d, %d, %d, %d\n", currentTask->TASK_ID, currentTask->TASK_State, currentTask->paras.priority,
	currentTask->paras.arrTime, currentTask->paras.exeTime);
}

void TskInit(void)//任务初始化
{
	addNewCmd("checkCut", check_currentTask, NULL, "check current task. \0");
	addNewCmd("checkRdq", check_ReadyQueue, NULL, "check the readyQueue.\0");
	addNewCmd("checkWtq", check_waitingQueue, NULL, "check the waitingQueue.\0");
	addNewCmd("testFCFS\0",testFCFS, NULL, "test of FCFS tasks.\0");
}