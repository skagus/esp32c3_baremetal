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
    0x3C00_0000 ~ 0x3C80_0000 : 8 MB  <-- SPI SRAM
    0x4200_0000 ~ 0x4280_0000 : 8 MB  <-- SPI Flash.

Direct 영역 (direct인데, I bus와 D bus에 중복 연결된건가??)
    0x3FC8_0000 ~ 0x3FCE_0000 : 384 KB  (400KB중 16KB cache제외 공간)
    0x4037_C000 ~ 0x403E_0000 : 400 KB  (Q: 왜 384KB가 아니고..?)

================================================================

*/