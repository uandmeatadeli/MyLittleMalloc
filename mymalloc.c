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
    int available;
    int payloadSize;
    //struct node *next; Will use address + its own size + payload to traverse
} header;

header *next(header *currHeader){

    return (header *)((char*)currHeader + sizeof(header) + currHeader->payloadSize);

}

void traversal(){

    header *currHeader = (header *)heapstart;

    header *nextHeader;

    while(currHeader >= (header*)heapstart && (char *)currHeader <= (char*)&memory[MEMLENGTH-1]){
        nextHeader = next(currHeader);

        if(nextHeader->available && ((char*)nextHeader + sizeof(header) + 8) < (char*)&memory[MEMLENGTH-1]){

            currHeader = nextHeader;
            currHeader->payloadSize = nextHeader->payloadSize;
            
        } 

        currHeader = next(currHeader);
    }
}

void *mymalloc(size_t size, char *file, int line){
    
    
    /* Defined at start of file (have to check if this is ok)
    char *heapstart = (char *) memory;
    char *byte200 = heapstart + 200; // this is just an example of moving through the heap
    */


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
    /*
     * Changing initialization method

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
    */

    // Decalare a header* that points to start of heap to check if it's initialized 
    // by checking payload size - if 0 then it isnt initialized - then proceed 
    // to initialize

    header *headChunk = (header*)heapstart;

    if(headChunk->payloadSize == 0){
    
        headChunk->available = 1;
        headChunk->payloadSize = MEMLENGTH - sizeof(header);
            
    }
    

    // Start linked list by setting a current node to the head of the heap/headChunk
    header *currChunk = headChunk;
    header *nextChunk;

    while((currChunk >= headChunk) && ((char*)currChunk <= (char *)&memory[MEMLENGTH-1])){

        nextChunk = next(currChunk);
        
        if(currChunk->available && currChunk->payloadSize >= allocationSize){

            header *newChunk = currChunk;
            newChunk->payloadSize = allocationSize;
            newChunk->available = 0;
            
            if((nextChunk->available && ((char*)(nextChunk) + sizeof(header) + 8) < (char *)&memory[MEMLENGTH])){

                currChunk = nextChunk;
                currChunk->payloadSize = (char*)next(currChunk) - ((char*)currChunk + sizeof(header));
                currChunk->available = 1;

            }
            else {

            // currChunk->payloadSize = beginning of next header position (if any), minus
            // end of position of payload

                
            //also want to check if we can coalesce/give rest of memory to chunk if remaining
            //isnt enough for a new chunk

            }

            // if next available chunk is right after the newChunk then we will make currChunk the next
            // of newChunk
            

            return (void *)((char *)newChunk + sizeof(header));

        }

        currChunk = nextChunk;
    
    }
    
    return NULL;

}


void myfree(void *ptr, char *file, int line){

}
