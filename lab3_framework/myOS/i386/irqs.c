#include "vga.h"
#include "myPrintk.h" 
void ignoreIntBody(void){
    //myPrintk(0x07,"Unknown interrupt\n\0");
    putstr_vga("Unknown interrupt\0", 0xc, VGA_ROW - 1, 0);
}
