#include <stdio.h>
#include <stdlib.h>
#include "mymalloc.h" // Make sure this is included to use the custom malloc/free

#define NUM_OBJECTS 3
#define OBJECT_SIZE 1024 


void fillPattern(char* ptr, char pattern, size_t size) {
    for (size_t i = 0; i < size; i++) {
        ptr[i] = pattern;;
    }
}

int checkPattern(char* ptr, char pattern, size_t size) {
    for (size_t i = 0; i < size; i++) { 
        if (ptr[i] != pattern) {
            return 0; // Pattern mismatch
        }
    }
    return 1; // Pattern intact
}

int main() {
    char* objects[NUM_OBJECTS];
    char pattern;

    // Allocate and fill objects with distinct patterns
    for (int i = 0; i < NUM_OBJECTS; i++) {
        objects[i] = (char*)malloc(OBJECT_SIZE); // Uses mymalloc implicitly
        if (objects[i] == NULL) {
            printf("Allocation failed for object %d\n", i);
            exit(EXIT_FAILURE);
        }
        pattern = 'A' + i; // Generate a distinct pattern for each object
        fillPattern(objects[i], pattern, OBJECT_SIZE);
    }

    // Verify patterns
    int allCorrect = 1;
    for (int i = 0; i < NUM_OBJECTS; i++) {
        pattern = 'A' + i;
        if (!checkPattern(objects[i], pattern, OBJECT_SIZE)) {
            printf("Pattern mismatch detected in object %d\n", i);
            allCorrect = 0;
        }
        free(objects[i]); // Uses myfree implicitly
    }

    if (allCorrect) {
        printf("All patterns preserved correctly.\n");
    }

    return EXIT_SUCCESS;
}
