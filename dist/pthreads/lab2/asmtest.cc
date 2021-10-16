#include <iostream>
int main()
{
    int x = 1;
    asm("mov %0, eax"
        : "=r" (x));
    asm("popcnt %eax, %eax\n\t");
    asm("mov eax, %0"
        : "=r" (x));
    printf("%d", x);

}
