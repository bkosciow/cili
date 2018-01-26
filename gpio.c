#include <wiringPi.h>
#include <stdint.h>
#include "ili.h"

static void set_pins(ILIObject *self, uint8_t data);
static void set_pin(uint8_t pin, unsigned char value);
static void gpio_send(ILIObject *self, uint16_t data);

static void gpio_send(ILIObject *self, uint16_t data) {
    digitalWrite(self->CS, LOW);
    set_pins(self, data >> 8);
    digitalWrite(self->W, LOW);
    digitalWrite(self->W, HIGH);
    set_pins(self, data);
    digitalWrite(self->W, LOW);
    digitalWrite(self->W, HIGH);
    digitalWrite(self->CS, HIGH);
}

static void set_pins(ILIObject *self, uint8_t data) {
    set_pin(self->DB8, (data >> 0));
    set_pin(self->DB9, (data >> 1));
    set_pin(self->DB10, (data >> 2));
    set_pin(self->DB11, (data >> 3));
    set_pin(self->DB12, (data >> 4));
    set_pin(self->DB13, (data >> 5));
    set_pin(self->DB14, (data >> 6));
    set_pin(self->DB15, (data >> 7));
}

static void set_pin(uint8_t pin, unsigned char value) {
    if (value & 0x01) { digitalWrite(pin, HIGH);} else {digitalWrite(pin, LOW);}
}

void gpio_cmd(ILIObject *self, uint16_t data) {
    digitalWrite(self->RS, LOW); gpio_send(self, data);
}

void gpio_data(ILIObject *self, uint16_t data) {
    digitalWrite(self->RS, HIGH); gpio_send(self, data);
}

void gpio_set_cs(ILIObject *self, int state) {
    digitalWrite (self->CS, state);
};

void gpio_set_rst(ILIObject *self, int state) {
    digitalWrite (self->RST, state);
}