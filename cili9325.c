#include <wiringPi.h>
#include <stdint.h>
#include <stdio.h>
#include "ili.h"
#include "interface.h"

void set_area(ILIObject *self, uint16_t pos_x1, uint16_t pos_y1, uint16_t pos_x2, uint16_t pos_y2) {
    cmd(self, 0x0020); data(self, pos_x1);
    cmd(self, 0x0021); data(self, pos_y1);
    cmd(self, 0x0050); data(self, pos_x1);
    cmd(self, 0x0052); data(self, pos_y1);
    cmd(self, 0x0051); data(self, pos_x2);
    cmd(self, 0x0053); data(self, pos_y2);
    cmd(self, 0x0022);
}

void init_display(ILIObject *self){
    int rotation_output = 0x100;
    int rotation_mode = 0x1030;
    int rotation_output2 = 0xa700;

    setup_pins(self);
    set_cs(self, HIGH);
    set_rst(self, HIGH);
    delay(5);
    set_rst(self, LOW);
    delay(5);
    set_rst(self, HIGH);
    delay(1);

    if (self->rotation == 90) {
        rotation_output = 0x0000;
        rotation_mode = 0x1038;
    } else if (self->rotation == 180) {
        rotation_output = 0x0000;
        rotation_output2 = 0x2700;
    } else if (self->rotation == 270) {
        rotation_mode = 0x1038;
        rotation_output2 = 0x2700;
    }

    cmd(self, 0x0001);
    data(self, rotation_output);

    // set 1 line inversion
    cmd(self, 0x0002);
    data(self, 0x0700);

    // set GRAM write direction and BGR=1
    cmd(self, 0x0003);
    data(self, rotation_mode);

    // Resize register
    cmd(self, 0x0004);
    data(self, 0x0000);
    // set the back porch and front porch
    cmd(self, 0x0008);
    data(self, 0x0207);
    // set non-display area refresh cycle ISC[3:0]
    cmd(self, 0x0009);
    data(self, 0x0000);
    // FMARK function
    cmd(self, 0x000A);
    data(self, 0x0000);
    // RGB interface setting
    cmd(self, 0x000C);
    data(self, 0x0000);
    // Frame marker Position
    cmd(self, 0x000D);
    data(self, 0x0000);
    // RGB interface polarity
    cmd(self, 0x000F);
    data(self, 0x0000);

    // ************* Power On sequence ****************
    // SAP, BT[3:0], AP, DSTB, SLP, STB
    cmd(self, 0x0010);
    data(self, 0x0000);
    // DC1[2:0], DC0[2:0], VC[2:0]
    cmd(self, 0x0011);
    data(self, 0x0007);
    // VREG1OUT voltage
    cmd(self, 0x0012);
    data(self, 0x0000);
    // VDV[4:0] for VCOM amplitude
    cmd(self, 0x0013);
    data(self, 0x0000);
    cmd(self, 0x0007);
    data(self, 0x0001);
    delay(5);  // Dis-charge capacitor power voltage
    // SAP, BT[3:0], AP, DSTB, SLP, STB
    cmd(self, 0x0010);
    data(self, 0x1690);
    // Set DC1[2:0], DC0[2:0], VC[2:0]
    cmd(self, 0x0011);
    data(self, 0x0227);
    delay(5);
    cmd(self, 0x0012);
    data(self, 0x000D);
    delay(5);
    // VDV[4:0] for VCOM amplitude
    cmd(self, 0x0013);
    data(self, 0x1200);
    // 04  VCM[5:0] for VCOMH
    cmd(self, 0x0029);
    data(self, 0x000A);
    // Set Frame Rate
    cmd(self, 0x002B);
    data(self, 0x000D);
    delay(5);
    // GRAM horizontal Address
    cmd(self, 0x0020);
    data(self, 0x0000);
    // GRAM Vertical Address
    cmd(self, 0x0021);
    data(self, 0x0000);

    // ************* Adjust the Gamma Curve *************
    cmd(self, 0x0030);
    data(self, 0x0000);
    cmd(self, 0x0031);
    data(self, 0x0404);
    cmd(self, 0x0032);
    data(self, 0x0003);
    cmd(self, 0x0035);
    data(self, 0x0405);
    cmd(self, 0x0036);
    data(self, 0x0808);
    cmd(self, 0x0037);
    data(self, 0x0407);
    cmd(self, 0x0038);
    data(self, 0x0303);
    cmd(self, 0x0039);
    data(self, 0x0707);
    cmd(self, 0x003C);
    data(self, 0x0504);
    cmd(self, 0x003D);
    data(self, 0x0808);

    // ************* Set GRAM area *************
    // Horizontal GRAM Start Address
    cmd(self, 0x0050);
    data(self, 0x0000);
    // Horizontal GRAM End Address
    cmd(self, 0x0051);
    data(self, 0x00EF);
    // Vertical GRAM Start Address
    cmd(self, 0x0052);
    data(self, 0x0000);
    // Vertical GRAM Start Address
    cmd(self, 0x0053);
    data(self, 0x013F);
    // Gate Scan Line
    cmd(self, 0x0060);
    data(self, rotation_output2);

    // NDL, VLE, REV
    cmd(self, 0x0061);
    data(self, 0x0001);
    // set scrolling line
    cmd(self, 0x006A);
    data(self, 0x0000);

    // ************* Partial Display Control *************
    cmd(self, 0x0080);
    data(self, 0x0000);
    cmd(self, 0x0081);
    data(self, 0x0000);
    cmd(self, 0x0082);
    data(self, 0x0000);
    cmd(self, 0x0083);
    data(self, 0x0000);
    cmd(self, 0x0084);
    data(self, 0x0000);
    cmd(self, 0x0085);
    data(self, 0x0000);

    // ************* Panel Control *************
    cmd(self, 0x0090);
    data(self, 0x0010);
    cmd(self, 0x0092);
    data(self, 0x0000);
    // 262K color and display ON
    cmd(self, 0x0007);
    data(self, 0x0133);
    delay(100);
}
