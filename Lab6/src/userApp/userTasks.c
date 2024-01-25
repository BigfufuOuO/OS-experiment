#include "../myOS/userInterface.h"
#include "shell.h"
#include "../myOS/include/task.h"

#define NULL (void*)0
#define WHITE 0x7
extern void tskEnd(void);
int createTsk(void (*tskBody)(void), tskPara para);
extern int myPrintf(int color,const char *format, ...);

char *message1 = "********************************\n";

void myTsk0(void){      
	myPrintf(WHITE, message1);
	myPrintf(WHITE, "*     Tsk0: BEGIN      *\n");
	myPrintf(WHITE, message1);
	
	tskEnd();   //the task is end
}

void myTsk1(void){
	myPrintf(WHITE, message1);
	myPrintf(WHITE, "*     Tsk1: HELLO WORLD!       *\n");
	
	myPrintf(WHITE, message1);
	
	tskEnd();   //the task is end
}

void myTsk2(void){
	myPrintf(WHITE, message1);
	myPrintf(WHITE, "*     Tsk2: HELLO WORLD!       *\n");
	myPrintf(WHITE, message1);
	
	tskEnd();  //the task is end
}

void myTsk3(void){
	myPrintf(WHITE, message1);
	myPrintf(WHITE, "*     Tsk3: HELLO WORLD!       *\n");
	myPrintf(WHITE, message1);
	tskEnd();  //the task is end
}

/*
int whatcurrenTsk(int argc, unsigned char ** argv)
{
	myPrintf(0x2, "current task:%s\n", currentTsk.);
}*/

int testFCFS(int argc, unsigned char ** argv)
{
	setScheduler(FCFS);
	setTickHook(systemSch.tick_hook);
	//tskPara para0 = {0,0,2};
	createTsk(myTsk3, (tskPara){2,1,2});
	tskPara para1 = {3,1,5};
	createTsk(myTsk0, para1);
	tskPara para2 = {4,3,4};
	createTsk(myTsk1, para2);
	tskPara para3 = {5,13,3};
	createTsk(myTsk2, para3);
}

//查看Readyqueue元素
int check_ReadyQueue(int argc, unsigned char ** argv)
{
	myTCB *p = readyQueue.head;
	while (p)
	{
		myPrintf(0x2, "task:%d, %d, %d, %d, %d\n", p->TSK_ID, p->TSK_State, p->para->priority, p->para->excuteTime, p->para->arriveTime);
		p = p->nextTCBready;
	}
	
}

//查看waitingqueue元素
int check_waitingQueue(int argc, unsigned char ** argv)
{
	myTCB *p = waitingQueue.head;
	while (p)
	{
		myPrintf(0x2, "task: %d, %d, %d, %d, %d\n", p->TSK_ID, p->TSK_State, p->para->priority, p->para->excuteTime, p->para->arriveTime);
	}
	
}

int check_currentTask(int argc, unsigned char ** argv)
{
	myPrintf(0x2, "current task:%d, %d, %d, %d, %d\n", currentTsk->TSK_ID, currentTsk->TSK_State, currentTsk->para->priority, currentTsk->para->excuteTime, currentTsk->para->arriveTime);
}

void TskInit(void)//任务初始化
{
	addNewCmd("checkCut", check_currentTask, NULL, "check current task. \0");
	addNewCmd("checkRdq", check_ReadyQueue, NULL, "check the readyQueue.\0");
	addNewCmd("checkWtq", check_waitingQueue, NULL, "check the waitingQueue.\0");
	addNewCmd("testFCFS\0",testFCFS, NULL, "test of FCFS tasks.\0");
}