#include "../../include/task.h"
#include "../../include/scheduler.h"
#include "../../include/wallClock.h"

#define TIME_SLICE 10

//初始化就绪队列
void readyQueueRRInit(myTCB *idleTsk){
    readyQueue.head = NULLptr;
    readyQueue.tail = NULLptr;
    readyQueue.idleTsk = idleTsk;
}

//如果就绪队列为空，返回True
int readyQueueRRIsEmpty(void){
    if(readyQueue.head == NULLptr && readyQueue.tail == NULLptr){
        return 1;
    }
    else{
        return 0;
    }
}

//获取就绪队列的头结点信息，并返回
myTCB * nextRRTsk(void){
    if(readyQueueRRIsEmpty()){
        return idleTsk;
    }
    else{
        return readyQueue.head;
    }
}

//将一个未在就绪队列中的TCB加入到就绪队列中
void tskEnqueueRR(myTCB *tsk){
    if(readyQueueRRIsEmpty()){
        readyQueue.head = tsk;
        readyQueue.tail = tsk;
        tsk->nextTCBready = NULLptr;
    }
    else{
        readyQueue.tail->nextTCBready = tsk;
        readyQueue.tail = tsk;
        tsk->nextTCBready = NULLptr;
    }
}

//将就绪队列中的TCB移除
void tskDequeueRR(void){
    if(readyQueueRRIsEmpty()){
        return;
    }
    else{
        if(readyQueue.head == readyQueue.tail){
            readyQueue.head = NULLptr;
            readyQueue.tail = NULLptr;
        }
        else{
            readyQueue.head = readyQueue.head->nextTCBready;
        }
    }
}

/******等待队列******/
//初始化等待队列
void waitingQueueRRInit(myTCB *idleTsk)
{
    waitingQueue.head = NULLptr;
    waitingQueue.tail = NULLptr;   
    waitingQueue.idleTsk = idleTsk;
}

//如果等待队列为空，返回True
int waitingQueueRRIsEmpty(void)
{
    if(waitingQueue.head == NULLptr && waitingQueue.tail == NULLptr){
        return 1;
    }
    else{
        return 0;
    }
}

//获取等待队列的头结点信息，并返回
myTCB * nextWaitingTsk(void)
{
    if(waitingQueueRRIsEmpty()){
        return idleTsk;
    }
    else{
        return waitingQueue.head;
    }
}

//将一个未在等待队列中的TCB加入到等待队列中
void tskEnqueueWaitingRR(myTCB *tsk)
{
    if(waitingQueueRRIsEmpty()){
        waitingQueue.head = tsk;
        waitingQueue.tail = tsk;
        tsk->nextTCBwaiting = NULLptr;
    }
    else{
        waitingQueue.tail->nextTCBwaiting = tsk;
        waitingQueue.tail = tsk;
        tsk->nextTCBwaiting = NULLptr;
    }
}

//等待队列中的TCB与tsk相等的移除
myTCB * tskDequeueWaitingRR(myTCB *tsk)
{
    if(waitingQueueRRIsEmpty()){
        return idleTsk;
    }
    else{
        if(waitingQueue.head == waitingQueue.tail){
            if(waitingQueue.head == tsk){
                return tsk;
            }
            waitingQueue.head = NULLptr;
            waitingQueue.tail = NULLptr;
            return idleTsk;
        }
        else if(waitingQueue.head == tsk){
            waitingQueue.head = waitingQueue.head->nextTCBwaiting;
            return tsk;
        }
        else{
            myTCB * task = waitingQueue.head;
            while(task->nextTCBwaiting != tsk){
                task = task->nextTCBwaiting;
            }
            task->nextTCBwaiting = task->nextTCBwaiting->nextTCBwaiting;
            return tsk;
        }
    }
}

//创建任务并加入等待队列，若到达时间为0则加入就绪队列
int createTaskRR(myTCB *tsk, void (*taskBody)(void))
{
    tsk->TSK_State = TSK_WAIT;
    if( tsk->para->arriveTime == 0 ){
        tskEnqueueRR(tsk);
        tsk->TSK_State = TSK_RDY;
    }
    else{
        tskEnqueueWaitingRR(tsk);
    }
    return 1;
}

//RR调度
void tickHookRR(void)
{
    myTCB *task = waitingQueue.head;
    while (task)
    {
        if(task->TSK_State == TSK_WAIT)
        {
            task->para->arriveTime--;
        }
        if(task->para->arriveTime == 0)
        {
            task->TSK_State = TSK_RDY;
            tskDequeueWaitingRR(task);
            tskEnqueueRR(task);
        }

        task = task->nextTCBwaiting;

    }

    if(currentTsk->para->excuteTime)
    {
        currentTsk->para->excuteTime--;
    }

    if(readyQueue.head != readyQueue.tail)
    {
        if(tick_number % TIME_SLICE == 0)
        {
            task = readyQueue.head;
            tskDequeueRR();
            tskEnqueueRR(task);
            schedule();
            return;
        }
    }
    
}

void schedulerRR(scheduler *sch)
{
    *sch = (scheduler){
        .type = RR,
        .preemptive_or_not = 1,
        .nextTsk_func = nextRRTsk,
        .enqueueTsk_func = tskEnqueueRR,
        .dequeueTsk_func = tskDequeueRR,
        .schedulerInit_func = readyQueueRRInit,
        .createTsk_hook = createTaskRR,
        .tick_hook = tickHookRR,
    };
}