#ifndef _FREESTAND_H
#define _FREESTAND_H

#include <types.h>

void set_memory(void *ptr,int value, size_t size);
int copy_memory(void *dest, void* src, size_t size);


#endif
