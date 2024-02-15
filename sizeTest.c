#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char** argv){

    typedef struct node{
        int available;
        size_t payloadSize;
        struct node *next;
    } header;
    
    header test;

    printf("Size of Struct: %zu, int:%zu, size_t:%zu, struct node*:%zu\n", sizeof(test), sizeof(test.available), 
            sizeof(test.payloadSize), sizeof(test.next));

}
