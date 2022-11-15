#include <stdio.h>
#include "Task2c.c"
#include "base.c"

struct fun_desc
{
    char *name;
    char (*fun)(char);
};

int main(int argc, char **argv)
{
    char array[5] = "";
    char *carray = array;

    struct fun_desc menu[] = {{"Get string", my_get},
                              {"Print string", cprt},
                              {"Print hex", xprt},
                              {"Censor", censor},
                              {"Encrypt", encrypt},
                              {"Decrypt", decrypt},
                              {"Quit", quit},
                              {NULL, NULL}};

    int bound = sizeof(struct fun_desc);
    
    while (1)
    {
        fprintf(stdout, "\nPlease choose a function:\n");
        for (int i = 0; i < bound - 1; i++)
        {
            fprintf(stdout, "%d) %s\n", i, menu[i].name);
        }
        fprintf(stdout, "Option: ");


        char c = fgetc(stdin);
        int option = atoi(&c);

        if (option >= 0 && option <= 6)
        {
            fprintf(stdout, "\n\nWithin bounds\n");
            carray = map(carray, 5, menu[option].fun);
            fprintf(stdout, "\nDONE.\n");
        }
        else
        {
            fprintf(stdout, "\nNot within bounds\n");
            exit(3);
        }
    }
}