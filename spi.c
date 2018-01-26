#include <wiringPi.h>
#include <wiringPiSPI.h>
#include <stdint.h>
#include "ili.h"
#include "spi.h"
//#include "interface.h"

static void spi_send(ILIObject *self, uint16_t data);

unsigned char buffer[1];
int result;

static void spi_send(ILIObject *self, uint16_t data) {
    buffer[0] = data;
    result = wiringPiSPIDataRW(self->SPI, buffer, 1);
    //write(self->SPI, data);
}

void spi_cmd(ILIObject *self, uint16_t data) {
    digitalWrite(self->RS, LOW); spi_send(self, data);
}

void spi_data(ILIObject *self, uint16_t data) {
    digitalWrite(self->RS, HIGH); spi_send(self, data);
}

void spi_set_cs(ILIObject *self, int state) {
    digitalWrite (self->CS, state);
}

void spi_set_rst(ILIObject *self, int state) {
    digitalWrite (self->RST, state);
}