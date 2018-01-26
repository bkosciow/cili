#include <wiringPi.h>
#include <stdint.h>
#include <stdio.h>
#include "ili.h"
#include "interface.h"

void set_area(ILIObject *self, uint16_t pos_x1, uint16_t pos_y1, uint16_t pos_x2, uint16_t pos_y2) {
    cmd(self, 0x2a);
    data(self, pos_x1 >> 8);
    data(self, pos_x1 & 0xff);
    data(self, pos_x2 >> 8);
    data(self, pos_x2 & 0xff);
    cmd(self, 0x2b);
    data(self, pos_y1 >> 8);
    data(self, pos_y1 & 0xff);
    data(self, pos_y2 >> 8);
    data(self, pos_y2 & 0xff);
    cmd(self, 0x2c);
}

void init_display(ILIObject *self){
    int rotate = 0x88;

    setup_pins(self);
    set_cs(self, HIGH);
    set_rst(self, HIGH);
    delay(5);
    set_rst(self, LOW);
    delay(5);
    set_rst(self, HIGH);
    delay(1);

    if (self->rotation == 90) {
        rotate = 0xf8;
    } else if (self->rotation == 180) {
        rotate = 0x48;
    } else if (self->rotation == 270) {
        rotate = 0x28;
    }

    //# Read Display MADCTL
    cmd(self, 0x0b);
    data(self, 0x00);
    data(self, 0x00);

    //# Sleep OUT
    cmd(self, 0x11);

    //# Interface Pixel Format
    cmd(self, 0x3a);
    data(self, 0x55); //#0x66 5-6-5 / 55 6-6-6

    //# Memory Access Control (
    cmd(self, 0x36);
    data(self, rotate);

    //# Power Control 3 (For Normal Mode)
    cmd(self, 0xc2);
    data(self, 0x44);

    //# VCOM Control
    cmd(self, 0xc5);
    data(self, 0x00);
    data(self, 0x00);
    data(self, 0x00);
    data(self, 0x00);

    //# PGAMCTRL(Positive Gamma Control)
    cmd(self, 0xe0);
    data(self, 0x0F);
    data(self, 0x1F);
    data(self, 0x1C);
    data(self, 0x0C);
    data(self, 0x0F);
    data(self, 0x08);
    data(self, 0x48);
    data(self, 0x98);
    data(self, 0x37);
    data(self, 0x0A);
    data(self, 0x13);
    data(self, 0x04);
    data(self, 0x11);
    data(self, 0x0D);
    data(self, 0x00);

    //# NGAMCTRL (Negative Gamma Correction)
    cmd(self, 0xe1);
    data(self, 0x0F);
    data(self, 0x32);
    data(self, 0x2E);
    data(self, 0x0B);
    data(self, 0x0D);
    data(self, 0x05);
    data(self, 0x47);
    data(self, 0x75);
    data(self, 0x37);
    data(self, 0x06);
    data(self, 0x10);
    data(self, 0x03);
    data(self, 0x24);
    data(self, 0x20);
    data(self, 0x00);

    //# Digital Gamma Control 1
    cmd(self, 0xe2);
    data(self, 0x0F);
    data(self, 0x32);
    data(self, 0x2E);
    data(self, 0x0B);
    data(self, 0x0D);
    data(self, 0x05);
    data(self, 0x47);
    data(self, 0x75);
    data(self, 0x37);
    data(self, 0x06);
    data(self, 0x10);
    data(self, 0x03);
    data(self, 0x24);
    data(self, 0x20);
    data(self, 0x00);

    //# Sleep OUT
    cmd(self, 0x11);

    //# Display ON
    cmd(self, 0x29);


    delay(100);
}

