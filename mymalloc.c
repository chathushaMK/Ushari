#include<stdio.h>
#include "mymalloc.h"

// Define the memory pool
#define MEMORY_POOL_SIZE 25000
static char memoryPool[MEMORY_POOL_SIZE];

// Metadata for managing memory blocks
typedef struct BlockMetadata {
    size_t size;                 // Size of the memory block
    int isFree;                  // 1 if the block is free, 0 if allocated
    struct BlockMetadata *next;  // Pointer to the next block
} BlockMetadata;

// Head of the linked list for managing blocks
static BlockMetadata *head = NULL;

void *MyMalloc(size_t size) {
    if (size == 0 || size > MEMORY_POOL_SIZE) {
        return NULL; // Invalid size
    }

    if (!head) {
        // Initialize the memory pool
        head = (BlockMetadata *)memoryPool;
        head->size = MEMORY_POOL_SIZE - sizeof(BlockMetadata);
        head->isFree = 1;
        head->next = NULL;
    }

    BlockMetadata *current = head;
    while (current) {
        if (current->isFree && current->size >= size) {
            // Split the block if there's enough space for another metadata
            if (current->size >= size + sizeof(BlockMetadata)) {
                BlockMetadata *newBlock = (BlockMetadata *)((char *)current + sizeof(BlockMetadata) + size);
                newBlock->size = current->size - size - sizeof(BlockMetadata);
                newBlock->isFree = 1;
                newBlock->next = current->next;

                current->size = size;
                current->next = newBlock;
            }

            current->isFree = 0;
            return (void *)((char *)current + sizeof(BlockMetadata));
        }
        current = current->next;
    }

    return NULL; // No suitable block found
}

void MyFree(void *ptr) {
    if (!ptr || ptr < (void *)memoryPool || ptr >= (void *)(memoryPool + MEMORY_POOL_SIZE)) {
        return; // Invalid pointer
    }

    BlockMetadata *current = (BlockMetadata *)((char *)ptr - sizeof(BlockMetadata));
    current->isFree = 1;

    // Coalesce adjacent free blocks
    BlockMetadata *block = head;
    while (block) {
        if (block->isFree && block->next && block->next->isFree) {
            block->size += sizeof(BlockMetadata) + block->next->size;
            block->next = block->next->next;
        } else {
            block = block->next;
        }
    }
}
