#include <stdio.h>

int main(int argc, char **argv)
{
    int iarray[] = {1, 2, 3};
    char carray[] = {'a', 'b', 'c'};
    int *iarrayPtr;
    char *carrayPtr;



    for (int i = 0; i < 3; i++)
    {
        iarrayPtr = &iarray[i];
        carrayPtr = &carray[i];

        printf("- iarrayPtr: %d\t address: %p\n", *iarrayPtr, &iarrayPtr);
        printf("- carrayPtr: %c\t address: %p\n", *carrayPtr, &carrayPtr);
    }

    int *p;
    printf("- p: %d\n", p);
}
