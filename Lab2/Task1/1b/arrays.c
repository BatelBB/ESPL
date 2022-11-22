#include <stdio.h>

// What can you say about the behavior of the '+' operator? 
// Answer: The + operator is equal to ptr + 1*sizeof(ptr), moving 1 * the size of the object.
//          Arrays are sequential in the memmory address 
int main(int argc, char **argv)
{
    int iarray[3];
    float farray[3];
    double darray[3];
    char carray[3];

    printf("- &iarray[0]: %p\n", &iarray[0]);
    printf("- &iarray[1]: %p\n", &iarray[1]);
    printf("- &iarray[2]: %p\n", &iarray[2]);

    printf("\n");

    printf("- &farray[0]: %p\n", &farray[0]);
    printf("- &farray[1]: %p\n", &farray[1]);
    printf("- &farray[2]: %p\n", &farray[2]);

    printf("\n");

    printf("- &darray[0]: %p\n", &darray[0]);
    printf("- &darray[1]: %p\n", &darray[1]);
    printf("- &darray[2]: %p\n", &darray[2]);

    printf("\n");

    printf("- &carray[0]: %p\n", &carray[0]);
    printf("- &carray[1]: %p\n", &carray[1]);
    printf("- &carray[2]: %p\n", &carray[2]);

    printf("\n");

    printf("- hexa iarray: %x\n", iarray);
    printf("- hexa iarray+1: %x\n", iarray + 1);
    printf("- hexa farray: %x\n", farray);
    printf("- hexa farray+1: %x\n", farray + 1);
    printf("- hexa darray: %x\n", darray);
    printf("- hexa darray+1: %x\n", darray + 1);
    printf("- hexa carray: %x\n", carray);
    printf("- hexa carray+1: %x\n", carray + 1);

    
}