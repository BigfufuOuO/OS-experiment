#include "../../include/task.h"
#include "../../include/scheduler.h"

/*短作业优先调度*/
//初始化就绪队列
void readyQueueSJFInit(myTCB* idleTsk) {//对readyQueue进行初始化处理
     readyQueue.head = NULLptr;
     readyQueue.tail = NULLptr;
     readyQueue.idleTsk = idleTsk;
}

//如果就绪队列为空，返回True
int readyQueueSJFIsEmpty(void) {//当head和tail均为NULL时，readyQueue为空
     if(readyQueue.head == NULLptr && readyQueue.tail == NULLptr){
          return 1;
     }
     else{
          return 0;
     }
}

//获取就绪队列的头结点信息，并返回
myTCB * nextSJFTsk(void) {//获取下一个Tsk
     if(readyQueueSJFIsEmpty()){
        return idleTsk;
     }
     else{
        return readyQueue.head;
     }
}

//将一个未在就绪队列中的TCB加入到就绪队列中，按执行时间的升序
void tskEnqueueSJF(myTCB *tsk)
{
    if(readyQueueSJFIsEmpty())
    {
        readyQueue.head = tsk;
        readyQueue.tail = tsk;
        tsk->nextTCBready = NULLptr;
    }
    else
    {
        myTCB *p = readyQueue.head;
        myTCB *q = NULLptr;
        while(p != NULLptr && p->para->excuteTime < tsk->para->excuteTime)//按执行时间的升序排序
        {
            q = p;
            p = p->nextTCBready;
        }
        if(p == NULLptr)//如果p为空，说明tsk的执行时间最长，将tsk插入到队尾
        {
            readyQueue.tail->nextTCBready = tsk;
            readyQueue.tail = tsk;
            tsk->nextTCBready = NULLptr;
        }
        else if(p == readyQueue.head)//如果p为head，说明tsk的执行时间最短，将tsk插入到队头
        {
            tsk->nextTCBready = readyQueue.head;
            readyQueue.head = tsk;
        }
        else//否则，将tsk插入到p和q之间
        {
            q->nextTCBready = tsk;
            tsk->nextTCBready = p;
        }
    }
}

//将就绪队列中的TCB移除
void tskDequeueSJF(myTCB *tsk) {//readyQueue出队头
     if(readyQueueSJFIsEmpty()){
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

/*****等待队列******/
void waitingQueueSJFInit(myTCB *idleTsk) {//对waitQueue进行初始化处理
     waitingQueue.head = NULLptr;
     waitingQueue.tail = NULLptr;
     waitingQueue.idleTsk = idleTsk;
}

//等待队列是否为空
int waitingQueueSJFIsEmpty(void)
{
    if(waitingQueue.head == NULLptr && waitingQueue.tail == NULLptr)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

//获取等待队列头结点信息
myTCB *nextwaitingQueueSJF(void)
{
    if(waitingQueueSJFIsEmpty())
    {
        return idleTsk;
    }
    else
    {
        return waitingQueue.head;
    }
}

//将一个未在等待队列中的TCB加入到等待队列中，按执行时间的升序
void tskEnqueueWaitingSJF(myTCB *tsk)
{
    if(waitingQueueSJFIsEmpty())
    {
        waitingQueue.head = tsk;
        waitingQueue.tail = tsk;
        tsk->nextTCBwaiting = NULLptr;
    }
    else
    {
        myTCB *p = waitingQueue.head;
        myTCB *q = NULLptr;
        while(p != NULLptr && p->para->excuteTime < tsk->para->excuteTime)//按执行时间的升序排序
        {
            q = p;
            p = p->nextTCBwaiting;
        }
        if(p == NULLptr)//如果p为空，说明tsk的执行时间最长，将tsk插入到队尾
        {
            waitingQueue.tail->nextTCBwaiting = tsk;
            waitingQueue.tail = tsk;
            tsk->nextTCBwaiting = NULLptr;
        }
        else if(p == waitingQueue.head)//如果p为head，说明tsk的执行时间最短，将tsk插入到队头
        {
            tsk->nextTCBwaiting = waitingQueue.head;
            waitingQueue.head = tsk;
        }
        else//否则，将tsk插入到p和q之间
        {
            q->nextTCBwaiting = tsk;
            tsk->nextTCBwaiting = p;
        }
    }
}

//等待队列的TCB与tsk相等的移除
myTCB * tskDequeueWaitingSJF(myTCB *tsk)
{
    if(waitingQueueSJFIsEmpty()){
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
int createTaskSJF(myTCB *tsk, void (*taskBody)(void))
{
    tsk->task_entrance = taskBody;
    stack_init(&(tsk->stkTop),taskBody);
    tsk->TSK_State = TSK_WAIT;
    if(tsk->para->arriveTime == 0)
    {
        tsk->TSK_State = TSK_RDY;
        tskEnqueueSJF(tsk);
    }
    else
        tskDequeueSJF(tsk);
    return 1;
}

void tickHookSJF()
{
    myTCB * task = waitingQueue.head;
    while(task != NULLptr){
        task->para->arriveTime--;//等待时间减一
        if(task->para->arriveTime == 0){
            tskDequeueWaitingSJF(task);//出等待队列
            tskEnqueueSJF(task);//进就绪队列
        }
        task = task->nextTCBwaiting;
    }

    if(currentTsk->para->excuteTime)
    {
        currentTsk->para->excuteTime--;//当前任务的执行时间
    }
}

void schedulerSJF(scheduler *sch)
{
    *sch = (scheduler){
        .type = SJF,
        .preemptive_or_not = 0,
        .nextTsk_func = nextSJFTsk,
        .enqueueTsk_func = tskEnqueueSJF,
        .dequeueTsk_func = tskDequeueSJF,
        .schedulerInit_func = readyQueueSJFInit,
        .createTsk_hook = createTaskSJF,
        .tick_hook = tickHookSJF,
    };
}