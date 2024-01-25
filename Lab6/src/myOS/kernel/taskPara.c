#include "../include/taskPara.h"
#include "../include/kmalloc.h"
#include "../include/task.h"
#include "../include/myPrintk.h"

void initTskPara(tskPara *buffer)
{
    buffer = (tskPara *)kmalloc(sizeof(tskPara));
    buffer->priority = 0;
    buffer->arriveTime = 0;
    buffer->excuteTime = 0;

}

void setTskPara(unsigned int option, unsigned int value, tskPara *para)
{
    switch (option)
    {
    case PRIORITY:
        para->priority = (value > MAX_PRIORITY)? MAX_PRIORITY : value;
        break;
    
    case ARRIVE_TIME:
        para->arriveTime = value;
        break;

    case EXCUTE_TIME:
        para->excuteTime = value;
        break;

    default:
        break;
    }
}

int getTskPara(unsigned option, tskPara para)
{
    switch (option)
    {
    case PRIORITY:
        return para.priority;
        break;
    
    case ARRIVE_TIME:
        return para.arriveTime;
        break;

    case EXCUTE_TIME:
        return para.excuteTime;
        break;

    default:
        return -1;
        break;
    }
}