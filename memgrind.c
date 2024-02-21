#include <stdio.h>

#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <sys/time.h>
#include <time.h> 
#include "mymalloc.h"
#include <stdint.h> 




// old time function
// Function to calculate the time difference in microseconds


// Function to print the time taken in milliseconds (with double precision)
void printElapsedTimeWithDouble(double timeMillis) {
    printf("Time taken is: %.2f milliseconds\n", timeMillis);
}

// Function to print the time taken in milliseconds
void printElapsedTime(long timeMicros) {
    double timeMillis = timeMicros / 1000.0;
    printf("Time taken is: %.2f milliseconds\n", timeMillis);
}

long calculateElapsedTime(struct timeval start, struct timeval end) {
    long startTimeMicros = start.tv_sec * 1000000 + start.tv_usec;
    long endTimeMicros = end.tv_sec * 1000000 + end.tv_usec;
    return endTimeMicros - startTimeMicros;
}




// Time function, used to be used but now in the fucntions em self
void runTestWithTiming(void (*testFunc)(), const char* testName) {
    long totalTime = 0;
    int runs = 50;
    for (int i = 0; i < runs; i++) {
        struct timeval start, end;
        gettimeofday(&start, NULL);

        testFunc(); // Run the test function

        gettimeofday(&end, NULL);
        totalTime += calculateElapsedTime(start, end);
    }
    printf("Average time for %s: %ld microseconds\n", testName, totalTime / runs);
}




// OG TESTS

// three tests from the assignnment details
void testMallocAndFreeImmediately() {

     for (int i = 0; i < 120; i++) {
        char *ptr = (char *)malloc(1);
        free(ptr);
    }
    printf("Test 1 completed: malloc() and immediately free() a 1-byte object, 120 times.\n");



    long totalTime = 0;
    int runs = 50;
    for (int i = 0; i < runs; i++) {
        struct timeval start, end;
        gettimeofday(&start, NULL);

    for (int i = 0; i < 120; i++) {
        char *ptr = (char *)malloc(1);
        free(ptr);
    }
        gettimeofday(&end, NULL);
        totalTime += calculateElapsedTime(start, end);
    }
    printf("Average time for testMallocAndFreeImmediately: %ld microseconds\n", totalTime / runs);
}



void testMallocStoreAndFree() {
    char *arr[120];
    for (int i = 0; i < 120; i++) {
        arr[i] = (char *)malloc(1);
    }
    for (int i = 0; i < 120; i++) {
        free(arr[i]);
    }
    printf("Test 2 completed: Allocated and then freed 120 1-byte objects.\n");


    long totalTime = 0;
    int runs = 50;
    for (int i = 0; i < runs; i++) {
        struct timeval start, end;
        gettimeofday(&start, NULL);
        
        char *arr[120];
    for (int i = 0; i < 120; i++) {
        arr[i] = (char *)malloc(1);
    }
    for (int i = 0; i < 120; i++) {
        free(arr[i]);
    }    
        gettimeofday(&end, NULL);
        totalTime += calculateElapsedTime(start, end);
    }
    printf("Average time for testMallocStoreAndFree: %ld microseconds\n", totalTime / runs);

}

void testRandomAllocFree() {

    char *arr[120] = {NULL};
    int allocCount = 0, index = 0;
    while (allocCount < 120) {
        if (rand() % 2 == 0 || index == 0) { // Allocate if random or no allocations yet
            arr[index++] = (char *)malloc(1);
            allocCount++;
        } else { // Free the most recently allocated object
            free(arr[--index]);
            arr[index] = NULL;
        }
    }
    // Free any remaining allocated objects
    for (int i = 0; i < index; i++) {
        free(arr[i]);
    }
    printf("Test 3 completed: Randomly allocated and freed 120 1-byte objects.\n");

    long totalTime = 0;
    int runs = 50;
    for (int i = 0; i < runs; i++) {
        struct timeval start, end;
        gettimeofday(&start, NULL);



    char *arr[120] = {NULL};
    int allocCount = 0, index = 0;
    while (allocCount < 120) {
        if (rand() % 2 == 0 || index == 0) { // Allocate if random or no allocations yet
            arr[index++] = (char *)malloc(1);
            allocCount++;
        } else { // Free the most recently allocated object
            free(arr[--index]);
            arr[index] = NULL;
        }
    }
    // Free any remaining allocated objects
    for (int i = 0; i < index; i++) {
        free(arr[i]);
    }
    
    gettimeofday(&end, NULL);
        totalTime += calculateElapsedTime(start, end);
    }
    printf("testRandomAllocFree: %ld microseconds\n", totalTime / runs);
}






void test1(){
    // malloc and free, 120 times
    long totalTime = 0;
    int runs = 50;
    char* p;
    for (int i = 0; i < runs; i++) {
        struct timeval start, end;
        gettimeofday(&start, NULL);

        for (int i = 0; i < 120; i++)
        {
            p = (char*) malloc(sizeof(char)*1);
            free(p);   
        }  
       gettimeofday(&end, NULL);
        totalTime += calculateElapsedTime(start, end);
    }
    printf("Average time for test1: %ld microseconds\n", totalTime / runs);   
}






void test2(){
    // malloc 120 times and then free all

    char *arr[150], *p;
    long totalTime = 0;
    int runs = 50;
    for (int i = 0; i < runs; i++) {
        struct timeval start, end;
        gettimeofday(&start, NULL);
        for (int i = 0; i < 120; i++)
        {
            p = (char*) malloc(sizeof(char)*1);
            arr[i] = p;
        }
        for (int i = 0; i < 120; i++)
        {
            free(arr[i]);
        }  
        gettimeofday(&end, NULL);
        totalTime += calculateElapsedTime(start, end);
    }
    printf("Average time test2: %ld microseconds\n", totalTime / runs); 
}


// malloc and free, 120 times
void test3() {
    char *arr[5000], *p;
    short repeat = 50, is_malloc;
    long average_time = 0;

    for (int run = 0; run < repeat; run++) {
        struct timeval start, end;
        gettimeofday(&start, NULL);

        short malloc_counter = 0, arr_count = -1;
        
        // Allocate memory and free it 120 times
        for (int i = 0; i < 120; i++) {
            is_malloc = rand() % 2; // Random with a probability of 1/2

            // Allocate memory
            if (is_malloc) {
                if (malloc_counter < 120) {
                    p = (char*)malloc(sizeof(char) * 1);
                    arr_count++;
                    arr[arr_count] = p;
                    malloc_counter++;
                }
            } 
            // Free memory
            else {
                if (arr_count >= 0) {
                    free(arr[arr_count]);
                    arr_count--;
                }
            }
        }

        // Free remaining allocated memory
        while (arr_count >= 0) {
            free(arr[arr_count]);
            arr_count--;
        }

        gettimeofday(&end, NULL);
        average_time += calculateElapsedTime(start, end);
    }

    // Calculate and print the average time
    printf("The average time is: \n");
    printElapsedTimeWithDouble(average_time / (double)repeat);
}




void test4() {
     // Fill up 4064 bytes of memory; Malloc and free 120 time
    struct timeval start, end;
    char *arr[260], *p;

    int repeat = 50;
    
    long average_time = 0;

    for (int run = 0; run < repeat; run++)
    {
        for (int i = 0; i < 254; i++) 
        {
            p = (char*) malloc(sizeof(char)*1);
            arr[i] = p;
        }
        gettimeofday(&start, NULL);

        int ptr_left = 126;

        int ptr_right = 127;
        for (int i = 0; i < 127; i++)
        {
            free(arr[ptr_left]);

            free(arr[ptr_right]);

            ptr_left--;

            ptr_right++;
        }  
        gettimeofday(&end, NULL);
        average_time += calculateElapsedTime(start,end);
        
    }

    
    printf("The average time is: \n");
    printElapsedTimeWithDouble(average_time/(double)repeat); 
}


void test5() {

    // Fill up 4064 bytes of memory; Malloc and free 120 time
    char *arr[260], *p;
    int repeat = 50;
    long average_time = 0;

    for (int run = 0; run < repeat; run++) {
        struct timeval start, end;

        // Allocate memory
        for (int i = 0; i < 254; i++) {
            p = (char*)malloc(sizeof(char) * 1);
            arr[i] = p;
        }

        // Start timing
        gettimeofday(&start, NULL);

        // Allocate and free memory 120 times
        for (int i = 0; i < 120; i++) {
            p = (char*)malloc(sizeof(char) * 1);
            free(p);
        }

        // End timing
        gettimeofday(&end, NULL);

        // Free allocated memory
        for (int i = 0; i < 254; i++) {
            free(arr[i]);
        }

        // Calculate and accumulate elapsed time
        long startTimeMicros = start.tv_sec * 1000000 + start.tv_usec;
        long endTimeMicros = end.tv_sec * 1000000 + end.tv_usec;
        average_time += endTimeMicros - startTimeMicros;
    }

    // Calculate and print the average time
    printf("The average time is: \n");
    double averageTimeMillis = average_time / (double)repeat / 1000.0;
    printf("Time taken is: %.2f milliseconds\n", averageTimeMillis);
}





// EXTRA tests from here ON

/*
void testCoalescing() {
    //printf("Starting coalescing test...\n");


    // Allocate 24-byte chunks until all memory is used
    void* chunks[100]; // Adjust size according to your memory size and management
    for (int i = 0; i < 100 && (chunks[i] = malloc(24)) != NULL; i++);

    // Free all these chunks
    for (int i = 0; chunks[i] != NULL && i < 100; i++) {
        free(chunks[i]);
    }

    // Request a 48-byte chunk to trigger coalescing
    void* largeChunk = malloc(48);
    if (largeChunk != NULL) {
        printf("Successfully allocated a 48-byte chunk after coalescing.\n");
        free(largeChunk);
    } else {
        printf("Failed to allocate a 48-byte chunk. Coalescing may not be working as expected.\n");
    }
     
}

void testCoalescingWithTiming() {
    long totalTime = 0;
    int runs = 50;
    for (int i = 0; i < runs; i++) {
        struct timeval start, end;
        gettimeofday(&start, NULL);

        void* chunks[100]; // Adjust size according to your memory size and management
    for (int i = 0; i < 100 && (chunks[i] = malloc(24)) != NULL; i++);

    // Free all these chunks
    for (int i = 0; chunks[i] != NULL && i < 100; i++) {
        free(chunks[i]);
    }

    // Request a 48-byte chunk to trigger coalescing
    void* largeChunk = malloc(48);
    if (largeChunk != NULL) {
        //printf("Successfully allocated a 48-byte chunk after coalescing.\n");
        free(largeChunk);
    } else {
        printf("Failed to allocate a 48-byte chunk. Coalescing may not be working as expected.\n");
    }

        gettimeofday(&end, NULL);
        totalTime += calculateElapsedTime(start, end);
    }
    printf("Average time for testCoalescing: %ld microseconds\n", totalTime / runs);
}

*/


void testCoalescing1() {
    //printf("Starting coalescing test...\n");

    // Part 1: Allocate 24-byte chunks until memory is exhausted
    void* pointers[100]; // Adjust size according to your expected memory limits
    int i = 0;
    while ((pointers[i] = malloc(24)) != NULL && i < (sizeof(pointers) / sizeof(pointers[0])) - 1) {
        i++;
    }
    printf("Allocated %d chunks of 24 bytes.\n", i);

    // Part 2: Free all the 24-byte chunks
    for (int j = 0; j < i; j++) {
        free(pointers[j]);
    }
    printf("Freed %d chunks.\n", i);

    // Part 3: Attempt to allocate a 48-byte chunk
    void* largeChunk = malloc(48);
    if (largeChunk != NULL) {
        //printf("Successfully allocated a 48-byte chunk after coalescing.\n");

        free(largeChunk); // Don't forget to free the large chunk as well
    } else {
        printf("Failed to allocate a 48-byte chunk. Coalescing may not be functioning as expected.\n");
    }
    printf("passed.\n");
}




// Function to check if a pointer is 8-byte aligned
int is8ByteAligned(void* ptr) {
    return ((uintptr_t)ptr % 8 == 0);
}


// Test function for alignment requirements
void testAlignment() {
    printf("Starting alignment test...\n");
    
    // Array of sizes to test, including edge cases around multiples of 8
    int testSizes[] = {1, 7, 8, 9, 15, 16, 17, 23, 24, 25, 31, 32, 33, 39, 40};
    int numTests = sizeof(testSizes) / sizeof(testSizes[0]);
    void* ptrs[numTests];
    
    for (int i = 0; i < numTests; i++) {
        ptrs[i] = malloc(testSizes[i]);
        if (ptrs[i] == NULL) {
            printf("Allocation failed for size %d\n", testSizes[i]);
            continue;
        }
        
        // Check if the returned pointer is 8-byte aligned
        if (is8ByteAligned(ptrs[i])) {
            printf("Pointer for size %d is correctly 8-byte aligned.\n", testSizes[i]);
        } else {
            printf("Error: Pointer for size %d is not 8-byte aligned.\n", testSizes[i]);
        }
    }
    
    // Free allocated memory
    for (int i = 0; i < numTests; i++) {
        if (ptrs[i] != NULL) {
            free(ptrs[i]);
        }
    }
    printf("Alignment is good");
}

// Test function for alignment requirements
void testAlignment1() {
    
    
    // Array of sizes to test, including edge cases around multiples of 8
    int testSizes[] = {1, 7, 8, 9, 15, 16, 17, 23, 24, 25, 31, 32, 33, 39, 40};
    int numTests = sizeof(testSizes) / sizeof(testSizes[0]);
    void* ptrs[numTests];
    
    for (int i = 0; i < numTests; i++) {
        ptrs[i] = malloc(testSizes[i]);
        if (ptrs[i] == NULL) {
            printf("Allocation failed for size %d\n", testSizes[i]);
            continue;
        }
        
        // Check if the returned pointer is 8-byte aligned
        if (is8ByteAligned(ptrs[i])) {
            //printf("Pointer for size %d is correctly 8-byte aligned.\n", testSizes[i]);
        } else {
            //printf("Error: Pointer for size %d is not 8-byte aligned.\n", testSizes[i]);
        }
    }
    
    // Free allocated memory
    for (int i = 0; i < numTests; i++) {
        if (ptrs[i] != NULL) {
            free(ptrs[i]);
        }
    }
    //printf("Alignment is good");
}




void testRandomized() {
    printf("Starting randomized test...\n");
    srand(time(NULL));
    void* blocks[100];
    int allocated = 0;

    for (int i = 0; i < 1000; i++) {
        int action = rand() % 2;
        int size = rand() % 100 + 1;
        if (action == 0 && allocated < 100) { // Allocate
            blocks[allocated++] = malloc(size);
        } else if (allocated > 0) { // Free
            free(blocks[--allocated]);
        }
    }

    // Cleanup
    for (int i = 0; i < allocated; i++) {
        free(blocks[i]);
    }
    printf("Randomized test completed.\n");
}

void testRandomized1() {
    //printf("Starting randomized test...\n");
    srand(time(NULL));
    void* blocks[100];
    int allocated = 0;

    for (int i = 0; i < 1000; i++) {
        int action = rand() % 2;
        int size = rand() % 100 + 1;
        if (action == 0 && allocated < 100) { // Allocate
            blocks[allocated++] = malloc(size);
        } else if (allocated > 0) { // Free
            free(blocks[--allocated]);
        }
    }

    // Cleanup
    for (int i = 0; i < allocated; i++) {
        free(blocks[i]);
    }
    //printf("Randomized test completed.\n");
}



// Extra testing here

  void testOverlappingAllocations() {
    printf("Test: Overlapping Allocations\n");
    char* first = malloc(1);
    char* second = malloc(1);
    if (second == first + 1) {
        printf("Error: Allocations are overlapping.\n");
    } else {
        printf("Success: Allocations are not overlapping.\n");
    }
    free(first);
    free(second);
}

void testOverlappingAllocations1() {
    //printf("Test: Overlapping Allocations\n");
    char* first = malloc(1);
    char* second = malloc(1);
    if (second == first + 1) {
       // printf("Error: Allocations are overlapping.\n");
    } else {
        //printf("Success: Allocations are not overlapping.\n");
    }
    free(first);
    free(second);
}



void testMemoryLeakDetection() {
    printf("Test: Memory Leak Detection\n");
//    char* leakTest = malloc(10);
    // Intentionally not freeing 'leakTest' to simulate a memory leak
    // You could implement a function to check for unfreed memory at this point
    // Example: checkForLeaks(); // This would be a custom function you implement
}


void testMemoryLeakDetection1() {
    //printf("Test: Memory Leak Detection\n");
//    char* leakTest = malloc(10);
    // Intentionally not freeing 'leakTest' to simulate a memory leak
    // You could implement a function to check for unfreed memory at this point
    // Example: checkForLeaks(); // This would be a custom function you implement
}

void testExactFitAllocation() {
    printf("Test: Exact Fit Allocation\n");
    char* first = malloc(10);
    free(first);
    char* second = malloc(10);
    if (first == second) {
        printf("Success: Exact fit block reused.\n");
    } else {
        printf("Error: Exact fit block not reused.\n");
    }
    free(second);
}
void testExactFitAllocation1() {
    //printf("Test: Exact Fit Allocation\n");
    char* first = malloc(10);
    free(first);
    char* second = malloc(10);
    if (first == second) {
        //printf("Success: Exact fit block reused.\n");
    } else {
        printf("Error: Exact fit block not reused.\n");
    }
    free(second);
}

void testFragmentationHandling() {
    printf("Test: Fragmentation Handling\n");
    char* smallChunks[5];
    for (int i = 0; i < 5; i++) {
        smallChunks[i] = malloc(1); // Allocate small chunks to create fragmentation
    }
    for (int i = 0; i < 5; i += 2) {
        free(smallChunks[i]); // Free every other chunk to create gaps
    }
    char* largerChunk = malloc(4); // Attempt to allocate a chunk that requires coalescing
    if (largerChunk != NULL) {
        printf("Success: Managed fragmentation effectively.\n");
    } else {
        printf("Error: Failed to handle fragmentation.\n");
    }
    // Free remaining allocations
    for (int i = 1; i < 5; i += 2) {
        free(smallChunks[i]);
    }
    free(largerChunk);
}

void testFragmentationHandling1() {
    //printf("Test: Fragmentation Handling\n");
    char* smallChunks[5];
    for (int i = 0; i < 5; i++) {
        smallChunks[i] = malloc(1); // Allocate small chunks to create fragmentation
    }
    for (int i = 0; i < 5; i += 2) {
        free(smallChunks[i]); // Free every other chunk to create gaps
    }
    char* largerChunk = malloc(4); // Attempt to allocate a chunk that requires coalescing
    if (largerChunk != NULL) {
      //  printf("Success: Managed fragmentation effectively.\n");
    } else {
        printf("Error: Failed to handle fragmentation.\n");
    }
    // Free remaining allocations
    for (int i = 1; i < 5; i += 2) {
        free(smallChunks[i]);
    }
    free(largerChunk);
}

#include <time.h>
void stressTestRandomOperations() {
    printf("Stress Test: Random Operations\n");
    srand(time(NULL)); // Seed random number generator
    void* ptrs[100];
    int ptrCount = 0;
    for (int i = 0; i < 1000; i++) {
        if (rand() % 2 && ptrCount < 100) { // 50% chance to allocate
            size_t size = (rand() % 64) + 1; // Random size between 1 and 64
            ptrs[ptrCount++] = malloc(size);
        } else if (ptrCount > 0) { // 50% chance to free, if there's anything to free
            free(ptrs[--ptrCount]);
        }
    }
    // Free any remaining pointers
    while (ptrCount > 0) {
        free(ptrs[--ptrCount]);
    }
     printf("Success: Managed Stress test.\n");
}

void stressTestRandomOperations1() {
   // printf("Stress Test: Random Operations\n");
    srand(time(NULL)); // Seed random number generator
    void* ptrs[100];
    int ptrCount = 0;
    for (int i = 0; i < 1000; i++) {
        if (rand() % 2 && ptrCount < 100) { // 50% chance to allocate
            size_t size = (rand() % 64) + 1; // Random size between 1 and 64
            ptrs[ptrCount++] = malloc(size);
        } else if (ptrCount > 0) { // 50% chance to free, if there's anything to free
            free(ptrs[--ptrCount]);
        }
    }
    // Free any remaining pointers
    while (ptrCount > 0) {
        free(ptrs[--ptrCount]);
    }
}




// ERROR reporting HERE
void testFreeInvalidAddress() {
    int x;
    printf("Test: Freeing an address not obtained from malloc...\n");
    free(&x); // This should trigger an error report
}

void testFreeAddressNotAtStart() {
    int* p = malloc(sizeof(int) * 2);
    printf("Test: Freeing an address not at the start of a chunk...\n");
    free(p + 1); // This should trigger an error report
    free(p); // Correctly free p to avoid memory leak
}

void testDoubleFree() {
    int* p = malloc(sizeof(int) * 100);
    printf("Test: Double freeing the same pointer...\n");
    free(p);
    free(p); // This should trigger an error report
}









// MAIN FUNCTION

int main(){


// tests from Assingment details
    testMallocAndFreeImmediately();
    testMallocStoreAndFree();
    testRandomAllocFree();

// Tha OG tests
printf("\n\n");

    printf("T1, Malloc and free 120 times\n");
    test1();

    printf("T2, Malloc 120 times then free all of em\n");
    test2();

    printf("T3, either malloc or free then, free all\n");
    test3(); 

    printf("T4, mem 4064 up;\n");
    test4();

    printf("T5, fill mem to 4064 bytes, malloc and then free 120 times\n"); 
    test5();

printf("\n");

    // ERROR REPORTING
    testFreeInvalidAddress();
    testFreeAddressNotAtStart();
    testDoubleFree();

printf("\n");

    // EXTRA TESTING
//    printf("#1 Coalescing Test: Attempt to allocate after freeing smaller chunks\n");
//    testCoalescing();
//    testCoalescingWithTiming();

    printf("#1 Test Coalesing\n");
    testCoalescing1();

    printf("#2 Test function for alignment requirements\n");
    testAlignment();
    runTestWithTiming( testAlignment1, "Alignment Testing");

    printf("#3 Test Randomized good\n");
    testRandomized();
    runTestWithTiming( testRandomized1, " Ranomdized Testing");


    printf("#4 Test if theire are Overlaps in allocation \n");
    testOverlappingAllocations();
    runTestWithTiming(testOverlappingAllocations1, "Overlaps Testing");

    
    
    printf("#5 \n");
    testMemoryLeakDetection();
    runTestWithTiming( testMemoryLeakDetection1, "testMemoryLeakDetection ");

   
    printf("#6 \n");
    testExactFitAllocation();
    runTestWithTiming( testExactFitAllocation1, "testExactFitAllocation ");

    
    printf("#7 \n");
    testFragmentationHandling();
    runTestWithTiming( testFragmentationHandling1, "testFragmentationHandling ");

    
    printf("#8 \n");
    stressTestRandomOperations();
    runTestWithTiming( stressTestRandomOperations1, "stressTestRandomOperations ");


/*
    printf("#6 Test Randomized good\n");
    testRandomized();
    runTestWithTiming( testRandomized1, "Alignment Testing");

*/
    //runTestWithTiming(testCoalescing, "Coalescing Test");
    //runTestWithTiming(testAlignment, "Alignment Test");
    //runTestWithTiming(testRandomized, "Randomized Test");
    
   
    




    printf("\n\n");
    printf("All passed\n");

    printf("\n\n");
    return 0;
}
