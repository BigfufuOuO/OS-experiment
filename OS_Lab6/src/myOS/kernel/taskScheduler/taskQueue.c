#include "../../include/task.h"


//初始化就绪队列
void readyQueueInit(myTCB* idleTask) {//对readyQueue进行初始化处理
    readyQueue.head = NULL;
    readyQueue.tail = NULL;
    readyQueue.idleTask = idleTask;
}    //idleTask不入队

//初始化等待队列
void waitingQueueInit(myTCB* idleTask)
{
    waitingQueue.head = NULL;
    waitingQueue.tail = NULL;
    waitingQueue.idleTask = idleTask;

}

//如果就绪队列为空，返回True
int readyQueueIsEmpty(void) {//当head和tail均为NULL时，readyQueue为空
    if (readyQueue.head == NULL && readyQueue.tail == NULL) {
        return 1;
    }
    else
        return 0;
}

//如果等待队列为空，返回True
int waitingQueueIsEmpty(void) {//当head和tail均为NULL时，waitingQueue为空
    if (waitingQueue.head == NULL && waitingQueue.tail == NULL) {
        return 1;
    }
    return 0;
}

//获取就绪队列的头结点信息，并返回
myTCB* readyQueueNextTask(void) {//获取下一个Task
    if (!readyQueue.head) {
        return &tcbPool[1];
    }
    else {
        return readyQueue.head;
    }
}    //若队列为空（如运行0号进程时），返回1号进程的地址

//获取等待队列的头结点信息，并返回
myTCB* waitingQueueNextTask(void) {//获取下一个Task
    if (!waitingQueue.head) {
        return &tcbPool[1];
    }
    else {
        return waitingQueue.head;
    }
}    //若队列为空（如运行0号进程时），返回1号进程的地址
