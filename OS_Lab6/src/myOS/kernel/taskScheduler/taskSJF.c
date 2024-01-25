#include "../../include/task.h"
#include "../../include/scheduler.h"
#include "../../include/myPrintk.h"
#include "../../include/taskQueue.h"


//将一个未在就绪队列中的TCB按执行时间升序加入到就绪队列中
void taskEnqueuereadyQueueSJF(myTCB* task) {//将task入队waitingQueue
    if (readyQueueIsEmpty()) {
        readyQueue.head = task;
        readyQueue.tail = task;
    }
    else if (readyQueue.head == readyQueue.tail) {
        if (task->paras.exeTime < readyQueue.head->paras.exeTime) {
            task->readyQueueNextTCB = readyQueue.head;
            readyQueue.head = task;
            schedule();//抢占式进行一次调度
            return;
        }
        else {
            readyQueue.tail->readyQueueNextTCB = task;
            task->readyQueueNextTCB = NULL;
            readyQueue.tail = task;
        }
    }
    else {
        if (task->paras.exeTime < readyQueue.head->paras.exeTime) {
            task->readyQueueNextTCB = readyQueue.head;
            readyQueue.head = task;
            schedule();
            return;
        }
        myTCB* temp = readyQueue.head;
        while (temp->readyQueueNextTCB) {
            if (task->paras.exeTime < temp->readyQueueNextTCB->paras.exeTime) {
                task->readyQueueNextTCB = temp->readyQueueNextTCB;
                temp->readyQueueNextTCB = task;
                break;
            }
            temp = temp->readyQueueNextTCB;
        }
        if (!(temp->readyQueueNextTCB)) {
            temp->readyQueueNextTCB = task;
            task->readyQueueNextTCB = NULL;
            readyQueue.tail = task;
        }
    }
}

//将就绪队列中的TCB移除
void taskDequeuereadyQueueSJF() {//waitingQueue出队
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


/********等待队列********/
//将一个未在等待队列中的TCB加入到等待队列中
void taskEnqueuewaitingQueueSJF(myTCB* task) {//将task入队waitingQueue
    if (waitingQueueIsEmpty()) {
        waitingQueue.head = task;
        waitingQueue.tail = task;
    }
    else if (waitingQueue.head == waitingQueue.tail) {
        if (task->paras.exeTime < waitingQueue.head->paras.exeTime) {
            task->waitingQueueNextTCB = waitingQueue.head;
            waitingQueue.head = task;
        }
        else {
            waitingQueue.tail->waitingQueueNextTCB = task;
            task->waitingQueueNextTCB = NULL;
            waitingQueue.tail = task;
        }
    }
    else {
        if (task->paras.exeTime < waitingQueue.head->paras.exeTime) {
            task->waitingQueueNextTCB = waitingQueue.head;
            waitingQueue.head = task;
        }
        myTCB* temp = waitingQueue.head;
        while (temp->waitingQueueNextTCB) {
            if (task->paras.exeTime < temp->waitingQueueNextTCB->paras.exeTime) {
                task->waitingQueueNextTCB = temp->waitingQueueNextTCB;
                temp->waitingQueueNextTCB = task;
                break;
            }
            temp = temp->waitingQueueNextTCB;
        }
        if (!(temp->waitingQueueNextTCB)) {
            temp->waitingQueueNextTCB = task;
            task->waitingQueueNextTCB = NULL;
            waitingQueue.tail = task;
        }
    }
}

//将等待队列中的TCB移除
void taskDequeuewaitingQueueSJF(myTCB* task) {//waitingQueue出队
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
void createTaskSJF(myTCB* task, void (*taskBody)(void)) {
    task->task_entrance = taskBody;
    stack_init(&(task->stackTop), taskBody);
    task->TASK_State = TASK_WAIT;
    if (task->paras.arrTime == 0) {
        task->TASK_State = TASK_READY;
        taskEnqueuereadyQueueSJF(task);
    }
    else {
        taskEnqueuewaitingQueueSJF(task);
    }
}

void tickHookSJF() {
    myTCB* task = waitingQueue.head;
    while (task) {
        if (task->TASK_State == TASK_WAIT) {
            task->paras.arrTime--;
            if (task->paras.arrTime == 0) {
                task->TASK_State = TASK_READY;
                taskDequeuewaitingQueueSJF(task);
                taskEnqueuereadyQueueSJF(task);
            }
        }
        task = task->waitingQueueNextTCB;
    }
    if (currentTask->paras.exeTime) {
        currentTask->paras.exeTime--;
    }
}

void SchedulerSJF(scheduler* sch) {
    *sch = (scheduler){
        .type = SJF,
        .preemptiveOrNot = 0,
        .nextTaskFunc = readyQueueNextTask,
        .enqueueTaskFunc = taskEnqueuereadyQueueSJF,
        .dequeueTaskFunc = taskDequeuereadyQueueSJF,
        .schedulerInitFunc = readyQueueInit,
        .createTaskHook = createTaskSJF,
        .tickHook = tickHookSJF,
    };
}
