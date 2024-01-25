#include "../include/task.h"
#include "../include/taskArrv.h"
#include "../include/taskPara.h"

/*********FIFO队列************/
void tskQueueFIFO_Init(tskQueue *queue)
{
    queue->head = NULL;
    queue->tail = NULL;
}

int tskQueueFIFO_IsEmpty(tskQueue *queue)//队列是否为空
{
    if(queue->head == NULL && queue->tail == NULL)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

myTCB *tskQueueFIFO_FistTCB(tskQueue *queue)//获取队头第一个元素
{  
    if(tskQueueFIFO_IsEmpty(queue))
        return NULL;
    else
        return queue->head->TCB;

}

void tskQueueFIFO_Enqueue(tskQueue *queue, myTCB *tsk)//队尾插入元素
{
    tskQueue_Node *newnode = (tskQueue_Node *)kmalloc(sizeof(tskQueue_Node));
    newnode->TCB = tsk;
    newnode->next = NULL;
    if(tskQueueFIFO_IsEmpty(queue))
    {
        queue->head = newnode;
        queue->tail = newnode;
    }
    else
    {
        queue->tail->next = newnode;
        queue->tail = newnode;
    }
}

myTCB *tskQueueFIFO_Dequeue(tskQueue *queue)//出队
{
    if(tskQueueFIFO_IsEmpty(queue))
        return NULL;
    else
    {
        tskQueue_Node *node = queue->head;
        myTCB *tsk = queue->head->TCB;
        
        if(queue->head == queue->tail)
        {
            queue->head = NULL;
            queue->tail = NULL;
        }
        else
        {
            queue->head = queue->head->next;
        }

        kfree(node);

        return tsk;
    }
}

/*************优先级队列*************/
void tskQueuePriority_Init(tskQueue *queue)
{
    queue->head = NULL;
    queue->tail = NULL;
}

int tskQueuePriority_IsEmpty(tskQueue *queue)
{
    if(queue->head == NULL && queue->tail == NULL)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

myTCB *tskQueuePriority_FistTCB(tskQueue *queue)//获取队头的第一个元素
{
    if(tskQueuePriority_IsEmpty(queue))
        return NULL;
    else
        return queue->head->TCB;
}

void tskQueuePriority_Enqueue(tskQueue *queue, myTCB *tsk)//入队，保持优先级最高处于队头
{
    tskQueue_Node *newnode = (tskQueue_Node *)kmalloc(sizeof(tskQueue_Node));
    newnode->TCB = tsk;
    newnode->next = NULL;

    if(tskQueuePriority_IsEmpty(queue))
    {
        queue->head = newnode;
        queue->tail = newnode;
    }
    else
    {
        tskQueue_Node *node = queue->head;
        tskQueue_Node *pre = NULL;
        while(node != NULL && node->TCB->para->priority >= tsk->para->priority)
        {
            pre = node;
            node = node->next;
        }
        if(pre == NULL)
        {
            newnode->next = queue->head;
            queue->head = newnode;
        }
        else
        {
            pre->next = newnode;
            newnode->next = node;
            if(node == NULL)
                queue->tail = newnode;
        }
    }
}

myTCB *tskQueuePriority_Dequeue(tskQueue *queue)//出队
{
    if(tskQueueFIFO_IsEmpty(queue))
    return NULL;
    else
    {
        tskQueue_Node *node = queue->head;
        myTCB *tsk = queue->head->TCB;
        
        if(queue->head == queue->tail)
        {
            queue->head = NULL;
            queue->tail = NULL;
        }
        else
        {
            queue->head = queue->head->next;
        }

        kfree(node);

        return tsk;
    }
}

/****************到达队列处理**************/
tskQueue *tskQueue_Arrive;

