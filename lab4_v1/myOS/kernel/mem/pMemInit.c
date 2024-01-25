#include "../../include/myPrintk.h"
#include "../../include/mem.h"
unsigned long pMemStart;  // 可用的内存的起始地址
unsigned long pMemSize;  // 可用的大小

unsigned long kMemStart;
unsigned long uMemStart;

void memTest(unsigned long start, unsigned long grainSize){
	// TODO
	/*功能：检测算法
		这一个函数对应实验讲解ppt中的第一大功能-内存检测。
		本函数的功能是检测从start开始有多大的内存可用，具体算法参照ppt检测算法的文字描述
	注意点三个：
	1、覆盖写入和读出就是往指针指向的位置写和读，不要想复杂。
	  (至于为什么这种检测内存的方法可行大家要自己想一下)
	2、开始的地址要大于1M，需要做一个if判断。
	3、grainsize不能太小，也要做一个if判断
	*/
	if(start < 0x10000)
		start = 0x10000;

	if(grainSize < 2)
		grainSize = 2;

	unsigned long addr = start;
	unsigned short data;
	unsigned short * head, * tail;
	pMemStart = start;
	pMemSize = 0;
	
	int fail = 0;
	while (fail == 0)
	{
		head = (unsigned short *)addr;
		tail = (unsigned short *)(addr + grainSize - 2);

		data = *head;
		*head = 0xAA55;
		if(*head != 0xAA55)
			fail = 1;
		*head = 0x55AA;
		if(*head != 0x55AA)
			fail = 1;
		*head = data;

		data = *tail;
		*tail = 0xAA55;
		if(*tail != 0xAA55)
			fail = 1;
		*tail = 0x55AA;
		if(*tail != 0x55AA)
			fail = 1;
		*tail = data;

		if(fail == 0)
		{
			addr += grainSize;
			pMemSize += grainSize;
		}
	}
	myPrintk(0x7,"addr:%x\n",addr);
	

	myPrintk(0x7,"MemStart: %x  \n", pMemStart);
	myPrintk(0x7,"MemSize:  %x  \n", pMemSize);
	
}

extern unsigned long _end;
void pMemInit(void){
	unsigned long _end_addr = (unsigned long) &_end;
	memTest(0x100000,0x1000);
	myPrintk(0x7,"_end:  %x  \n", _end_addr);
	if (pMemStart <= _end_addr) {
		pMemSize -= _end_addr - pMemStart;
		pMemStart = _end_addr;
	}
	
	// 此处选择不同的内存管理算法
	pMemHandler = dPartitionInit(pMemStart,pMemSize);
	kMemStart = dPartitionAlloc(pMemHandler, pMemSize / 2 - 30 * sizeof(unsigned long));
	myPrintk(0x17, "kMemStart: %x\n", kMemStart);
	uMemStart = dPartitionAlloc(pMemHandler, pMemSize / 2 - 30 * sizeof(unsigned long));
	myPrintk(0x17, "uMemStart: %x\n", uMemStart);

	pMemHandler = dPartitionInit(kMemStart, pMemSize / 2 - 30 * sizeof(unsigned long));
	uMemStart = dPartitionInit(uMemStart, pMemSize / 2 - 30 * sizeof(unsigned long));
	
		
}
