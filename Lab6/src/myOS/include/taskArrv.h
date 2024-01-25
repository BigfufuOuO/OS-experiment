#ifndef __TASK_ARRV_H__
#define __TASK_ARRV_H__

#include "task.h"

#define PriorityQueue 0
#define NULL 0

typedef struct tskQueue_Node //队列结点
{
    myTCB *TCB;
    struct tskQueue_Node *next;
}tskQueue_Node;

typedef struct tskQueue
{
    tskQueue_Node *head;
    tskQueue_Node *tail;
}tskQueue;

/***************FIFO队列**************/
void tskQueueFIFO_Init(tskQueue *queue);
int tskQueueFIFO_IsEmpty(tskQueue *queue);//检测是否为空
myTCB *tskQueueFIFO_FistTCB(tskQueue *queue); //获取队头的第一个TCB
void tskQueueFIFO_Enqueue(tskQueue *queue, myTCB *tsk);
myTCB *tskQueueFIFO_Dequeue(tskQueue *queue);//出队

/*************优先级队列***************/
void tskQueuePriority_Init(tskQueue *queue);
int tskQueuePriority_IsEmpty(tskQueue *queue);
myTCB *tskQueuePriority_FistTCB(tskQueue *queue);
void tskQueuePriority_Enqueue(tskQueue *queue, myTCB *tsk);
myTCB *tskQueuePriority_Dequeue(tskQueue *queue);



void taskArrvQueueInit(void);
void enableTask(int tid);

#endif