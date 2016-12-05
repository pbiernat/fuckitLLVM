#include <stdio.h>

int main(int argc, char ** argv)
{
    int i = 0x41414141;

    *(int *)i = 0x10;

    printf("I'm still alive!\n");
    return 0;
}
