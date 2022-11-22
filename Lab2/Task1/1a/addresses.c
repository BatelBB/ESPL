#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int addr5;
int addr6;

int foo()
{
    return -1;
}
void point_at(void *p);
void foo1();
char* g = "ll";
void foo2();


// Can you tell the location (stack, code, etc.) of each memory address?
// What can you say about the numerical values? Do they obey a particular order?
// Answer: global variables and functions are stored in 0x56..
//          local variables are stored in 0xff with on bit less then the global variable so it is higher in the stack

// Is long integer data type enough for dist (address difference) variables ? 
// No, because in 32 system the size is 4 bytes, in 64 system the size is 8 bytes

// Where is each memory address allocated and what does it have to do with the printed distance?
// Answer: dist1 is the distance between two consecutive global integers. So the distance is 4
//         dist2 is between a local and a global variable. Locals are stored higher in the stack.
//         dist3 is the distance between a global function and a global variable.

int main(int argc, char **argv)
{
    printf("- sizeof(Long) in my machine: %ld\n", sizeof(long));
    int addr2;
    int addr3;
    char *yos = "ree";
    int *addr4 = (int *)(malloc(50));
    printf("- &addr2: %p\n", &addr2);
    printf("- &addr3: %p\n", &addr3);
    printf("- foo: %p\n", &foo);
    printf("- &addr5: %p\n", &addr5);

    point_at(&addr5);

    printf("- &addr6: %p\n", &addr6);
    printf("- yos: %p\n", yos);
    printf("- gg: %p\n", &g);
    printf("- addr4: %p\n", addr4);
    printf("- &addr4: %p\n", &addr4);

    printf("- &foo1: %p\n", &foo1);
    printf("- &foo1: %p\n", &foo2);
    printf("- &foo2 - &foo1: %ld\n", &foo2 - &foo1);

    char* nn[9];

    int iarray[] = {1, 2, 3};
    char carray[] = {'a', 'b', 'c'};
    int *iarrayPtr;
    char *carrayPtr;
    iarrayPtr = iarray;
    carrayPtr = carray;
    int p[3];
    for (int i = 0; i < 3; i++)
    {
        printf("%d", *iarrayPtr);
        iarrayPtr = iarrayPtr + 1;
    }
    printf("\n");
    for (int i = 0; i < 3; i++)
    {
        printf("%c", *carrayPtr);
        carrayPtr = carrayPtr + 1;
    }
    printf("\n");
    // for (int i = 0; i < 3; i++)
    // {
    // printf("%d", p + i);
    // }
    return 0;
}

void point_at(void *p)
{
    int local;
    static int addr0 = 2;
    static int addr1;

    long dist1 = (size_t)&addr6 - (size_t)p;
    long dist2 = (size_t)&local - (size_t)p;
    long dist3 = (size_t)&foo - (size_t)p;

    printf("dist1: (size_t)&addr6 - (size_t)p: %ld\n", dist1);
    printf("dist2: (size_t)&local - (size_t)p: %ld\n", dist2);
    printf("dist3: (size_t)&foo - (size_t)p:  %ld\n", dist3);

    printf("- addr0: %p\n", &addr0);
    printf("- addr1: %p\n", &addr1);
}

void foo1()
{
    printf("foo1\n");
}

void foo2()
{
    printf("foo2\n");
}
