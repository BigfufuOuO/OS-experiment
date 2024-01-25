#include "../../include/mem.h"

// 这里实现之前内存管理函数的封装，只对用户提供malloc/free接口

unsigned long malloc(unsigned long size){
    // 调用实现的dPartition或者是ePartition的alloc
    return dPartitionAlloc(uMemStart,size);

}

unsigned long free(unsigned long start){
    // 调用实现的dPartition或者是ePartition的free
    return dPartitionFree(uMemStart,start);
}

unsigned long kmalloc(unsigned long size){
    // 调用实现的dPartition或者是ePartition的alloc
    return dPartitionAlloc(kMemStart,size);
}

unsigned long kfree(unsigned long start){
    // 调用实现的dPartition或者是ePartition的free
    return dPartitionFree(kMemStart,start);
}