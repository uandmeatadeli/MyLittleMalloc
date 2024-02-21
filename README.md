# MyLittleMalloc
## CS: 214 Project 1: My Little Malloc

**Jack Arteaga - ja1327**

**Pranav Chundi - prc83**

## Project Overview
This project implements custom versions of the `malloc()` and `free()` functions to manage dynamic memory allocation in C. It aims to provide a deeper understanding of memory management and to introduce error detection mechanisms not present in the standard library functions.

## MetaData
We chose a struct called `header` that holds two ints to preserve memory for our payload. We use the address of the header + its own size to find the payload address. From here we can add the payload's size to its own address to get to the next Header. This allowed us to traverse memory without using explicit nodes. The two ints in our structure are as follows:

**Header Structure (`header`):**
- `isAllocated`: An integer flag (0 or 1) indicating whether the associated memory chunk is currently allocated (1) or not (0).
- `payloadSize`: An integer representing the size of the actual data payload in the memory chunk.

## Functions
### `mymalloc` Function
**Responsibility:** Finds and allocates a block of memory with a specified size requested. Makes proper checks to ensure size requested is acceptable and does not exceed memory bounds. Returns a pointer to the payload of block allocated.

**Algorithm:**
1. Initializes the memory if it's not initialized.
2. Aligns the requested size to ensure it's a multiple of 8.
3. Iterates through the memory chunks to find a suitable free block.
4. If a block is found:
   - If the block is large enough, it splits the block into an allocated part and a free part (if needed).
   - Returns a pointer to the allocated memory.
5. If no suitable block is found, returns NULL and prints an error message.

### `myfree` Function
**Responsibility:** Frees a previously allocated block of memory. Makes adequate checks to ensure the pointer is acceptable and points to an allocated chunk of memory. Also makes necessary checks to find non-allocated chunks that can be coalesced to optimize memory.

**Algorithm:**
1. Checks if the given pointer is NULL and prints an error if true.
2. Converts the pointer to a header pointer to access metadata.
3. Validates that the pointer is within the allocated memory bounds.
4. Marks the associated memory chunk as unallocated.
5. Attempts to coalesce the freed chunk with adjacent free chunks (previous and next).

### `align` Function
**Responsibility:** Aligns the given size to the nearest multiple of 8.

### `next` Function
**Responsibility:** Calculates the address of the next header given a current header.

# Code Layout
- `mymalloc.c`: Contains the custom implementation of the `malloc` and `free` functions. It includes the logic for memory allocation, deallocation, error handling, and memory optimization strategies.
- `mymalloc.h`: The header file for `mymalloc.c`. It declares the functions and any macros or structures used by the memory allocation implementation.
- `memgrind.c`: Extensive test cases, alignment, error handling, coalescing test, correctness testing, performance testing.
- `memtest.c`: Testing `mymalloc`.
- `hardtesting.c`: This code tests the custom memory allocator by allocating multiple objects, filling them with distinct patterns, verifying the integrity of these patterns, and then deallocating the memory.
- `Makefile`: The makefile lol.
- `Readme`: Read it. You're reading it now!

# Installation and Running Instructions
Use the "make" command to run the makefile and compile all files:

```bash
make
```

Choose a test and run it using the following commands:

For Memgrind use: 
```bash
./memgrind
```
For Memtest use:
```bash
./memtest
```
For Hardtesting use:
```bash
./hardtesting
```
To remove files created by make:
```bash
make clean
```

## Testplan/Tests: 

## Test #1
**Function:** testMallocAndFreeImmediately  
**Description:** Allocates and immediately frees a 1-byte object, 120 times. Basic Allocation and Immediate Deallocation  
**Outcome:** Validates immediate deallocation after allocation.  
**Average Time:** 7 microseconds.

## Test #2
**Function:** testMallocStoreAndFree  
**Description:** Sequentially allocates 120 1-byte objects, then frees them. Sequential Allocation and Deallocation  
**Outcome:** Tests allocator's handling of sequential operations.  
**Average Time:** 180 microseconds.

## Test #3
**Function:** testRandomAllocFree  
**Description:** Randomly allocates and frees 120 1-byte objects. Random Allocation and Deallocation  
**Outcome:** Assesses how the allocator deals with random allocation patterns.  
**Average Time:** 84 microseconds.

## Test #4
**Function:** Test 1: Malloc and Free 120 Times (test1)  
**Objective:** To evaluate the efficiency and correctness of allocating and immediately freeing a small amount of memory (1 byte) repeatedly.  
**Steps:**
- Start timing the test.
- Repeat 120 times:
    - Allocate 1 byte of memory.
    - Immediately free the allocated memory.
- Stop timing the test and calculate the total duration.  
**Average Time:** 10 microseconds  
**Analysis:** This test demonstrates excellent performance in allocating and immediately freeing a small amount of memory. The low average time suggests that the allocator is highly efficient for small, short-lived allocations.

## Test #5
**Function:** Test 2: Malloc 120 Times Then Free All (test2)  
**Objective:** To assess the behavior of the memory allocator when handling multiple allocations followed by a batch deallocation.  
**Steps:**
- Start timing the test.
- Allocate 1 byte of memory 120 times, storing each returned pointer in an array.
- After all allocations, free each memory block in a separate loop.
- Stop timing the test and calculate the total duration.
- Report the average time taken for the entire process.  
**Average Time:** 178 microseconds.

## Test #6
**Function:** Test 3: Either Malloc or Free, Then Free All  
**Objective:** To test the allocator's ability to handle a mix of allocations and deallocations in a random order.  
**Steps:**
- Start timing the test.
- Perform 120 operations, where each operation is randomly chosen to be either an allocation of 1 byte (if not all 120 allocations have been made) or a free of the most recently allocated block (if any blocks are allocated).
- Ensure all allocated memory is freed by the end of the test.
- Stop timing the test and calculate the total duration.
- Report the average time taken for the entire process.  
**Time Taken:** 32.54 milliseconds  
**Analysis:** The significantly higher time taken for this test, expressed in milliseconds rather than microseconds, suggests that random allocation and deallocation patterns are more challenging for the allocator to manage efficiently. This scenario likely simulates real-world usage more closely and indicates areas where performance optimizations could be beneficial.

## Test #7
**Function:** Test 4: Mem 4064 Up  
**Objective:** Allocator performance test when working with larger memory, filling up to 4064 bytes of memory.  
**Steps:**
- Allocate enough 1-byte blocks to approximately fill up 4064 bytes of memory.
- Free specific blocks to simulate a particular pattern of usage.
- Measure the time taken for allocations and deallocations.
- Report the time taken and any potential issues encountered.  
**Time Taken:** 291.60 milliseconds

## Test #8
**Function:** Test 5: Fill Mem to 4064 Bytes, Malloc and Free 120 Times (test5)  
**Objective:** To evaluate how the allocator performs under conditions of high memory usage, followed by intensive allocation and deallocation operations.  
**Steps:**
- Fill up the memory with allocations to reach around 4064 bytes of used memory.
- Perform 120 malloc-free cycles of 1 byte each.
- Measure and report the time taken for the 120 cycles.
- Free all initially allocated memory.  
**Time Taken:** 0.54 milliseconds

## Test #9
**Error Reporting**
**Functions:** testFreeInvalidAddress, testFreeAddressNotAtStart, testDoubleFree  
**Description:** Checks error handling for invalid free operations and double freeing.  
**Outcomes:** Ensures error conditions are correctly identified and reported.

## Test #10
**Function:** testCoalescing  
**Description:** Allocates and then frees smaller chunks before trying to allocate a larger chunk, testing block coalescing.  
**Outcome:** Verifies that freed memory blocks are coalesced properly.  
**Average Time for Coalescing Test:** 32 microseconds.

## Test #11
**Function:** testAlignment  
**Description:** Tests that allocated memory addresses are 8-byte aligned.  
**Outcome:** Confirms proper memory alignment for various sizes.  
**Average Time for Alignment Test:** 1 microsecond.

## Test #12
**Function:** testRandomized  
**Objective:** To evaluate the allocator's behavior under randomized operations, including both allocations and deallocations, with varying sizes.  
**Procedure:**
- Initialize a random seed with the current time to ensure varied test runs.
- Allocate and free memory blocks randomly within a loop of 1000 iterations, where each iteration randomly chooses between allocation and deallocation.
- Allocation: Allocate a block of random size (1 to 100 bytes) if fewer than 100 blocks have been allocated.
- Deallocation: Free the most recently allocated block if any blocks are currently allocated.
- Perform cleanup at the end by freeing any remaining allocated blocks.  
**Outcomes:** 

## Test #13
**Function:** testOverlappingAllocations  
**Objective:** To verify that the custom memory allocator does not allocate overlapping memory blocks.  
**Procedure:**
- Allocate two consecutive memory blocks of 1 byte each.
- Compare the addresses of the two blocks to check if they are adjacent in memory.
- Assert that the second block is not immediately after the first in memory to ensure no overlap.  
**Outcomes:** Success: Allocations are not overlapping.  
**Average time for Overlaps Testing:** 0 microseconds

## Test #14
**Exact Fit Allocation**
**Function:** testExactFitAllocation  
**Description:** Tests if the allocator reuses blocks for exact fit allocations.  
**Outcome:** Validates block reuse for exact size allocations.  
**Average Time:** 1 microsecond.

## Test#15
**Function:** testFragmentationHandling  
**Description:** Tests allocator's ability to manage memory fragmentation.  
**Outcome:** Indicates effective handling of fragmentation.  
**Average Time:** 5 microseconds.

## Test#16
**Function:** stressTestRandomOperations  
**Description:** Executes a mix of random allocations and deallocations to stress-test the allocator. Random Operations Stress Test  
**Outcome:** Evaluates performance and stability under random operations.  
**Average Time:** 579 microseconds.

# Output of Memgrind.c test:

```bash
Test 1 completed: malloc() and immediately free() a 1-byte object, 120 times.
Average time for testMallocAndFreeImmediately: 2 microseconds
Test 2 completed: Allocated and then freed 120 1-byte objects.
Average time for testMallocStoreAndFree: 22 microseconds
Test 3 completed: Randomly allocated and freed 120 1-byte objects.
testRandomAllocFree: 10 microseconds


T1, Malloc and free 120 times
Average time for test1: 1 microseconds
T2, Malloc 120 times then free all of em
Average time test2: 22 microseconds
T3, either malloc or free then, free all
The average time is: 
Time taken is: 4.30 milliseconds
T4, mem 4064 up;
The average time is: 
Time taken is: 82.98 milliseconds
T5, fill mem to 4064 bytes, malloc and then free 120 times
The average time is: 
Time taken is: 0.24 milliseconds

Test: Freeing an address not obtained from malloc...
myfree error: Pointer 0x7ffd425aec30 not allocated by mymalloc (memgrind.c:744)
Test: Freeing an address not at the start of a chunk...
myfree error: Pointer 0x5560759bf18c does not match the start of any allocated chunk (memgrind.c:750)
Test: Double freeing the same pointer...
myfree error: Pointer 0x5560759bf188 does not match the start of any allocated chunk (memgrind.c:758)

#1 Test Coalesing
Allocated 99 chunks of 24 bytes.
Freed 99 chunks.
passed.
#2 Test function for alignment requirements
Starting alignment test...
Pointer for size 1 is correctly 8-byte aligned.
Pointer for size 7 is correctly 8-byte aligned.
Pointer for size 8 is correctly 8-byte aligned.
Pointer for size 9 is correctly 8-byte aligned.
Pointer for size 15 is correctly 8-byte aligned.
Pointer for size 16 is correctly 8-byte aligned.
Pointer for size 17 is correctly 8-byte aligned.
Pointer for size 23 is correctly 8-byte aligned.
Pointer for size 24 is correctly 8-byte aligned.
Pointer for size 25 is correctly 8-byte aligned.
Pointer for size 31 is correctly 8-byte aligned.
Pointer for size 32 is correctly 8-byte aligned.
Pointer for size 33 is correctly 8-byte aligned.
Pointer for size 39 is correctly 8-byte aligned.
Pointer for size 40 is correctly 8-byte aligned.
Alignment is goodAverage time for Alignment Testing: 0 microseconds
#3 Test Randomized good
Starting randomized test...
Randomized test completed.
Average time for  Ranomdized Testing: 57 microseconds
#4 Test if theire are Overlaps in allocation 
Test: Overlapping Allocations
Success: Allocations are not overlapping.
Average time for Overlaps Testing: 0 microseconds
#5 
Test: Memory Leak Detection
Average time for testMemoryLeakDetection : 0 microseconds
#6 
Test: Exact Fit Allocation
Success: Exact fit block reused.
Average time for testExactFitAllocation : 0 microseconds
#7 
Test: Fragmentation Handling
Success: Managed fragmentation effectively.
Average time for testFragmentationHandling : 0 microseconds
#8 
Stress Test: Random Operations
Success: Managed Stress test.
Average time for stressTestRandomOperations : 106 microseconds


All passed


```
# Output of Memtest.c test
```bash
0 incorrect bytes
```
# Additional test: Hardtesting.c
**Test:** Hardtesting.c  
**Description:** This code tests the custom memory allocator by allocating multiple objects, filling them with distinct patterns, verifying the integrity of these patterns, and then deallocating the memory. The key steps include:

1. **Allocation:** It dynamically allocates memory for NUM_OBJECTS objects, each of OBJECT_SIZE bytes, using the custom malloc implementation.
2. **Pattern Filling:** Each allocated block is filled with a unique character pattern ('A', 'B', 'C', ...), which serves as a distinct identifier for that memory block.
3. **Pattern Checking:** After filling, it verifies that the pattern in each block remains intact and unchanged, indicating no memory corruption or overlap.
4. **Deallocation:** Finally, it frees the allocated memory using the custom free implementation.

**Output:** 
```bash
All patterns preserved correctly.
```