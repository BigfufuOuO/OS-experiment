#include "../../include/myPrintk.h"


//dPartition 是整个动态分区内存的数据结构
typedef struct dPartition{
	unsigned long size;//分区大小
	unsigned long firstFreeStart; 
} dPartition;	//共占8个字节

#define dPartition_size ((unsigned long)0x8)

void showdPartition(struct dPartition *dp){
	myPrintk(0x5,"dPartition(start=0x%x, size=0x%x, firstFreeStart=0x%x)\n", dp, dp->size,dp->firstFreeStart);
}

// EMB 是每一个block的数据结构，userdata可以暂时不用管。
typedef struct EMB{
	unsigned long size;
	union {
		unsigned long nextStart;    // if free: pointer to next block
        unsigned long userData;		// if allocated, belongs to user
	};	                           
} EMB;	//共占8个字节

#define EMB_size ((unsigned long)0x8)

void showEMB(struct EMB * emb){
	myPrintk(0x3,"EMB(start=0x%x, size=0x%x, nextStart=0x%x)\n", emb, emb->size, emb->nextStart);
}


unsigned long dPartitionInit(unsigned long start, unsigned long totalSize){
	// TODO
	/*功能：初始化内存。
	1. 在地址start处，首先是要有dPartition结构体表示整个数据结构(也即句柄)。
	2. 然后，一整块的EMB被分配（以后使用内存会逐渐拆分），在内存中紧紧跟在dP后面，然后dP的firstFreeStart指向EMB。
	3. 返回start首地址(也即句柄)。
	注意有两个地方的大小问题：
		第一个是由于内存肯定要有一个EMB和一个dPartition，totalSize肯定要比这两个加起来大。
		第二个注意EMB的size属性不是totalsize，因为dPartition和EMB自身都需要要占空间。
	
	*/
	if(totalSize < sizeof(dPartition) + sizeof(EMB))
		return 0;

	dPartition *handler = (dPartition *)start;
	handler->size = totalSize;
	handler->firstFreeStart = start + sizeof(dPartition);
	myPrintk(0x7, "handler->firstFreeStart = 0x%x\n", handler->firstFreeStart);

	EMB *block = (EMB *)handler->firstFreeStart;
	block->size = totalSize - sizeof(dPartition);
	block->nextStart = 0;
	return start;
}


void dPartitionWalkByAddr(unsigned long dp){
	// TODO
	/*功能：本函数遍历输出EMB 方便调试
	1. 先打印dP的信息，可调用上面的showdPartition。
	2. 然后按地址的大小遍历EMB，对于每一个EMB，可以调用上面的showEMB输出其信息

	*/
	showdPartition((dPartition *)dp);
	EMB *block = (EMB *)((dPartition *)dp)->firstFreeStart;
	while(block != 0){
		showEMB(block);
		block = (EMB *)block->nextStart;
	}
}


//=================firstfit, order: address, low-->high=====================
/**
 * return value: addr (without overhead, can directly used by user)
**/

unsigned long dPartitionAllocFirstFit(unsigned long dp, unsigned long size){
	// TODO
	/*功能：分配一个空间
	1. 使用firstfit的算法分配空间，
	2. 成功分配返回首地址，不成功返回0
	3. 从空闲内存块组成的链表中拿出一块供我们来分配空间(如果提供给分配空间的内存块空间大于size，我们还将把剩余部分放回链表中)，并维护相应的空闲链表以及句柄
	注意的地方：
		1.EMB类型的数据的存在本身就占用了一定的空间。

	*/
	EMB *block = (EMB *)((dPartition *)dp)->firstFreeStart;
	EMB *pre = 0;
	while(block != 0){
		if(block->size >= size + sizeof(EMB)){
			if(block->size - (size + sizeof(EMB)) >= sizeof(EMB)){
				EMB *newBlock = (EMB *)((unsigned long)block + sizeof(EMB) + size);
				newBlock->size = block->size - (size + sizeof(EMB));
				newBlock->nextStart = block->nextStart;
				if(pre == 0){
					((dPartition *)dp)->firstFreeStart = (unsigned long)newBlock;
				}
				else{
					pre->nextStart = (unsigned long)newBlock;
				}
				block->size = size + sizeof(EMB);
				//block->nextStart = (unsigned long)newBlock;
			}
			else{
				if(pre == 0){
					((dPartition *)dp)->firstFreeStart = block->nextStart;
				}
				else{
					pre->nextStart = block->nextStart;
				}
			}
			return (unsigned long)block + sizeof(EMB);
		}
		pre = block;
		block = (EMB *)block->nextStart;
	}
	return 0;//没有找到合适的空间
}


unsigned long dPartitionFreeFirstFit(unsigned long dp, unsigned long start){
	// TODO
	/*功能：释放一个空间
	1. 按照对应的fit的算法释放空间
	2. 注意检查要释放的start~end这个范围是否在dp有效分配范围内
		返回1 没问题
		返回0 error
	3. 需要考虑两个空闲且相邻的内存块的合并
	
	*/
	if(start < dp + sizeof(dPartition))	return 0;

	start -= sizeof(EMB);//back to block pointer
	EMB *block = (EMB *)((dPartition *)dp)->firstFreeStart;
	EMB *pre = 0;
	
	while(block != 0){
		if(block > (EMB *)start)	break;
		pre = block;
		block = (EMB *)block->nextStart;
	}
	if(pre == 0){
		((dPartition *)dp)->firstFreeStart = start;
		((EMB *)start)->nextStart = (unsigned long)block;
	}
	else{
		pre->nextStart = start;
		((EMB *)start)->nextStart = (unsigned long)block;
	}

	//合并
	if(block != 0 && (unsigned long)block == start + ((EMB *)start)->size){
		((EMB *)start)->size += block->size;
		((EMB *)start)->nextStart = block->nextStart;
	}
	if(pre != 0 && (unsigned long)pre + pre->size == start){
		pre->size += ((EMB *)start)->size;
		pre->nextStart = ((EMB *)start)->nextStart;
	}
	return 1;

}


// 进行封装，此处默认firstfit分配算法，当然也可以使用其他fit，不限制。
unsigned long dPartitionAlloc(unsigned long dp, unsigned long size){
	return dPartitionAllocFirstFit(dp,size);
}

unsigned long dPartitionFree(unsigned long	 dp, unsigned long start){
	return dPartitionFreeFirstFit(dp,start);
}
