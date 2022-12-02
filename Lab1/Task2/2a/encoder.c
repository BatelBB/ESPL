//Copied
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
//Done Copy

int main(int argc, char **argv)
{
    /**************
     * If there are not arguments - error
     * ****************/
    if (argc == 1)
    {
        fprintf(stderr, "%s", "Missing arguments\n");
    }
    /***********************
     * If there is only one argument - make upper case
     * ****************/
    else if (argc == 2)
    {
        FILE *fp;

        fp = fopen(argv[1] + 2, "r");
        if (fp == NULL)
        {
            fprintf(stderr, "%s", "Error opening file\n");
            exit(0);
        }
        char input;
        // printf("%d", size);

        while ((input = fgetc(fp)) != EOF)
        {
            if (input >= 97 && input <= 122)
            {
                input = input - 32;
            }
            fprintf(stdout, "%c", input);
        }
        fclose(fp);
        printf("\n");
    }

    /******************
     * If there are two arguments - make encoder.
     * ****************/
    else if (argc == 3)
    {
        /*****************
         * If the -e{key} is the first argument
         * ****************/
        if (argv[1][1] == 'e')
        {
            //encoder
            //argv[2][1] == i
            FILE *fp;

            fp = fopen(argv[2] + 2, "r");
            if (fp == NULL)
            {
                fprintf(stderr, "%s", "Error opening file\n");
                exit(0);
            }
            int in;
            int i = 2;
            int size = sizeof(argv[1]);
            char input;
            // printf("%d", size);

            while ((input = fgetc(fp)) != EOF)
            {

                // printf("i: %d size: %d ", i, size);
                if (i == size - 2)
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
            }
            fclose(fp);
        }

        /**************
         * If the -iinput is the first argument
         * ****************/
        if (argv[1][1] == 'i')
        {
            //read file
            //argv[2] - encoder

            FILE *fp;

            fp = fopen(argv[1] + 2, "r");
            if (fp == NULL)
            {
                fprintf(stderr, "%s", "Error opening file\n");
                exit(0);
            }

            int in;
            int i = 2;
            int size = sizeof(argv[2]);
            char input;
            // printf("%d", size);

            while ((input = fgetc(fp)) != EOF)
            {

                // printf("i: %d size: %d ", i, size);
                if (i == size - 2)
                {
                    i = 2;
                }
                // printf("%c", argv[1][i]);
                if (argv[2][0] == '+')
                {
                    in = argv[2][i] - '0';
                    input = input + in;
                    // printf("in case +: %c\n", argv[1][i]);
                    fprintf(stdout, "%c", input);
                }
                if (argv[2][0] == '-')
                {
                    in = argv[2][i] - '0';
                    input = input - in;
                    fprintf(stdout, "%c", input);
                }
                i++;
            }
            fclose(fp);
        }

        printf("\n");
    }

    return 0;
}
