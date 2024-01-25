#include "../../include/task.h"
#include "../../include/scheduler.h"


//初始化就绪队列
void readyQueuePriority0Init(myTCB* idleTsk) {//对readyQueue进行初始化处理
     readyQueue.head = NULLptr;
     readyQueue.tail = NULLptr;
     readyQueue.idleTsk = idleTsk;
}

//如果就绪队列为空，返回True
int readyQueuePriority0IsEmpty(void) {//当head和tail均为NULL时，readyQueue为空
     if(readyQueue.head == NULLptr && readyQueue.tail == NULLptr){
          return 1;
     }
     else{
          return 0;
     }
}

//获取就绪队列的头结点信息，并返回
myTCB * nextPriority0Tsk(void) {//获取下一个Tsk
     if(readyQueuePriority0IsEmpty()){
          return idleTsk;
     }
     else{
          return readyQueue.head;
     }
}

//将一个未在就绪队列中的TCB加入到就绪队列中，按优先级升序
void tskEnqueuePriority0(myTCB *tsk) {//将tsk入队readyQueue
    if(readyQueuePriority0IsEmpty()){
        readyQueue.head = tsk;
        readyQueue.tail = tsk;
        tsk->nextTCBready = NULLptr;
    }
    else{
        if(tsk->para->priority < readyQueue.head->para->priority){
            tsk->nextTCBready = readyQueue.head;
            readyQueue.head = tsk;
        }
        else{
            myTCB *temp = readyQueue.head;
            while(temp->nextTCBready != NULLptr && temp->nextTCBready->para->priority <= tsk->para->priority){
                    temp = temp->nextTCBready;
            }
            tsk->nextTCBready = temp->nextTCBready;
            temp->nextTCBready = tsk;
            if(tsk->nextTCBready == NULLptr){
                    readyQueue.tail = tsk;
            }
        }
    }
}

//将就绪队列中的TCB移除(头出队)
void tskDequeuePriority0(void) {//readyQueue出队头
    if(readyQueuePriority0IsEmpty()){
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

/****等待队列*****/
//初始化等待队列
void waitingQueuePriority0Init(myTCB* idleTsk) {//对readyQueue进行初始化处理
     readyQueue.head = NULLptr;
     readyQueue.tail = NULLptr;
     readyQueue.idleTsk = idleTsk;
}

//等待队列是否为空
int waitingQueuePriority0IsEmpty(void)
{
    if(waitingQueue.head == NULLptr && waitingQueue.tail == NULLptr){
        return 1;
    }
    else{
        return 0;
    }
}

//获取等待队列的头结点信息，并返回
myTCB * nextPriority0WaitingTsk(void)
{
    if(waitingQueuePriority0IsEmpty()){
        return idleTsk;
    }
    else{
        return waitingQueue.head;
    }
}

//将一个未在等待队列中的TCB加入到等待队列中，按优先级升序
void tskEnqueuePriority0Waiting(myTCB *tsk)
{
    if(waitingQueuePriority0IsEmpty()){
        waitingQueue.head = tsk;
        waitingQueue.tail = tsk;
        tsk->nextTCBwaiting = NULLptr;
    }
    else{
        if(tsk->para->priority < waitingQueue.head->para->priority){
            tsk->nextTCBwaiting = waitingQueue.head;
            waitingQueue.head = tsk;
        }
        else{
            myTCB *temp = waitingQueue.head;
            while(temp->nextTCBwaiting != NULLptr && temp->nextTCBwaiting->para->priority <= tsk->para->priority){
                    temp = temp->nextTCBwaiting;
            }
            tsk->nextTCBwaiting = temp->nextTCBwaiting;
            temp->nextTCBwaiting = tsk;
            if(tsk->nextTCBwaiting == NULLptr){
                    waitingQueue.tail = tsk;
            }
        }
    }
}

//等待队列的TCB与tsk相等的移除
myTCB * tskDequeueWaitingPriority0(myTCB *tsk)
{
    if( waitingQueuePriority0IsEmpty() ){
        return NULLptr;
    }
    else{
        myTCB *temp = waitingQueue.head;
        if( temp == tsk ){
            waitingQueue.head = waitingQueue.head->nextTCBwaiting;
            if( waitingQueue.head == NULLptr ){
                waitingQueue.tail = NULLptr;
            }
            return temp;
        }
        else{
            while( temp->nextTCBwaiting != NULLptr && temp->nextTCBwaiting != tsk ){
                temp = temp->nextTCBwaiting;
            }
            if( temp->nextTCBwaiting == NULLptr ){
                return NULLptr;
            }
            else{
                temp->nextTCBwaiting = temp->nextTCBwaiting->nextTCBwaiting;
                if( temp->nextTCBwaiting == NULLptr ){
                    waitingQueue.tail = temp;
                }
                return tsk;
            }
        }
    }
}


//创建任务并加入等待队列，若到达时间为0则加入就绪队列
int createTaskPriority0(myTCB *tsk, void (*taskBody)(void))
{
    tsk->TSK_State = TSK_WAIT;
    if( tsk->para->arriveTime == 0 ){
        tskEnqueuePriority0(tsk);
        tsk->TSK_State = TSK_RDY;
    }
    else{
        tskEnqueuePriority0Waiting(tsk);
    }
    return 1;
}

//时间tick
void tickHookPriority0()
{
    myTCB * task = waitingQueue.head;
    while(task != NULLptr){
        task->para->arriveTime--;//等待时间减一
        if(task->para->arriveTime == 0){
            tskDequeueWaitingPriority0(task);//出等待队列
            tskEnqueuePriority0(task);//进就绪队列
        }
        task = task->nextTCBwaiting;
    }

    if(currentTsk->para->excuteTime)
    {
        currentTsk->para->excuteTime--;//当前任务的执行时间
    }
}

//任务调度
void schedulerPriority0(scheduler *sch)
{
    *sch = (scheduler){
        .type = PRIORITY0,
        .preemptive_or_not = 0,//不抢占
        .nextTsk_func = nextPriority0Tsk,
        .enqueueTsk_func = tskEnqueuePriority0,
        .dequeueTsk_func = tskDequeuePriority0,
        .schedulerInit_func = readyQueuePriority0Init,
        .createTsk_hook  = createTaskPriority0,
        .tick_hook = tickHookPriority0,
    };
}