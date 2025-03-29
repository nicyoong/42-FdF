#include <stdio.h>
#include <math.h>

int main(void)
{
    #ifdef M_PI
    printf("M_PI is defined: %.15f\n", M_PI);
    #else
    printf("Error: M_PI not defined in math.h\n");
    #endif
    return 0;
}