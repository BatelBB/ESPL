#include <stdio.h>
#include <string.h>
#include <stdlib.h>

struct fun_desc
{
    char *name;
    char (*fun)(char);
};

/* Gets a char c, and if the char is 'q' , ends the program with exit code 0. Otherwise returns c. */
char quit(char c){
    if(c == 'q')
        exit(0);
    else
        return c;
    
}
char *map(char *array, int array_length, char (*f)(char))
{
  char *mapped_array = (char *)(malloc(array_length * sizeof(char)));
  for(int i=0; i<array_length; i++){
    mapped_array[i] = f(array[i]);
  }
  free(array);
  return mapped_array;
}
/* Ignores c, reads and returns a character from stdin using fgetc. */
char my_get(char c)
{
    char input = fgetc(stdin);

    return input;
}

/* If c is a number between 0x20 and 0x7E, cprt prints the character of ASCII value c followed by a new line. Otherwise, cprt prints the dot ('.') character. After printing, cprt returns the value of c unchanged. */
char cprt(char c){
    if(c > ' ' && c<= '~'){
        printf("%c\n", c);
    }
    else
    {
        printf(".\n");
    }
    return c;
}

/* Gets a char c and returns its encrypted form by adding 3 to its value. If c is not between 0x20 and 0x7E it is returned unchanged */
char encrypt(char c){
    if(!(c >= ' ' && c<= '~'))
        return c;
    return c + 3;
}

/* Gets a char c and returns its decrypted form by reducing 3 to its value. If c is not between 0x20 and 0x7E it is returned unchanged */
char decrypt(char c){
    if(!(c >= ' ' && c<= '~'))
        return c;
    return c - 3;
}

/* xprt prints the value of c in a hexadecimal representation followed by a new line, and returns c unchanged. */
char xprt(char c){
    if(c > 0x20 && c< 0x7E)
        printf("%02X\n", c);
    else
        printf(".\n");
    return c;
}

char censor(char c)
{
  if (c == '!')
    return '.';
  else
    return c;
}

int main(int argc, char **argv)
{
    char *carray = (char*)(calloc(5, sizeof(char)));
    
    struct fun_desc menu[] = {{"Get string", my_get},
                              {"Print string", cprt},
                              {"Print hex", xprt},
                              {"Censor", censor},
                              {"Encrypt", encrypt},
                              {"Decrypt", decrypt},
                              {"Quit", quit},
                              {NULL, NULL}};

    int bound = sizeof(menu)/sizeof(menu[0]);
    
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

        if (option >= 0 && option <= bound)
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