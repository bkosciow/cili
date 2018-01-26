#include <stdint.h>
#include "ili.h"
#include "interface.h"
#include "spi.h"
#include "gpio.h"

void cmd(ILIObject *self, uint16_t data) {
    if (self->SPI == -1) {
        gpio_cmd(self, data);
    } else {
        spi_cmd(self, data);
    }
}

void data(ILIObject *self, uint16_t data) {
    if (self->SPI == -1) {
        gpio_data(self, data);
    } else {
        spi_data(self, data);
    }
}

void set_cs(ILIObject *self, int state) {
    if (self->SPI == -1) {
        gpio_set_cs(self, state);
    } else {
        spi_set_cs(self, state);
    }
}
void set_rst(ILIObject *self, int state) {
    if (self->SPI == -1) {
        gpio_set_rst(self, state);
    } else {
        spi_set_rst(self, state);
    }
}
