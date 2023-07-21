#include <stdio.h>

#define printf(...) 	ets_printf(__VA_ARGS__)


extern int gnSData;


void mydelay(int nCnt)
{
	while (--nCnt > 0)
	{
		asm("nop\r\n");
	}
}

int gnCnt = 30;
int main(void)
{
	int nLoop = 10;
	mydelay(1000000);
	printf("Hello World : %s\n", __DATE__);

	while (gnCnt--)
	{
		printf("Loop : %d, %X, %X\n", gnCnt, &gnSData, gnSData);
		mydelay(1000000);
	}
	while (1);
	return 0;
}
