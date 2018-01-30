#include <Python.h>

typedef struct {
    PyObject_HEAD
    int width;
    int height;
    int CS;
    int RS;
    int W;
    int DB8;
    int DB9;
    int DB10;
    int DB11;
    int DB12;
    int DB13;
    int DB14;
    int DB15;
    int RST;
    int SPI;
    int SPEED;
    int spi_fd;
    int color;
    int background_color;
    int rotation;
} ILIObject;

#define min(a,b) (((a)<(b))?(a):(b))
#define max(a,b) (((a)>(b))?(a):(b))
#define abs(x) ((x)>0?(x):-(x))

void init_display(ILIObject *self);
void swap(uint16_t*, uint16_t*);
void set_area(ILIObject *self, uint16_t pos_x1, uint16_t pos_y1, uint16_t pos_x2, uint16_t pos_y2);
void setup_pins(ILIObject *self);
void set_color(ILIObject *self, uint16_t r, uint16_t g, uint16_t b);
void set_backgroundcolor(ILIObject *self, uint16_t r, uint16_t g, uint16_t b);
uint16_t get_color(uint16_t r, uint16_t g, uint16_t b);
void fill_rect(ILIObject *self, uint16_t pos_x1, uint16_t pos_y1, uint16_t pos_x2, uint16_t pos_y2);
void draw_rect(ILIObject *self, uint16_t pos_x1, uint16_t pos_y1, uint16_t pos_x2, uint16_t pos_y2);
void draw_pixel(ILIObject *self, uint16_t pos_x1, uint16_t pos_y1);
void draw_line(ILIObject *self, uint16_t pos_x1, uint16_t pos_y1, uint16_t pos_x2, uint16_t pos_y2);
void draw_image(ILIObject *self, uint16_t pos_x, uint16_t pos_y, PyObject *image);

void draw_circle(ILIObject *self, uint16_t pos_x, uint16_t pos_y, uint16_t radius);
void draw_arc(ILIObject *self, uint16_t pos_x, uint16_t pos_y, uint16_t radius, uint16_t start, uint16_t end);

void draw_vertical_line(ILIObject *self, uint16_t pos_x1, uint16_t pos_y1, uint16_t length);
void draw_horizontal_line(ILIObject *self, uint16_t pos_x1, uint16_t pos_y1, uint16_t length);
