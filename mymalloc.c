#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "mymalloc.h"

#define MEMLENGTH 512

static double memory[MEMLENGTH];

#define heapstart ((char*)memory)

void *checkAhead(struct header, size_t size){
    
}

void *mymalloc(size_t size, char *file, int line){
    
    // Cast pointer to memory to char* as the start of the Heap,
    // so we can traverse 1 byte at a time
    
    /* Defined at start of file (have to check if this is ok)
    char *heapstart = (char *) memory;
    char *byte200 = heapstart + 200; // this is just an example of moving through the heap
    */

    // Create a Header struct to hold meta data and serve as a node

    typedef struct node {
        int available;
        size_t payloadSize;
        struct node *next;
    } header;

    // Check to make sure a size greater than 0 is given
    // and that it is a multiple of 8 if not assign the 
    // allocation size to the smallest multiple that fits
    // request

    size_t allocationSize;

    if(size <= 0){
        return NULL;
    }
    else if ((size % 8) != 0){

        allocationSize = (size + 7) & ~7; 
        
    }else{

        allocationSize = size;

    }

    // Check for initialization by looping through memory array
    // if we encounter anything other than 0 we know it has been
    // initialized and can proceed

    int initialized = 0;

    for(size_t i = 0; i < MEMLENGTH; i++){
        
        if ( memory[i] != 0){
            initialized = 1;
            break;
        }
    }

    // if it is not initialized, then we initialize first Chunk of memory
    // which will be empty and it will point to the beginning of the heap
    
    header *headChunk;

    if(!initialized){
    
        headChunk = (header *)heapstart; 
        headChunk->available = 1;
        headChunk->payloadSize = MEMLENGTH - sizeof(header);
        headChunk->next = NULL;
            
    }

    // Start linked list by setting a current node to the head of the heap/headChunk
    header *currChunk = headChunk;

    while(!currChunk){
    
        if(currChunk->available && currChunk->payloadSize >= allocationSize){

            header *newChunk = currChunk;
            newChunk->payloadSize = allocationSize;
            newChunk->available = 0;
            
            currChunk = (header *)((char *)newChunk + sizeof(newChunk) + allocationSize);

            // currChunk->payloadSize = beginning of next header position (if any), minus
            // end of position of payload

            if(!currChunk){
                
                //also want to check if we can coalesce/give rest of memory to chunk if remaining
                //isnt enough for a new chunk

                currChunk->payloadSize = MEMLENGTH - ((char*)currChunk - (char*)headChunk) - sizeof(header);
                currChunk->available = 1;  
            }

            // if next available chunk is right after the newChunk then we will make currChunk the next
            // of newChunk
            
            newChunk->next = currChunk; //(have to check if possible first)
            
            // May have to introduce double linked list 


            return (void *)((char *)newChunk + sizeof(header));

        }

        currChunk = currChunk->next;
    
    }
    
    return NULL;

}


void myfree(void *ptr, char *file, int file){

}
