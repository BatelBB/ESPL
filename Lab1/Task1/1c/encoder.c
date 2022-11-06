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
    if (argc == 1)
    {
        while (input != '\n')
        {
            input = fgetc(stdin);
            if (input >= 97 && input <= 122)
            {
                input = input - 32;
            }
            fprintf(stdout, "%c", input);
        }
    }
    else if (argc == 2)
    {
        int in;
        int i = 2;
        int size = sizeof(argv[1]);
        // printf("%d", size);

        while ((input = fgetc(stdin)) != EOF)
        {

            // printf("i: %d size: %d ", i, size);
            if (i == size-2)
            {
                i = 2;
            }
            // printf("%c", argv[1][i]);
            if (argv[1][0] == '+')
            {
                in = argv[1][i] - '0';
                input = input + in;
                // printf("in case +: %c\n", argv[1][i]);
                fprintf(stdout, "%c", input);
            }
            if (argv[1][0] == '-')
            {
                in = argv[1][i] - '0';
                input = input - in;
                fprintf(stdout, "%c", input);
            }
            i++;
        printf("\n");
        }
    }
    else
    {
        return 1;
    }

    return 0;
}