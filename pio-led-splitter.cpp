#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/pio.h"

#include "ws28xx-splitter.pio.h"

#define SOURCE_PIN 28
#define OUT_START_PIN 0
#define OUT_PIN_COUNT 3

void init_splitter_pio(PIO pio, uint splitter_sm, uint splitter_offset, uint reset_watch_sm, uint reset_watch_offset) {
    ws28xx_splitter_init(pio, splitter_sm, splitter_offset, SOURCE_PIN, OUT_START_PIN, OUT_PIN_COUNT);
    ws28xx_reset_watch_init(pio, reset_watch_sm, reset_watch_offset, SOURCE_PIN);
}

int main() {
    stdio_init_all();

    PIO pio = pio0;
    uint splitter_sm = 0;
    uint reset_watch_sm = 1;

    init_splitter_pio(pio, splitter_sm, pio_add_program(pio, &ws28xx_splitter_program), reset_watch_sm, pio_add_program(pio, &ws28xx_reset_watch_program));

    while (true) {
        sleep_ms(1000);
        // // Number of strands
        // pio_sm_put_blocking(pio, 0, (1 << OUT_PIN_COUNT));
        // // Count of pixels per strand
        // pio_sm_put_blocking(pio, 0, 5);
        // pio_sm_put_blocking(pio, 0, 5);
        // pio_sm_put_blocking(pio, 0, 5);
    }
}
