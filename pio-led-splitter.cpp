#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/pio.h"

#include "ws28xx-splitter.pio.h"

#define SOURCE_PIN 28
#define OUT_START_PIN 0
#define OUT_PIN_COUNT 1

void init_splitter_pio(PIO pio, uint sm, uint offset) {
    ws28xx_splitter_init(pio, sm, offset, SOURCE_PIN, OUT_START_PIN, OUT_PIN_COUNT);
    pio_sm_set_enabled(pio, sm, true);
}

int main() {
    stdio_init_all();

    PIO pio = pio0;
    uint offset = pio_add_program(pio, &ws28xx_splitter_program);
    printf("Loaded program at %d\n", offset);

    init_splitter_pio(pio, 0, offset);

    while (true) {
        sleep_ms(1000);
    }
}
