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
    
    //struct node *next; Will use address + its own size + payload, to traverse
    //instead of next node
} header;

#define HEADERSIZE sizeof(header)

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
    return (header *)((char*)currHeader + sizeof(header) + align(currHeader->payloadSize));
}


void *mymalloc(size_t size, char *file, int line){
    
    
    /* Defined at start of file (have to check if this is ok)
    char *heapstart = (char *) memory;
    char *byte200 = heapstart + 200; // this is just an example of moving through the heap
    */


    // Minimum size of a chunk
    int minSize = sizeof(header) + 8;



    // Decalare a header* that points to start of heap to check if it's initialized 
    // by checking payload size - if 0 then it isnt initialized - then proceed 
    // to initialize

    header *headChunk = (header*)heapstart;

    if(headChunk->payloadSize == 0){
    
        headChunk->isAllocated = 0;
        headChunk->payloadSize = MEMLENGTH * sizeof(double) - sizeof(header);
            
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

    // Loop through headers between beginning of memory and last possible header 
    while(currChunk){

        if((char *)currChunk < heapstart ||  (char*)currChunk >= (char *)&memory[MEMLENGTH - 1]){
            break;
        }


        // Check if current chunk is NOT allocated AND the Payload size is greater or equal to size needed
        if((!currChunk->isAllocated) && currChunk->payloadSize >= allocationSize){

                if(currChunk->payloadSize >= allocationSize + minSize){

                    header *newChunk = (header *)((char *)currChunk + sizeof(header) + allocationSize);
                    newChunk->payloadSize = currChunk->payloadSize - allocationSize - sizeof(header);
                    newChunk->isAllocated = 0;
                    currChunk->payloadSize = allocationSize;
                }
            
            
                currChunk->isAllocated = 1;

                return (void *)((char *)currChunk + sizeof(header));
        }
                


        currChunk = next(currChunk);

        }

    fprintf(stderr, "mymalloc error: Unable to allocate memory (%s:%d)\n", file, line); // Change 2: Added error message for unable to allocate memory
    return NULL;
}

void myfree(void *ptr, char *file, int line){


    // Check if ptr is NULL
   if (!ptr) {
        fprintf(stderr, "myfree error: NULL pointer passed for freeing (%s:%d)\n", file, line); // Change 3: Changed printf to fprintf for error messages
        return;
   }


   // Convert ptr to header pointer by subtracting header size
   header *freedChunk = (header *)((char *)ptr - sizeof(header));


   // Validate that the pointer is within the heap bounds
   /*
   if ((char *)freedChunk < heapstart || (char *)freedChunk > (char *)memory + MEMLENGTH * sizeof(double) - sizeof(header)) {
       printf("myfree error: Pointer %p not allocated by mymalloc (%s:%d)\n", ptr, file, line);
       return;
   }
*/

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


   // Attempt to coalesce with next chunk if it's free
   header *nextChunk = next(freedChunk);
   if ((char *)nextChunk <= (char *)memory + MEMLENGTH * sizeof(double) - sizeof(header) && !nextChunk->isAllocated) {
       // Merge the current chunk with the next chunk
       freedChunk->payloadSize += sizeof(header) + nextChunk->payloadSize;
   }


/*

   if ((char *)nextChunk < (char *)memory + MEMLENGTH * sizeof(double) && !nextChunk->isAllocated) { // Change 5: Corrected comparison for heap boundaries
        freedChunk->payloadSize += sizeof(header) + nextChunk->payloadSize;
    }

    */


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
       prevChunk->payloadSize += sizeof(header) + freedChunk->payloadSize;
   }


  
}

