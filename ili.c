#include <wiringPi.h>
#include <wiringPiSPI.h>
#include <stdint.h>
#include <stdio.h>
#include <math.h>
#include "ili.h"
#include "interface.h"
#define PNG_SKIP_SETJMP_CHECK
#include <png.h>
#include <jpeglib.h>

void setup_pins(ILIObject *self) {
    wiringPiSetupGpio();
    if (self->SPI == -1) {
        pinMode (self->W, OUTPUT);
        pinMode (self->DB8, OUTPUT);
        pinMode (self->DB9, OUTPUT);
        pinMode (self->DB10, OUTPUT);
        pinMode (self->DB11, OUTPUT);
        pinMode (self->DB12, OUTPUT);
        pinMode (self->DB13, OUTPUT);
        pinMode (self->DB14, OUTPUT);
        pinMode (self->DB15, OUTPUT);

        digitalWrite (self->W, LOW);
        digitalWrite (self->DB8, LOW);
        digitalWrite (self->DB9, LOW);
        digitalWrite (self->DB10, LOW);
        digitalWrite (self->DB11, LOW);
        digitalWrite (self->DB12, LOW);
        digitalWrite (self->DB13, LOW);
        digitalWrite (self->DB14, LOW);
        digitalWrite (self->DB15, LOW);

    } else {
        self->spi_fd = wiringPiSPISetup (self->SPI, self->SPEED);
    }

    pinMode (self->RS, OUTPUT);
    pinMode (self->RST, OUTPUT);
    pinMode (self->CS, OUTPUT);
    digitalWrite (self->RS, LOW);
    digitalWrite (self->RST, LOW);
    digitalWrite (self->CS, HIGH);
}

void set_color(ILIObject *self, uint16_t r, uint16_t g, uint16_t b) {
    self->color = get_color(r,g,b);
}

void set_backgroundcolor(ILIObject *self, uint16_t r, uint16_t g, uint16_t b) {
    self->background_color = get_color(r,g,b);
}

uint16_t get_color(uint16_t r, uint16_t g, uint16_t b) {
    uint32_t rgb = (r << 16) | (g << 8) | b;
    return ((rgb & 0x00f80000) >> 8) | ((rgb & 0x0000fc00) >> 5) | ((rgb & 0x000000f8) >> 3);
}

void fill_rect(ILIObject *self, uint16_t pos_x1, uint16_t pos_y1, uint16_t pos_x2, uint16_t pos_y2) {
    unsigned int size = (abs(pos_x2 - pos_x1) + 1) * (abs(pos_y2 - pos_y1) + 1);
    set_area(self, min(pos_x1, pos_x2), min(pos_y1, pos_y2), max(pos_x1, pos_x2), max(pos_y1, pos_y2));
    int i;
    for(i=0; i<size; i++){
        data(self, self->background_color);
    }
}

void draw_pixel(ILIObject *self, uint16_t pos_x1, uint16_t pos_y1) {
    set_area(self, pos_x1, pos_y1, pos_x1, pos_y1);
    data(self, self->color);
}

void draw_vertical_line(ILIObject *self, uint16_t pos_x1, uint16_t pos_y1, uint16_t length) {
    set_area(self, pos_x1, pos_y1, pos_x1, pos_y1 + length);
    int i;
    for(i=0; i<length; i++){
        data(self, self->color);
    }
}

void draw_horizontal_line(ILIObject *self, uint16_t pos_x1, uint16_t pos_y1, uint16_t length) {
    set_area(self, pos_x1, pos_y1, pos_x1 + length, pos_y1);
    int i;
    for(i=0; i<length; i++){
        data(self, self->color);
    }
}

void swap(uint16_t *a, uint16_t *b) {
   uint16_t t;
   t  = *b; *b = *a; *a = t;
}

int calculate_line_appendix(int appendix) {
    if (appendix == 0) {
        return -1;
    }
    if (appendix < 0) {
        return appendix * -1;
    }
    return (appendix + 1) * -1;
}

void calculate_line_steps(int *steps, uint16_t length, uint16_t step, uint16_t required_length) {
    int i;
    for (i=0; i<step; i++) {
        steps[i] = length;
    }
    if (step * length < required_length) {
        int rest = required_length - step * length;
        int offset = round(step / 2);
        int steps_even = step % 2;
        int rest_even = rest % 2;
        int appendix = 0;
        for (i=0; i<rest; i++) {
            steps[offset + appendix] += 1;
            if (steps_even == 0) {
                appendix = calculate_line_appendix(appendix);
            } else if (i > 0 && rest_even == 0) {
                appendix = calculate_line_appendix(appendix);
            } else if (rest_even > 0) {
                appendix = calculate_line_appendix(appendix);
            }
        }
    }
}

void draw_line(ILIObject *self, uint16_t pos_x1, uint16_t pos_y1, uint16_t pos_x2, uint16_t pos_y2) {
    uint16_t width = abs(pos_x2 - pos_x1);
    uint16_t height = abs(pos_y2 - pos_y1);
    int horizontal;
    int offset_x=0, offset_y=0;

    int step = 0, required_length = 0, length = 0;

    if (pos_x1 == pos_x2) {
        draw_vertical_line(self, pos_x1, pos_y1, height+1);

        return;
    }
    if (pos_y1 == pos_y2) {
        draw_horizontal_line(self, pos_x1, pos_y1, width+1);

        return;
    }
    if (width > height) {
        horizontal = 1;
        width += 1;
        if (pos_x2 < pos_x1) {
            swap(&pos_x2, &pos_x1);
            swap(&pos_y2, &pos_y1);
        }
        if (pos_y2 > pos_y1) {
            offset_y = 1;
        } else {
            offset_y = -1;
        }
        if (pos_x2 > pos_x1) {
            offset_x = 1;
        } else {
            offset_x = -1;
        }
        length = round(width / (height + 1));
        step = height + 1;
        required_length = width;
    } else {
        horizontal = 0;
        height += 1;
        if (pos_y2 < pos_y1) {
            swap(&pos_x2, &pos_x1);
            swap(&pos_y2, &pos_y1);
        }
        if (pos_y2 > pos_y1) {
            offset_y = 1;
        } else {
            offset_y = -1;
        }
        if (pos_x2 > pos_x1) {
            offset_x = 1;
        } else {
            offset_x = -1;
        }
        length = round(height / (width + 1));
        step = width + 1;
        required_length = height;
    }

    int *steps = (int*)calloc(step, sizeof(int*));
    calculate_line_steps(steps, length, step, required_length);

    int delta_x=0, delta_y=0, i;
    for (i=0; i<step;i++) {
        //printf("i = %d \n", i);
        if (horizontal == 1) {
            draw_horizontal_line(
                self,
                pos_x1 + delta_x,
                pos_y1 + i * offset_y,
                steps[i]
            );
            delta_x += *(steps + i) * offset_x;
        } else {
            draw_vertical_line(
                self,
                pos_x1 + (i * offset_x),
                pos_y1 + delta_y,
                steps[i]
            );
            delta_y += *(steps + i) * offset_y;
        }
    }
    free(steps);
}

void draw_circle(ILIObject *self, uint16_t pos_x, uint16_t pos_y, uint16_t radius) {
    int err=0, offset_x = radius, offset_y=0;
    while (offset_x >= offset_y) {
        draw_pixel(self, pos_x + offset_x, pos_y + offset_y);
        draw_pixel(self, pos_x + offset_y, pos_y + offset_x);
        draw_pixel(self, pos_x - offset_y, pos_y + offset_x);
        draw_pixel(self, pos_x - offset_x, pos_y + offset_y);
        draw_pixel(self, pos_x - offset_x, pos_y - offset_y);
        draw_pixel(self, pos_x - offset_y, pos_y - offset_x);
        draw_pixel(self, pos_x + offset_y, pos_y - offset_x);
        draw_pixel(self, pos_x + offset_x, pos_y - offset_y);
        if (err <= 0) {
            offset_y += 1;
            err += 2*offset_y + 1;
        } else {
            offset_x -= 1;
            err -= 2*offset_x + 1;
        }
    }
}

void draw_arc(ILIObject *self, uint16_t pos_x, uint16_t pos_y, uint16_t radius, uint16_t start, uint16_t end) {
    int err=0, offset_x = radius, offset_y=0;
    double start_r, end_r;
    start_r = start * M_PI / 180;
    end_r = end * M_PI / 180;
    while (offset_x >= offset_y) {
        if (start_r <= atan2(offset_y, offset_x) && atan2(offset_y, offset_x) <= end_r) {draw_pixel(self, pos_x + offset_x, pos_y + offset_y);}
        if (start_r <= atan2(offset_x, offset_y) && atan2(offset_x, offset_y) <= end_r) {draw_pixel(self, pos_x + offset_y, pos_y + offset_x);}
        if (start_r <= atan2(offset_x, -offset_y) && atan2(offset_x, -offset_y) <= end_r) {draw_pixel(self, pos_x - offset_y, pos_y + offset_x);}
        if (start_r <= atan2(offset_y, -offset_x) && atan2(offset_y, -offset_x) <= end_r) {draw_pixel(self, pos_x - offset_x, pos_y + offset_y);}
        if (start_r <= atan2(-offset_y, -offset_x) + 2*M_PI && atan2(-offset_y, -offset_x) + 2*M_PI <= end_r) {draw_pixel(self, pos_x - offset_x, pos_y - offset_y);}
        if (start_r <= atan2(-offset_x, -offset_y) + 2*M_PI && atan2(-offset_x, -offset_y) + 2*M_PI <= end_r) {draw_pixel(self, pos_x - offset_y, pos_y - offset_x);}
        if (start_r <= atan2(-offset_x, offset_y) + 2*M_PI && atan2(-offset_x, offset_y) + 2*M_PI <= end_r) {draw_pixel(self, pos_x + offset_y, pos_y - offset_x);}
        if (start_r <= atan2(-offset_y, offset_x) + 2*M_PI && atan2(-offset_y, offset_x) + 2*M_PI <= end_r) {draw_pixel(self, pos_x + offset_x, pos_y - offset_y);}
        if (err <= 0) {
            offset_y += 1;
            err += 2*offset_y + 1;
        } else {
            offset_x -= 1;
            err -= 2*offset_x + 1;
        }
    }
}

void draw_rect(ILIObject *self, uint16_t pos_x1, uint16_t pos_y1, uint16_t pos_x2, uint16_t pos_y2) {
    draw_line(self, pos_x1, pos_y1, pos_x2, pos_y1);
    draw_line(self, pos_x1, pos_y2, pos_x2, pos_y2);
    draw_line(self, pos_x1, pos_y1, pos_x1, pos_y2);
    draw_line(self, pos_x2, pos_y1, pos_x2, pos_y2);
}

static int is_transparent(ILIObject *self, int r, int g, int b) {
    if (self->transparency[0] == -1) {
        return 0;
    }
    if (self->transparency[0] == r && self->transparency[1] == g && self->transparency[2] == b) {
        return 1;
    }

    return 0;
}

void draw_object_image(ILIObject *self, uint16_t pos_x, uint16_t pos_y, PyObject *image) {
    PyObject *size;
    int width, height, i, j;
    PyObject *pixel;
    int r,g,b;
    int temp_area[3] = {-1};
    int area[3] = {-1};

    size = PyObject_GetAttrString(image, "size");
    width = PyLong_AsLong(PyTuple_GetItem(size, 0));
    height = PyLong_AsLong(PyTuple_GetItem(size, 1));

    set_area(self, pos_x, pos_y, pos_x + width - 1, pos_y + height - 1);

    for(j=0; j<height; j++) {
        for (i=0; i<width; i++) {
            pixel = PyObject_CallMethodObjArgs(image, PyUnicode_FromString("getpixel"), Py_BuildValue("(ii)", i, j), NULL);
            r = PyLong_AsLong(PyTuple_GetItem(pixel, 0));
            g = PyLong_AsLong(PyTuple_GetItem(pixel, 1));
            b = PyLong_AsLong(PyTuple_GetItem(pixel, 2));
            if (is_transparent(self, r, g, b)) {
                area[0] = pos_x;
                area[1] = pos_y + j + 1;
                area[2] = pos_x + width - 1;
                area[3] = pos_y + height - 1;

                temp_area[0] = pos_x + i + 1;
                temp_area[1] = pos_y + j;
                temp_area[2] = pos_x + width -1;
                temp_area[3] = pos_y + j;
            } else {
                if (temp_area[0] != -1) {
                     set_area(self, temp_area[0], temp_area[1], temp_area[2], temp_area[3]);
                     temp_area[0] = -1;
                }
                data(self, get_color(r, g, b));
            }
        }
        if (area[0] != -1) {
            set_area(self, area[0], area[1], area[2], area[3]);
            area[0] = -1;
            temp_area[0] = -1;
        }
    }
}

void draw_jpeg_file_image(ILIObject *self, uint16_t pos_x, uint16_t pos_y, FILE *infile) {
    struct jpeg_decompress_struct cinfo;
    struct jpeg_error_mgr err;
    JSAMPARRAY buffer;		/* Output row buffer */
    int row_stride;		/* physical row width in output buffer */
    int red,green,blue;
    int i;
    int temp_area[3] = {-1};
    int area[3] = {-1};

    /* Step 1: allocate and initialize JPEG decompression object */
    cinfo.err = jpeg_std_error(&err);
    /* Now we can initialize the JPEG decompression object. */
    jpeg_create_decompress(&cinfo);

    /* Step 2: specify data source (eg, a file) */
    jpeg_stdio_src(&cinfo, infile);

    /* Step 3: read file parameters with jpeg_read_header() */
    (void) jpeg_read_header(&cinfo, TRUE);

    /* Step 5: Start decompressor */
    (void) jpeg_start_decompress(&cinfo);

    /* JSAMPLEs per row in output buffer */
    row_stride = cinfo.output_width * cinfo.output_components;
    /* Make a one-row-high sample array that will go away when done with image */
    buffer = (*cinfo.mem->alloc_sarray)
        ((j_common_ptr) &cinfo, JPOOL_IMAGE, row_stride, 1);

    set_area(self, pos_x, pos_y, pos_x + cinfo.output_width - 1, pos_y + cinfo.output_height - 1);
    /* Here we use the library's state variable cinfo.output_scanline as the
    * loop counter, so that we don't have to keep track ourselves.
    */
    while (cinfo.output_scanline < cinfo.output_height) {
        /* jpeg_read_scanlines expects an array of pointers to scanlines.
         * Here the array is only one element long, but you could ask for
         * more than one scanline at a time if that's more convenient.
         */
        (void) jpeg_read_scanlines(&cinfo, buffer, 1);
        unsigned char* pixel_row = (unsigned char*)(buffer[0]);
        for(i = 0; i < cinfo.output_width; i++) {
            red = (int)(*pixel_row++);
            green = (int)(*pixel_row++);
            blue = (int)(*pixel_row++);
            if (is_transparent(self, red, green, blue)) {
                area[0] = pos_x;
                area[1] = pos_y + cinfo.output_scanline + 1;
                area[2] = pos_x + cinfo.output_width - 1;
                area[3] = pos_y + cinfo.output_height - 1;

                temp_area[0] = pos_x + i + 1;
                temp_area[1] = pos_y + cinfo.output_scanline;
                temp_area[2] = pos_x + cinfo.output_width -1;
                temp_area[3] = pos_y + cinfo.output_scanline;
            } else {
                if (temp_area[0] != -1) {
                     set_area(self, temp_area[0], temp_area[1], temp_area[2], temp_area[3]);
                     temp_area[0] = -1;
                }
                data(self, get_color(red, green, blue));
            }
        }
        if (area[0] != -1) {
            set_area(self, area[0], area[1], area[2], area[3]);
            area[0] = -1;
            temp_area[0] = -1;
        }
    }

    /* Step 7: Finish decompression */
    (void) jpeg_finish_decompress(&cinfo);

    /* Step 8: Release JPEG decompression object */
    /* This is an important step since it will release a good deal of memory. */
    jpeg_destroy_decompress(&cinfo);
}

void draw_png_file_image(ILIObject *self, uint16_t pos_x, uint16_t pos_y, FILE *fp) {
    int width, height;
    png_byte color_type;
    png_byte bit_depth;
    png_bytep *row_pointers;
    int x,y;
    int temp_area[3] = {-1};
    int area[3] = {-1};
    int red,green,blue;

    png_structp png = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if(!png) abort();

    png_infop info = png_create_info_struct(png);
    if(!info) abort();

    if(setjmp(png_jmpbuf(png))) abort();

    png_init_io(png, fp);

    png_read_info(png, info);

    width      = png_get_image_width(png, info);
    height     = png_get_image_height(png, info);
    color_type = png_get_color_type(png, info);
    bit_depth  = png_get_bit_depth(png, info);

    // Read any color_type into 8bit depth, RGBA format.
    // See http://www.libpng.org/pub/png/libpng-manual.txt
    if(bit_depth == 16)
        png_set_strip_16(png);

    if(color_type == PNG_COLOR_TYPE_PALETTE)
        png_set_palette_to_rgb(png);

    // PNG_COLOR_TYPE_GRAY_ALPHA is always 8 or 16bit depth.
    if(color_type == PNG_COLOR_TYPE_GRAY && bit_depth < 8)
        png_set_expand_gray_1_2_4_to_8(png);

    if(png_get_valid(png, info, PNG_INFO_tRNS))
        png_set_tRNS_to_alpha(png);

    // These color_type don't have an alpha channel then fill it with 0xff.
    if(color_type == PNG_COLOR_TYPE_RGB ||
     color_type == PNG_COLOR_TYPE_GRAY ||
     color_type == PNG_COLOR_TYPE_PALETTE)
        png_set_filler(png, 0xFF, PNG_FILLER_AFTER);

    if(color_type == PNG_COLOR_TYPE_GRAY ||
     color_type == PNG_COLOR_TYPE_GRAY_ALPHA)
    png_set_gray_to_rgb(png);

    png_read_update_info(png, info);

    row_pointers = (png_bytep*)malloc(sizeof(png_bytep) * height);
    for(y = 0; y < height; y++) {
        row_pointers[y] = (png_byte*)malloc(png_get_rowbytes(png,info));
    }
    png_read_image(png, row_pointers);

    set_area(self, pos_x, pos_y, pos_x + width - 1, pos_y + height - 1);
    for(y = 0; y < height; y++) {
        png_bytep row = row_pointers[y];
        for(x = 0; x < width; x++) {
            png_bytep px = &(row[x * 4]);
            red = px[0];
            green = px[1];
            blue = px[2];
            if (is_transparent(self, red, green, blue)) {
                area[0] = pos_x;
                area[1] = pos_y + y + 1;
                area[2] = pos_x + width - 1;
                area[3] = pos_y + height - 1;

                temp_area[0] = pos_x + x + 1;
                temp_area[1] = pos_y + y;
                temp_area[2] = pos_x + width -1;
                temp_area[3] = pos_y + y;
            } else {
                if (temp_area[0] != -1) {
                   set_area(self, temp_area[0], temp_area[1], temp_area[2], temp_area[3]);
                   temp_area[0] = -1;
                }
                data(self, get_color(red, green, blue));
            }
        }
        if (area[0] != -1) {
            set_area(self, area[0], area[1], area[2], area[3]);
            area[0] = -1;
            temp_area[0] = -1;
        }
    }

    png_destroy_read_struct(&png, &info, NULL);
    png=NULL;
    info=NULL;
}