#include "vga.h"

char *start_p = (char *)0xb8f90;
#define colon 0x3a

void displayClock(int hh, int mm, int ss)
{
    char str[9];

    //fill num
    str[0] = hh/10 + '0';
    str[1] = hh%10 + '0';
    str[2] = colon;
    str[3] = mm/10 + '0';
    str[4] = mm%10 + '0';
    str[5] = colon;
    str[6] = ss/10 + '0';
    str[7] = ss%10 + '0';
    str[8] = '\0';

    putstr_vga(str, 27, VGA_ROW - 1, VGA_COL - 8);
}

void setWallClock(int hh,int mm,int ss){
    if(hh < 0 || hh > 23)
        hh = 0;
    if(mm < 0 || mm > 59)
        mm = 0;
    if(ss < 0 || ss > 59)
        ss = 0;
    
    displayClock(hh,mm,ss);
}

void getWallClock(int *hh,int *mm,int *ss){
	char str[8];
    int i;

    for(i = 0;i<8;i++)
    {
        str[i] = *(start_p + 2*i);
    }

    *hh = (str[0]-'0')*10 + str[1]-'0';
    *mm = (str[3]-'0')*10 + str[4]-'0';
    *ss = (str[6]-'0')*10 + str[7]-'0';
}
