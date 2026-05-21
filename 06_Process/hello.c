#include <unistd.h>
#include <stdio.h>
int main()
{
    while (1)
    {
        printf("hello \n");
        usleep(5000 * 1000);
    }

    return 0;
}