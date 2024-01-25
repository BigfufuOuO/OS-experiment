#include "wallClock.h"
int system_ticks;
int hh,mm,ss,ms;

void tick(void){
	system_ticks++;
	ms += 10;
	if(ms >= 1000)
	{
		ms = 0;
		++ss;
	}
	if(ss >= 60)
	{
		ss = 0;
		++mm;
	}
	if(mm >= 60)
	{
		mm = 0;
		++hh;
	}
	if(hh >= 24)
	{
		hh = 0;
	}
	setWallClock(hh,mm,ss);
	return;
}

