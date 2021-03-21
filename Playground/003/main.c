#include <stdio.h>

typedef struct {
    int var1;
    int var2;
}dummy_struct_t;


typedef void (* handler_t)(dummy_struct_t *);

handler_t array[5];


void callback(dummy_struct_t *reg)
{
    printf("works\n");
}

void register_shit(int index, handler_t handler){
    array[index] = handler;
}

void handler(dummy_struct_t *reg)
{
    handler_t handle = array[reg->var1];
    handle(reg);
}

int main()
{
    dummy_struct_t *temp;
    temp->var1 = 1;
    temp->var2 = 2;

    handler(temp);

    return 0;
}