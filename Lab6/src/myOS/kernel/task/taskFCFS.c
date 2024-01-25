#include "../../include/myPrintk.h"
#include "../../include/task.h"
#include "../../include/scheduler.h"


//初始化就绪队列（需要填写）
void readyQueueFCFSInit(myTCB* idleTsk) {//对readyQueue进行初始化处理
     readyQueue.head = NULLptr;
     readyQueue.tail = NULLptr;
     readyQueue.idleTsk = idleTsk;
}

//如果就绪队列为空，返回True（需要填写）
int readyQueueFCFSIsEmpty(void) {//当head和tail均为NULL时，readyQueue为空
     if(readyQueue.head == NULLptr && readyQueue.tail == NULLptr){
          return 1;
     }
     else{
          return 0;
     }
}

//获取就绪队列的头结点信息，并返回（需要填写）
myTCB * nextFCFSTsk(void) {//获取下一个Tsk
     if(readyQueueFCFSIsEmpty()){
          return idleTsk;
     }
     else{
          return readyQueue.head;
     }
}

//将一个未在就绪队列中的TCB加入到就绪队列中（需要填写）
void tskEnqueueFCFS(myTCB *tsk) {//将tsk入队readyQueue
     if(readyQueueFCFSIsEmpty()){
          readyQueue.head = tsk;
          readyQueue.tail = tsk;
          tsk->nextTCBready = NULLptr;
     }
     else{
          readyQueue.tail->nextTCBready = tsk;
          readyQueue.tail = tsk;
          tsk->nextTCBready = NULLptr;
     }
     myPrintk(0x2, "inonce2:%d\n", readyQueue.tail->para->excuteTime);
}

//将就绪队列中的TCB移除（需要填写）
void tskDequeueFCFS(void) {//readyQueue出队头
     if(readyQueueFCFSIsEmpty()){
          return;
     }
     else{
        myPrintk(0x2, "outonce2:%d\n", readyQueue.head->para->excuteTime);
          if(readyQueue.head == readyQueue.tail){
               readyQueue.head = NULLptr;
               readyQueue.tail = NULLptr;
          }
          else{
               readyQueue.head = readyQueue.head->nextTCBready;
          }
        //*(readyQueue.head->task_entrance);
        
     }   
}

/*****等待队列****/
void waitingQueueFCFSInit(myTCB* idleTsk) {//对waitingQueue进行初始化处理
     waitingQueue.head = NULLptr;
     waitingQueue.tail = NULLptr;
     waitingQueue.idleTsk = idleTsk;
}

//等待队列是否为空
int waitingQueueFCFSIsEmpty(void)
{
    if(waitingQueue.head == NULLptr && waitingQueue.tail == NULLptr){
        return 1;
    }
    else{
        return 0;
    }
}

//获取等待队列的头结点信息，并返回
myTCB * nextWaitingTskFCFS(void)
{
    if(waitingQueueFCFSIsEmpty()){
        return idleTsk;
    }
    else{
        return waitingQueue.head;
    }
}

//将一个未在等待队列中的TCB加入到等待队列中
void tskEnqueueWaitingFCFS(myTCB *tsk)
{
    if(waitingQueueFCFSIsEmpty()){
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
myTCB * tskDequeueWaitingFCFS(myTCB *tsk)
{
    if(waitingQueueFCFSIsEmpty()){
        return idleTsk;
    }
    else{
        if(waitingQueue.head == waitingQueue.tail){
            if(waitingQueue.head == tsk){
                waitingQueue.head = NULLptr;
                waitingQueue.tail = NULLptr;
                return tsk;
            }
            else{
                waitingQueue.head = NULLptr;
                waitingQueue.tail = NULLptr;
                return idleTsk;
            }
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

//创建任务并加入等待队列. 若时间为0则直接加入就绪队列
int createTskFCFS(myTCB *task, void (*taskBody)(void))
{

    task->task_entrance = taskBody;
    stack_init(&(task->stkTop),taskBody);
    task->TSK_State =TSK_WAIT;
    if(task->para->arriveTime == 0){
        task->TSK_State = TSK_RDY;
        tskEnqueueFCFS(task);  
    }
    else{
        tskEnqueueWaitingFCFS(task);
    }
    return 1;

}

void tickhookFCFS(void)//tick
{
    //myPrintk(0x2, "jehje22\n");
    myTCB * task = waitingQueue.head;
    while(task){
        if(task->para->arriveTime)
            task->para->arriveTime--;//等待时间减一
        if(task->para->arriveTime == 0){
            tskDequeueWaitingFCFS(task);//出等待队列
            task->TSK_State = TSK_RDY;
            tskEnqueueFCFS(task);//进就绪队列
        }
        task = task->nextTCBwaiting;
    }

    if(currentTsk->para->excuteTime)
    {
        currentTsk->para->excuteTime--;//当前任务的执行时间
    }
}

void schedulerFCFS(scheduler *sch)
{
    *sch = (scheduler){
        .type = FCFS,
        .preemptive_or_not = 0,
        .nextTsk_func = nextFCFSTsk,
        .enqueueTsk_func = tskEnqueueFCFS,
        .dequeueTsk_func = tskDequeueFCFS,
        .schedulerInit_func = readyQueueFCFSInit,
        .createTsk_hook = createTskFCFS,
        .tick_hook = tickhookFCFS,
    };
}