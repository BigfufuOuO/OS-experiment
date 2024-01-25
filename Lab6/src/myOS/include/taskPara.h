#ifndef _TASKPARA_H_
#define _TASKPARA_H_

#define PRIORITY 0
#define ARRIVE_TIME 1
#define EXCUTE_TIME 2

#define MAX_PRIORITY 7 //最大优先级

typedef struct tskPara
{
    int priority; //优先级
    int arriveTime; //到达时间
    int excuteTime; //执行时间

} tskPara;

void initTskPara(tskPara *buffer);
void setTskPara(unsigned int option, unsigned int value, tskPara *buffer);
int getTskPara(unsigned option, tskPara para);//获得参数

#endif