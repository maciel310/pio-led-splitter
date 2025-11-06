#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/pio.h"

#include "ws28xx-splitter.pio.h"

#define SOURCE_PIN 28
#define OUT_START_PIN 0
#define OUT_PIN_COUNT 3

PIO pio = pio0;
uint splitter_sm = 0;
uint reset_watch_sm = 1;
uint splitter_offset;
uint reset_watch_offset;

void ws28xx_reset_watch_handler() {
    ws28xx_splitter_reset(pio, splitter_sm, splitter_offset);

    // TODO: Move this to DMA so more than 4 messages can be sent in the IRQ without blocking.
    // Number of strands
    pio_sm_put_blocking(pio, splitter_sm, (1 << OUT_PIN_COUNT));
    // Count of pixels per strand
    pio_sm_put_blocking(pio, splitter_sm, 6-1);
    pio_sm_put_blocking(pio, splitter_sm, 10-1);
    pio_sm_put_blocking(pio, splitter_sm, 21-1);

    pio_interrupt_clear(pio, 0);
}

void init_splitter_pio(PIO pio, uint splitter_sm, uint splitter_offset, uint reset_watch_sm, uint reset_watch_offset) {
    pio_set_irq0_source_enabled(pio, pis_interrupt0, true);
    irq_set_exclusive_handler(pio_get_irq_num(pio, 0), ws28xx_reset_watch_handler);
    irq_set_enabled(pio_get_irq_num(pio, 0), true);

    ws28xx_splitter_init(pio, splitter_sm, splitter_offset, SOURCE_PIN, OUT_START_PIN, OUT_PIN_COUNT);
    ws28xx_reset_watch_init(pio, reset_watch_sm, reset_watch_offset, SOURCE_PIN);
}

int main() {
    stdio_init_all();

    splitter_offset = pio_add_program(pio, &ws28xx_splitter_program);
    reset_watch_offset = pio_add_program(pio, &ws28xx_reset_watch_program);
    init_splitter_pio(pio, splitter_sm, splitter_offset, reset_watch_sm, reset_watch_offset);

    while (true) {
    }
}
