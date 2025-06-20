#ifndef MYMALLOC_H
#define MYMALLOC_H

#include <stddef.h> // For size_t

void *MyMalloc(size_t size);
void MyFree(void *ptr);

#endif // MYMALLOC_H
