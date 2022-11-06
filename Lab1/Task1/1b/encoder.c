//Copied
#include <stdio.h>
#include <string.h>
//Done Copy

/**
 * Name:        encoder - encodes the input text as uppercase letters.
 * Description: encoder reads the characters from standard input 
 *              and prints the corresponding uppercase characters to the standard output. 
 *              Non lowercase characters remain unchanged.
*/
int main(int argc, char **argv)
{
    char input;
    if (argc == 2 && strcmp(argv[1], "-D") == 0)
    {
        while (input != '\n')
        {
            input = fgetc(stdin);
            if ((input >= 65 && input <= 122) || (input >= 32 && input <= 47))
            {
                fprintf(stderr, "%x ", input);
                if (!((input >= 32 && input <= 47) || (input >= 65 && input <= 96)))
                    input = input - 32;
                fprintf(stderr, "%x\n", input);
                fprintf(stdout, "%c", input);
            }
        }
        printf("\n");
    }

    while (input != '\n')
    {
        input = fgetc(stdin);
        if (input >= 97 && input <= 122)
        {
            input = input - 32;
        }
        fprintf(stdout, "%c", input);
    }

    return 0;
}
