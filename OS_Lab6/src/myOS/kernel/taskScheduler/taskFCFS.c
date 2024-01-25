#include "../../include/task.h"
#include "../../include/scheduler.h"
#include "../../include/myPrintk.h"
#include "../../include/taskQueue.h"



//将一个未在就绪队列中的TCB加入到就绪队列中
void taskEnqueuereadyQueueFCFS(myTCB* task) {//将task入队readyQueue
    if (readyQueueIsEmpty()) {
        readyQueue.head = task;
        readyQueue.tail = task;
    }
    else {
        readyQueue.tail->readyQueueNextTCB = task;
        readyQueue.tail = task;
    }
    task->readyQueueNextTCB = NULL;
}

//将就绪队列中的TCB移除
void taskDequeuereadyQueueFCFS() {//readyQueue出队
    myTCB* task = readyQueue.head;
    if (readyQueue.head == readyQueue.tail) {
        readyQueue.head = NULL;
        readyQueue.tail = NULL;
    }
    else {
        readyQueue.head = readyQueue.head->readyQueueNextTCB;
    }
    task->readyQueueNextTCB = NULL;
}


/*****************等待队列******************/
//将一个未在等待队列中的TCB加入到等待队列中
void taskEnqueuewaitingQueueFCFS(myTCB* task) {//将task入队waitingQueue
    if (waitingQueueIsEmpty()) {
        waitingQueue.head = task;
        waitingQueue.tail = task;
    }
    else {
        waitingQueue.tail->waitingQueueNextTCB = task;
        waitingQueue.tail = task;
    }
    task->waitingQueueNextTCB = NULL;
}

//将等待队列中的TCB移除
void taskDequeuewaitingQueueFCFS(myTCB* task) {//waitingQueue出队
    if (waitingQueue.head == waitingQueue.tail) {
        if (task != waitingQueue.head) {
            return;
        }
        waitingQueue.head = NULL;
        waitingQueue.tail = NULL;
    }
    if (waitingQueue.head == task) {
        waitingQueue.head = waitingQueue.head->waitingQueueNextTCB;
        task->waitingQueueNextTCB = NULL;
    }
    myTCB* temp = waitingQueue.head;
    while (temp->waitingQueueNextTCB) {
        if (temp->waitingQueueNextTCB == task) {
            temp->waitingQueueNextTCB = temp->waitingQueueNextTCB->waitingQueueNextTCB;
            task->waitingQueueNextTCB = NULL;
            break;
        }
        temp = temp->waitingQueueNextTCB;
    }
}


//创建任务并加入等待队列，若到达时间为0则加入就绪队列
void createTaskFCFS(myTCB* task, void (*taskBody)(void)) {
    task->task_entrance = taskBody;
    stack_init(&(task->stackTop), taskBody);
    task->TASK_State = TASK_WAIT;
    if (task->paras.arrTime == 0) {
        task->TASK_State = TASK_READY;
        taskEnqueuereadyQueueFCFS(task);
    }
    else {
        taskEnqueuewaitingQueueFCFS(task);
    }
}

void tickHookFCFS() {
    myTCB* task = waitingQueue.head;
    while (task) {
        if (task->TASK_State == TASK_WAIT) {
            task->paras.arrTime--;
            if (task->paras.arrTime == 0) {
                task->TASK_State = TASK_READY;
                taskDequeuewaitingQueueFCFS(task);
                taskEnqueuereadyQueueFCFS(task);
            }
        }
        task = task->waitingQueueNextTCB;
    }
    if (currentTask->paras.exeTime) {
        currentTask->paras.exeTime--;
    }
}

void SchedulerFCFS(scheduler* sch) {
    *sch = (scheduler){
        .type = FCFS,
        .preemptiveOrNot = 0,
        .nextTaskFunc = readyQueueNextTask,
        .enqueueTaskFunc = taskEnqueuereadyQueueFCFS,
        .dequeueTaskFunc = taskDequeuereadyQueueFCFS,
        .schedulerInitFunc = readyQueueInit,
        .createTaskHook = createTaskFCFS,
        .tickHook = tickHookFCFS,
    };
}
