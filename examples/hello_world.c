
#include "esp32c3.h"
#include "rom_ref.h"

#define printf(...) 	ets_printf(__VA_ARGS__)

bool gbIn = true;

int gnCnt = 8;
int main(void)
{
	delay_ms(1000);
	
	gpio_output(LED0);
	gpio_output(LED1);

	gpio_input(BTN1);
	gpio_int_en(BTN1);
	soc_enable_int(9);
	soc_int(true);


	printf("ROM Version : %X\n", esp_pp_rom_version_get());

	int nCnt = 0;
	while (nCnt < gnCnt)
	{
		printf("Loop : %d / %d\n", nCnt, gnCnt);
		delay_ms(100);
		gpio_toggle(LED1);
		nCnt++;
	}

	uint32_t bmPrvIS = 0;
	uint32_t bmIS = 0;
	while (1)
	{
//		bool bIn = gpio_read(BTN1);
//		test_ecall();
		bmIS = REG(C3_GPIO + 0x44);
		if(bmIS != bmPrvIS)
		{
			printf("IntStatus: %8X\n", bmIS);
			bmPrvIS = bmIS;
		}
		gpio_write(LED0, gbIn);
		delay_ms(1000);
	}
	return 0;
}

