#include "../include/task.h"
#include "../include/scheduler.h"
#include "../include/myPrintk.h"

void setScheduler(int type)
{
    switch (type)
    {
    case FCFS:
        schedulerFCFS(&systemSch);
        break;

    case SJF:
        schedulerSJF(&systemSch);
        break;

    case PRIORITY0:
        schedulerPriority0(&systemSch);
        break;

    case RR:
        schedulerRR(&systemSch);
        break; 

    default:
        myPrintk(0x07, "unknown schedule type.\n");
        break;
    }
}
