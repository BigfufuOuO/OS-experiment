#include "io.h"

#define i8253_clock 1193180
#define i8253_clock_set 100
unsigned short split_screen = i8253_clock / i8253_clock_set;

void init8253(void){
	outb(0x43,0x34);

	//split-screen parameter
	outb(0x40,(unsigned char)split_screen);//lower 8 bit
	outb(0x40,(unsigned char)(split_screen >> 8));//higher 8 bit

	//enable interrupt
	outb(0x21,inb(0x21) & 0xfe);//put lowest bit to 0
}
