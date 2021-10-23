#include <stdio.h>
#include <unistd.h>

int main()
{
    while (1) {
        printf("so sleepy...........");
        fflush(stdout);
        usleep(1000000);
    }
    return 0;
}
