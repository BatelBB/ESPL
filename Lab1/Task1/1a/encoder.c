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

    FILE *out = stdout;
    FILE *in = stdin;

    char input;

        while (input != '\n')
        {
            input = fgetc(in);
            if (input >= 97 && input <= 122)
            {
                input = input - 32;
            }
            fprintf(out, "%c", input);
        }

    return 0;
}