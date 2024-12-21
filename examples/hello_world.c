
#include "esp32c3.h"
#include "rom_ref.h"

#define printf(...) 	ets_printf(__VA_ARGS__)

bool gbIn = true;

int gnCnt = 4;

extern const unsigned char _binary____examples_data_txt_start;
extern const unsigned char _binary____examples_data_txt_end;
extern const unsigned int _binary____examples_data_txt_size;

void intr_matrix_route(int intr_src, int intr_num)
{
	*(uint32_t*)(C3_INTERRUPT + 4 * intr_src) = intr_num;
}

#define INT_SRC_GPIO		(16)
#define INT_NUM				(20)
int main(void)
{
	delay_ms(1000);

#if 0
	gpio_output(LED0);
	gpio_output(LED1);

	gpio_input(BTN1);
	gpio_int_en(BTN1);
#if 1
	intr_matrix_route(15, INT_NUM);
	intr_matrix_route(16, INT_NUM);
	soc_enable_int(15);
	soc_enable_int(16);
#else
	soc_enable_int(9);
#endif
	soc_int(true);
#endif

	printf("ROM Version : %X\n", esp_pp_rom_version_get());

	int nCnt = 0;
	while (nCnt < gnCnt)
	{
		printf("Loop : %d / %d\n", nCnt, gnCnt);
		delay_ms(1000);
		nCnt++;
	}
	char* ptr = & _binary____examples_data_txt_start;
	while(ptr < &_binary____examples_data_txt_end)
	{
		printf("%c", *ptr);
		ptr++;
	}

	uint32_t bmPrvIS = 0;
	uint32_t bmIS = 0;
	while (1)
	{
//		bmIS = gpio_read(BTN1);
//		test_ecall();
//		bmIS = REG(C3_GPIO + 0x44);
		if(bmIS != bmPrvIS)
		{
			printf("IntStatus: %8X\n", bmIS);
			bmPrvIS = bmIS;
		}
//		gpio_write(LED0, gbIn);
		delay_ms(100);
	}
	return 0;
}

