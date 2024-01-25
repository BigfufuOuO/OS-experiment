#include "io.h"

void init8259A(void){
	//masking interrupt source
	outb(0x21,0xff);
	outb(0xa1,0xff);

	//master initialization
	outb(0x20,0x11);
	outb(0x21,0x20);
	outb(0x21,0x04);
	outb(0x21,0x03);

	//slice initialization
	outb(0xa0,0x11);
	outb(0xa1,0x28);
	outb(0xa1,0x02);
	outb(0xa1,0x01);
}
