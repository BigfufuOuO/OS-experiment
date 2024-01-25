#include "../../include/task.h"
#include "../../include/scheduler.h"
#include "../../include/myPrintk.h"
#include "../../include/taskQueue.h"


//将一个未在就绪队列中的TCB按优先数升序加入到就绪队列中
void taskEnqueuereadyQueuePrio(myTCB* task) {//将task入队readyQueue
    if (readyQueueIsEmpty()) {
        readyQueue.head = task;
        readyQueue.tail = task;
    }
    else if (readyQueue.head == readyQueue.tail) {
        if (task->paras.priority < readyQueue.head->paras.priority) {
            task->readyQueueNextTCB = readyQueue.head;
            readyQueue.head = task;
            schedule(); //优先级高切换
            return;
        }
        else {
            readyQueue.tail->readyQueueNextTCB = task;
            task->readyQueueNextTCB = NULL;
            readyQueue.tail = task;
        }
    }
    else {
        if (task->paras.priority < readyQueue.head->paras.priority) {
            task->readyQueueNextTCB = readyQueue.head;
            readyQueue.head = task;
            schedule(); 
            return;
        }
        myTCB* temp = readyQueue.head;
        while (temp->readyQueueNextTCB) {
            if (task->paras.priority < temp->readyQueueNextTCB->paras.priority) {
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
void taskDequeuereadyQueuePrio() {//readyQueue出队
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


//将一个未在等待队列中的TCB加入到等待队列中
void taskEnqueuewaitingQueuePrio(myTCB* task) {//将task入队waitingQueue
    if (waitingQueueIsEmpty()) {
        waitingQueue.head = task;
        waitingQueue.tail = task;
    }
    else if (waitingQueue.head == waitingQueue.tail) {
        if (task->paras.priority < waitingQueue.head->paras.priority) {
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
        if (task->paras.priority < waitingQueue.head->paras.priority) {
            task->waitingQueueNextTCB = waitingQueue.head;
            waitingQueue.head = task;
        }
        myTCB* temp = waitingQueue.head;
        while (temp->waitingQueueNextTCB) {
            if (task->paras.priority < temp->waitingQueueNextTCB->paras.priority) {
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
void taskDequeuewaitingQueuePrio(myTCB* task) {//waitingQueue出队
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
void createTaskPrio(myTCB* task, void (*taskBody)(void)) {
    task->task_entrance = taskBody;
    stack_init(&(task->stackTop), taskBody);
    task->TASK_State = TASK_WAIT;
    if (task->paras.arrTime == 0) {
        task->TASK_State = TASK_READY;
        taskEnqueuereadyQueuePrio(task);
    }
    else {
        taskEnqueuewaitingQueuePrio(task);
    }
}

void tickHookPrio() {
    myTCB* task = waitingQueue.head;
    while (task) {
        if (task->TASK_State == TASK_WAIT) {
            task->paras.arrTime--;
            if (task->paras.arrTime == 0) {
                task->TASK_State = TASK_READY;
                taskDequeuewaitingQueuePrio(task);
                taskEnqueuereadyQueuePrio(task);
            }
        }
        task = task->waitingQueueNextTCB;
    }
    if (currentTask->paras.exeTime) {
        currentTask->paras.exeTime--;
    }
}

void SchedulerPrio(scheduler* sch) {
    *sch = (scheduler){
        .type = PRIO,
        .preemptiveOrNot = 0,
        .nextTaskFunc = readyQueueNextTask,
        .enqueueTaskFunc = taskEnqueuereadyQueuePrio,
        .dequeueTaskFunc = taskDequeuereadyQueuePrio,
        .schedulerInitFunc = readyQueueInit,
        .createTaskHook = createTaskPrio,
        .tickHook = tickHookPrio,
    };
}
