#include <stdio.h>
#include "esp32c3.h"

#define printf(...) 	ets_printf(__VA_ARGS__)

int gnCnt = 8;
int main(void)
{
	delay_ms(1000);

	int nCnt = 0;
	while (nCnt < gnCnt)
	{
		printf("Loop : %d / %d\n", nCnt, gnCnt);
		delay_ms(100);
		nCnt++;
	}
	while (1);
	return 0;
}
