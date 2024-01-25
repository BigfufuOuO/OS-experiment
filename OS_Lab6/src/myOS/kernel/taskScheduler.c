#include "../include/myPrintk.h"
#include "../include/scheduler.h"

void setScheduler(int type) {
    switch (type) {
    case FCFS:
        SchedulerFCFS(&sysSch);
        break;
    case PRIO:
        SchedulerPrio(&sysSch);
        break;
    case SJF:
        SchedulerSJF(&sysSch);
        break;
    case RR:
        SchedulerRR(&sysSch);
    default:
        myPrintk(0x02, "Invalid sysScheduler type.\n");
        break;
    }
}