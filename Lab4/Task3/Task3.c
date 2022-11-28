#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct multi {
    unsigned int size;
    unsigned char* num;
};

void print_multi(struct multi* p){
    for(int i = 0; i < p->size; i++){
        printf("%x\n", p->num[i]);
    }
    printf("\n");
}

struct multi* add_multi(struct multi *p, struct multi *q){
    unsigned char carry = 0;

    struct multi* ans = malloc(sizeof(struct multi));
    ans->size = p->size + 1;
    ans->num = malloc(sizeof(unsigned char) * (4 + p->size));

    for(int i = 0; i < p->size+1; i++){
        unsigned char tmp = 0;
        if(q->size > i)
            tmp = q->num[i];

        if(p->size == i)
            ans->num[i] = carry;
        else
            ans->num[i] = tmp + p->num[i] + carry;

        if(ans->num[i] < tmp)
            carry = 1;
        else
            carry = 0;

    }

    return ans;
}

int main(int argc, char** argv){
    struct multi* ms = malloc(sizeof(struct multi)); 
    ms->size = 5;
    ms->num = malloc(sizeof(unsigned char)*5);
    memcpy(ms->num, (unsigned char[]){0xaa, 1,2,0x44,0x4f}, 5);

    struct multi* y = malloc(sizeof(struct multi)); 
    y->size = 6;
    y->num = malloc(sizeof(unsigned char)*6);
    memcpy(y->num, (unsigned char[]){0xaa, 1,2,3,0x44,0x4f}, 6);

    print_multi(add_multi(y, ms));
    return 0;
}