#include "../include/myPrintk.h"
#include "../include/wallClock.h"
extern void oneTickUpdateWallClock(void);       //TODO: to be generalized

void (*tick_hook)(void) = 0;

/*设置tick_hook*/
void setTickHook(void (*func)(void)) {
        tick_hook = func;
}

int tick_number = 0;
void tick(void){
     tick_number++;	

     oneTickUpdateWallClock();

     if(tick_hook) 
     {
        tick_hook();  //user defined  
     }
}
