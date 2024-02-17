#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "mymalloc.h"

#define MEMLENGTH 512

static double memory[MEMLENGTH];

// define pointer to memory to char* as the start of the Heap,
// so we can traverse 1 byte at a time

#define heapstart ((char*)memory)

// Create a Header struct to hold meta data and serve as a node

typedef struct node {

    int isAllocated;
    // 0: not allocated, 1: is allocated -- will be default 0 when unitialized
    
    int payloadSize;
    
    //struct node *next; Will use address + its own size + payload, to traverse
    //instead of next node
} header;

// next() function to calculate the next header - takes header pointer
header *next(header *currHeader){

    // starts at currHeader given and adds size of the header + its payload size to get
    // address right after the payload and returns it as a header pointer
    return (header *)((char*)currHeader + sizeof(header) + currHeader->payloadSize);
}

void traversal(){

    int minSize = sizeof(header) + 8;

    char *lastPossibleHeader = (char*)&memory[MEMLENGTH - 1] - minSize;  

    header *headChunk = (header *)heapstart;

    header *currChunk = headChunk;

    while((currChunk >= headChunk) && ((char*)currChunk <= lastPossibleHeader)){

        currChunk = next(currChunk);

    }
}

void *mymalloc(size_t size, char *file, int line){
    
    
    /* Defined at start of file (have to check if this is ok)
    char *heapstart = (char *) memory;
    char *byte200 = heapstart + 200; // this is just an example of moving through the heap
    */


    // Minimum size of a chunk
    int minSize = sizeof(header) + 8;

    // Last possible address that can hold a chunk (header + payload)
    char *lastPossibleHeader = (char*)&memory[MEMLENGTH - 1] - minSize;  


    // Decalare a header* that points to start of heap to check if it's initialized 
    // by checking payload size - if 0 then it isnt initialized - then proceed 
    // to initialize

    header *headChunk = (header*)heapstart;

    if(headChunk->payloadSize == 0){
    
        headChunk->isAllocated = 0;
        headChunk->payloadSize = MEMLENGTH - sizeof(header);
            
    }

    // Check to make sure a size greater than 0 is given
    // and that it is a multiple of 8 if not assign the 
    // allocation size to the smallest multiple that fits
    // request

    int allocationSize;

    if(size <= 0){
        return NULL;
    }
    else if ((size % 8) != 0){

        allocationSize = (size + 7) & ~7; 
        
    }else{

        allocationSize = size;

    }

    // Start linked list by setting a current header to the head of the heap/headChunk
    header *currChunk = headChunk;

    // Loop through headers between beginning of memory and last possible header 
    while((currChunk >= headChunk) && ((char*)currChunk <= lastPossibleHeader)){

        // Check if current chunk is NOT allocated AND the Payload size is greater or equal to size needed
        if((!currChunk->isAllocated) && currChunk->payloadSize >= allocationSize){

            header *newChunk = currChunk;
            newChunk->payloadSize = allocationSize;
            newChunk->isAllocated = 1;
            
            printf("Before Allocation - currChunk: %p, newChunk: %p\n", (void*)currChunk, (void*)newChunk);
            
            currChunk = next(newChunk);

            printf("After Allocation - currChunk: %p, newChunk: %p\n", (void*)currChunk, (void*)newChunk);

            // Check if chunk is initialized by checking payload size; 
            if(currChunk->payloadSize == 0){
                currChunk->payloadSize = (char*)&memory[MEMLENGTH - 1] - ((char *)currChunk + sizeof(header));
            }

            // Once chunk is allocated return address of payload
            return (void *)((char *)newChunk + sizeof(header));
        }
        else {

            }
                
            //also want to check if we can coalesce/give rest of memory to chunk if remaining
            //isnt enough for a new chunk


        currChunk = next(currChunk);

        }
    return NULL;
}


void myfree(void *ptr, char *file, int line){

    header *freedChunk = (header *)((char *)ptr - sizeof(header));

    freedChunk->isAllocated = 0;
}
