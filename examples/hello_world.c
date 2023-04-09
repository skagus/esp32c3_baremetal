#include <stdio.h>

void mydelay(int nCnt)
{
    while(--nCnt > 0)
    {
        asm("nop\r\n");
    }
}

extern int gnSBss;
extern const unsigned int gnConst;
extern int gnSData;

int main(void)
{
    int nLoop = 10;
    mydelay(20000000);   // Wait USB CDC ready.
    printf("Hello World : %s\n", __DATE__);

    printf("Const   0x%X, %p\n", gnConst, &gnConst);
    printf("Data:   0x%X, %p\n", gnSData, &gnSData);
    printf("BSS :   %p\n", &gnSBss);

    while(nLoop--)
    {
        printf("Loop ZI %d\n", gnSBss);
        mydelay(20000000);
        gnSBss ++;
    }
    while(1);
    return 0;
}
