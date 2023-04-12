MEMORY
{
    irom (x): org = 0x42000000, len = 4M    /* 0x4200_0000 ~ 0x4240_0000   */
    drom (r): org = 0x3C000000, len = 4M    /* 0x3C00_0000 ~ 0x3C40_0000   */
    ram (rw): org = 0x3FC80000, len = 320K  /* 0x3FC8_0000 ~ 0x3FCD_0000 */
    rtc_ram (rx): org = 0x50000000, len = 8K
}

/**
Physical Memory
    Boot ROM: 384KB
    SRAM : 400KB (used by iram/drom/ram/cache(16KB))
    RTC Fast Memory : 8KB

MASK ROM 영역.
    0x4000_0000 ~ 0x4006_0000 : 384 KB
    0x3ff0_0000 ~ 0x3FF2_0000 : 128 KB (Q: 웬 RAM인가??)

Cached 영역(실제 16KB cache로 서비스하는 공간)
    0x4200_0000 ~ 0x4280_0000 : 8 MB  <-- SPI Flash.

SPI RAM 공간 (이것도 Cache되는가? 아니면 Access할 때마다 가져오는건가?)
    0x3C00_0000 ~ 0x3C80_0000 : 8 MB  <-- SPI SRAM

Direct 영역 (direct인데, I bus와 D bus에 중복 연결된건가??)
    I: 0x4037_C000 ~ 0x403E_0000 : 400 KB  (Q: 왜 384KB가 아니고..?)
    D: 0x3FC8_0000 ~ 0x3FCE_0000 : 384 KB  (400KB중 16KB cache제외 공간)

추가 설명.
    C3에서는 400KB의 메모리를 세가지로 쪼개서 사용.
    16KB Flash Cache / X KB IRAM / 400-16-X KB DRAM 
    여기에서, IRAM으로 사용하는 용량에 따라서, DRAM은 줄어드는 구조..
    (이건 RWX가 되는 memory를 나눠쓰는 거니까 당연한 얘기이다.)
    C3에서 특이한 점은, Address map인데, 
    IRAM은 0x4038_0000, DRAM은 0x3FC8_0000 에 붙여놓은거다. 
    (즉, 같은 memory인데 보는 주소는 서로 다름.)
    그리고 Physical SRAM상의 앞쪽에는 IRAM이 뒷쪽에는 DRAM이 차지하는 구조. 
    +------------------+---------------------+
    |<- 0x4038_ IRAM   |  DRAM   0x3FCE_ --->|
    +------------------+---------------------+
    (예상: 저 위치의 IRAM은 Flash에서 non-cached로 full load되지 않을까?)
    이런 memory map은 정해진 크기의 [IRAM + .RO/.RW]를 그냥 묶어서 load하면 loading은 끝남.
    .text는 0x4038_0000에 두고, 
    RO/RW는 DRAM base + .text크기 위치에 두면됨. <-- load시 RW copy도 필요없음.
    장점: Address가 떨어져 있지만, RW copy없이 사용가능.   

================================================================

*/