#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "mymalloc.h"
#include <stdbool.h>

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
    
    // Will use address + its own size + payload, to traverse
    // instead of next node
} header;

//define a HEADERSIZE which the size of our header struct
#define HEADERSIZE sizeof(header)

//Align function takes size and checks if it is a multiple of 8
//if not it will make it a multiple of 8 and return that aligned size
static int align(int size) {

    int allocationSize;

    if ((size % 8) != 0){

        allocationSize = (size + 7) & ~7; 
        
    }else{

        allocationSize = size;

    }
    
    return allocationSize;
}

// next() function to calculate the next header - takes header pointer
static header *next(header *currHeader){
 
    if(currHeader->payloadSize == 0) return NULL;

    // starts at currHeader given and adds size of the header + its payload size to get
    // address right after the payload and returns it as a header pointer
    return (header *)((char*)currHeader + HEADERSIZE + align(currHeader->payloadSize));
}


void *mymalloc(size_t size, char *file, int line){
    
    
    // Minimum size of a chunk
    int minChunkSize = HEADERSIZE + 8;

    // Decalare a header* that points to start of heap to check if it's initialized 
    // by checking payload size - if 0 then it isnt initialized - then proceed 
    // to initialize

    header *headChunk = (header*)heapstart;

    if(headChunk->payloadSize == 0){
    
        headChunk->isAllocated = 0;
        headChunk->payloadSize = MEMLENGTH * sizeof(double) - HEADERSIZE;
            
    }

    // Check to make sure a size greater than 0 is given
    // and that it is a multiple of 8 if not assign the 
    // allocation size to the smallest multiple that fits
    // request

    int allocationSize;

    if(size <= 0){
        fprintf(stderr, "mymalloc error: Invalid size (%s:%d)\n", file, line); // Change 1: Added error message for invalid size
        return NULL;
    }
    else{
        allocationSize = align(size);
    }

    // Start linked list by setting a current header to the head of the heap/headChunk
    header *currChunk = headChunk;

    // Loop through memory while currchunk is not null 
    while(currChunk){

        // Break if we are out of the bounds of our memory
        if((char *)currChunk < heapstart ||  (char*)currChunk >= (char *)&memory[MEMLENGTH - 1]){
            break;
        }


        // Check that currChunk is not allocated andn that the payload size can fit the requested size
        if((!currChunk->isAllocated) && currChunk->payloadSize >= allocationSize){

            // Check if we can fit another chunk after payload size being requested 
            if(currChunk->payloadSize >= allocationSize + minChunkSize){

                // New Chunk is set to the chunk after the one being requested and set to NOT allocated 
                // and currChunk becomes the requested chunk with reuqested allocation size
                header *newChunk = (header *)((char *)currChunk + HEADERSIZE + allocationSize);
                newChunk->payloadSize = currChunk->payloadSize - allocationSize - HEADERSIZE;
                newChunk->isAllocated = 0;
                currChunk->payloadSize = allocationSize;
            }

            // Set Current Chunk to allocated
            currChunk->isAllocated = 1;

            //return pointer to start of payload requested
            return (void *)((char *)currChunk + HEADERSIZE);
        }

        // Set current chunk to the the next chunk to move through memory
        currChunk = next(currChunk);

    }

    // Not enough memory to allocate return NULL and print error
    fprintf(stderr, "mymalloc error: Unable to allocate memory (%s:%d)\n", file, line);
    return NULL;
}

void myfree(void *ptr, char *file, int line){


    // Check if ptr is NULL
   if (!ptr) {
        fprintf(stderr, "myfree error: NULL pointer passed for freeing (%s:%d)\n", file, line); // Change 3: Changed printf to fprintf for error messages
        return;
   }

   // Convert ptr to header pointer by subtracting header size
   header *freedChunk = (header *)((char *)ptr - HEADERSIZE);


   if ((char *)freedChunk < heapstart || (char *)freedChunk >= (char *)memory + MEMLENGTH * sizeof(double)) { // Change 4: Corrected comparison for heap boundaries
        fprintf(stderr, "myfree error: Pointer %p not allocated by mymalloc (%s:%d)\n", ptr, file, line);
        return;
    }

    header *current = (header *)heapstart;
    bool validFree = false;

    while (current && (char *)current < (char *)memory + MEMLENGTH * sizeof(double)) {
        if (current == freedChunk && current->isAllocated) {
            validFree = true;
            break;
        }
        current = next(current);
    }

    if (!validFree) {
        fprintf(stderr, "myfree error: Pointer %p does not match the start of any allocated chunk (%s:%d)\n", ptr, file, line);
        return;
    }


   // Check if the chunk is already free
   if (!freedChunk->isAllocated) {
       fprintf(stderr, "myfree error: Double free detected (%s:%d)\n", file, line);
        return;
   }


   // Mark the chunk as free
   freedChunk->isAllocated = 0;

   // Attempt to coalesce with previous chunk if it's free
   // This requires a traversal from the start of the heap since we don't have back pointers
   header *currChunk = (header *)heapstart;
   header *prevChunk = NULL;
   while ((char *)currChunk < (char *)freedChunk) {
       prevChunk = currChunk;
       currChunk = next(currChunk);
   }
   if (prevChunk && !prevChunk->isAllocated) {
       // Merge the previous chunk with the freed chunk
       prevChunk->payloadSize += HEADERSIZE + freedChunk->payloadSize;
       freedChunk = prevChunk;  // Update freedChunk to point to the coalesced block
   }

   // Attempt to coalesce with next chunk if it's free
   header *nextChunk = next(freedChunk);
   if ((char *)nextChunk <= (char *)memory + MEMLENGTH * sizeof(double) - HEADERSIZE && !nextChunk->isAllocated) {
       // Merge the current chunk with the next chunk
       freedChunk->payloadSize += HEADERSIZE + nextChunk->payloadSize;
   }

}

