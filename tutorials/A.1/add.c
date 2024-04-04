#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
    float a = atof(argv[1]);
    float b = atof(argv[2]);
    float c = a + b;
    printf("%f\n", c);
    return 0;
}