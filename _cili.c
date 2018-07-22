#include <Python.h>
#include "ili.h"

/*
width, height, PIN_CS, PIN_RS,PIN_W,PIN_RST,PIN_DB8,PIN_DB9,PIN_DB10,PIN_DB11,PIN_DB12,PIN_DB13,PIN_DB14,PIN_DB15
width, height, SPI, SPEED, CS, RST, RS
*/
int ili_init(ILIObject *self, PyObject *args) {
    Py_ssize_t TupleSize = PyTuple_Size(args);
    if (TupleSize == 14) {
        if (!PyArg_ParseTuple(args, "IIIIIIIIIIIIII", &self->width, &self->height,
            &self->CS, &self->RS, &self->W, &self->RST,
            &self->DB8, &self->DB9, &self->DB10, &self->DB11,
            &self->DB12, &self->DB13, &self->DB14, &self->DB15)) {
            return -1;
        }
        self->SPI = -1;
    } else if (TupleSize == 7) {
         if (!PyArg_ParseTuple(args, "IIIIIII", &self->width, &self->height,
            &self->SPI, &self->SPEED,
            &self->CS, &self->RST, &self->RS)) {
            return -1;
        }
        printf("%d", self->SPEED);
    } else {
        PyErr_SetString(PyExc_RuntimeError,
                    "Wrong number of parameters: \n width, height, PIN_CS, PIN_RS,PIN_W,PIN_RST,PIN_DB8,PIN_DB9,PIN_DB10,PIN_DB11,PIN_DB12,PIN_DB13,PIN_DB14,PIN_DB15 \n width, height, SPI, SPEED, CS, RST, RS");
        return -1;
    }
    self->rotation = 0;
    self->is_transparency=0;

    return 0;
}

void ili_dealloc(ILIObject* self) {
    Py_TYPE(self)->tp_free((PyObject*)self);
}

PyObject *ili_new(PyTypeObject *type, PyObject *args, PyObject *kwds) {
    ILIObject *self;
    self = (ILIObject *)type->tp_alloc(type, 0);

    self->color = get_color(255, 255, 255);
    self->background_color = 0;

    return (PyObject *)self;
}

PyObject *ili_init_display(ILIObject *self) {
    init_display(self);
    Py_INCREF(Py_None);
    return Py_None;
}

//*********************
// magic get / set
PyObject *magic_get_width(ILIObject *self, void *closure) {
    if (self->rotation == 0 || self->rotation == 180)
        return Py_BuildValue("i", self->width);
    else
        return Py_BuildValue("i", self->height);
}

PyObject *magic_get_height(ILIObject *self, void *closure) {
    if (self->rotation == 0 || self->rotation == 180)
        return Py_BuildValue("i", self->height);
    else
        return Py_BuildValue("i", self->width);
}

PyObject *magic_get_color(ILIObject *self, void *closure) {
    return Py_BuildValue("i", self->color);
}

int *magic_set_color(ILIObject *self, PyObject *args, void *closure) {
    int r,g,b;
    r = PyLong_AsLong(PyTuple_GetItem(args, 0));
    g = PyLong_AsLong(PyTuple_GetItem(args, 1));
    b = PyLong_AsLong(PyTuple_GetItem(args, 2));

    set_color(self, r, g, b);

    return 0;
}

PyObject *magic_get_background_color(ILIObject *self, void *closure) {
    return Py_BuildValue("i", self->background_color);
}

int *magic_set_background_color(ILIObject *self, PyObject *args, void *closure) {

    int r,g,b;
    r = PyLong_AsLong(PyTuple_GetItem(args, 0));
    g = PyLong_AsLong(PyTuple_GetItem(args, 1));
    b = PyLong_AsLong(PyTuple_GetItem(args, 2));

    set_backgroundcolor(self, r, g, b);

    return 0;
}

PyObject *magic_get_auto_flush(ILIObject *self, void *closure) {
    return Py_BuildValue("B", 1);
}

int *magic_set_auto_flush(ILIObject *self, PyObject *value, void *closure) {
    return 0;
}

PyObject *magic_get_rotation(ILIObject *self, void *closure) {
    return Py_BuildValue("i", self->rotation);
}

int *magic_set_rotation(ILIObject *self, PyObject *value, void *closure) {
    int r = PyLong_AsLong(value);
    self->rotation = r;

    return 0;
}

PyObject *magic_get_transparency_color(ILIObject *self, void *closure) {
    if (self->is_transparency == 0) {
        Py_INCREF(Py_None);
        return Py_None;
    }
    int i;
    PyObject *pylist = PyTuple_New(self->is_transparency);
    for (i=0; i<self->is_transparency; i++) {
        PyObject *item = PyTuple_New(3);
        PyTuple_SetItem(item, 0, PyLong_FromLong(self->transparency[i].r));
        PyTuple_SetItem(item, 1, PyLong_FromLong(self->transparency[i].g));
        PyTuple_SetItem(item, 2, PyLong_FromLong(self->transparency[i].b));
        PyTuple_SetItem(pylist, i, item);
    }

    return Py_BuildValue("N",pylist);
}

int *magic_set_transparency_color(ILIObject *self, PyObject *args, void *closure) {
    free(self->transparency);
    self->is_transparency = 0;
    if (args == Py_None) {
        self->is_transparency = 0;
    } else {
        //multiple rgb tuples
        if (PyTuple_Check(PyTuple_GetItem(args, 0))) {
            int size = PyTuple_Size(args);
            int i;
            self->transparency = malloc((size+1) * sizeof(RGB));
            self->is_transparency = size;
            for(i=0; i<size; i++) {
                PyObject *t = PyTuple_GetItem(args, i);
                RGB rgb;
                rgb.r = PyLong_AsLong(PyTuple_GetItem(t, 0));
                rgb.g = PyLong_AsLong(PyTuple_GetItem(t, 1));
                rgb.b = PyLong_AsLong(PyTuple_GetItem(t, 2));
                self->transparency[i] = rgb;
            }
        } else { //single rgb tuple
            self->is_transparency = 1;
            RGB rgb;
            rgb.r = PyLong_AsLong(PyTuple_GetItem(args, 0));
            rgb.g = PyLong_AsLong(PyTuple_GetItem(args, 1));
            rgb.b = PyLong_AsLong(PyTuple_GetItem(args, 2));

            self->transparency = malloc(1 * sizeof(RGB));
            self->transparency[0] = rgb;
        }
    }

    return 0;
}

//***************************
// drawing funcions
PyObject *ili_fill_rect(ILIObject *self, PyObject *args) {
    int pos_x1, pos_x2, pos_y1, pos_y2;
    if (!PyArg_ParseTuple(args, "IIII", &pos_x1, &pos_y1, &pos_x2, &pos_y2)) {
        return NULL;
    }
    fill_rect(self, pos_x1, pos_y1, pos_x2, pos_y2);

    Py_INCREF(Py_None);
    return Py_None;
}

PyObject *ili_draw_rect(ILIObject *self, PyObject *args) {
    int pos_x1, pos_x2, pos_y1, pos_y2;
    if (!PyArg_ParseTuple(args, "IIII", &pos_x1, &pos_y1, &pos_x2, &pos_y2)) {
        return NULL;
    }
    draw_rect(self, pos_x1, pos_y1, pos_x2, pos_y2);

    Py_INCREF(Py_None);
    return Py_None;
}

PyObject *ili_set_color(ILIObject *self, PyObject *args) {
    int r, g, b;
    if (!PyArg_ParseTuple(args, "III", &r, &g, &b)) {
        return NULL;
    }
    set_color(self, r,g,b);

    Py_INCREF(Py_None);
    return Py_None;
}

PyObject *ili_set_backgroundcolor(ILIObject *self, PyObject *args) {
    int r, g, b;
    if (!PyArg_ParseTuple(args, "III", &r, &g, &b)) {
        return NULL;
    }

    set_backgroundcolor(self, r,g,b);

    Py_INCREF(Py_None);
    return Py_None;
}

PyObject *ili_draw_pixel(ILIObject *self, PyObject *args) {
    int pos_x1, pos_y1;
    if (!PyArg_ParseTuple(args, "II", &pos_x1, &pos_y1)) {
        return NULL;
    }
    draw_pixel(self, pos_x1, pos_y1);

    Py_INCREF(Py_None);
    return Py_None;
}

PyObject *ili_draw_line(ILIObject *self, PyObject *args) {
    int pos_x1, pos_x2, pos_y1, pos_y2;
    if (!PyArg_ParseTuple(args, "IIII", &pos_x1, &pos_y1, &pos_x2, &pos_y2)) {
        return NULL;
    }
    draw_line(self, pos_x1, pos_y1, pos_x2, pos_y2);

    Py_INCREF(Py_None);
    return Py_None;
}

PyObject *ili_draw_circle(ILIObject *self, PyObject *args) {
    int pos_x, pos_y, radius;
    if (!PyArg_ParseTuple(args, "III", &pos_x, &pos_y, &radius)) {
        return NULL;
    }
    draw_circle(self, pos_x, pos_y, radius);

    Py_INCREF(Py_None);
    return Py_None;
}

PyObject *ili_draw_arc(ILIObject *self, PyObject *args) {
    int pos_x, pos_y, radius, start, end;
    if (!PyArg_ParseTuple(args, "IIIII", &pos_x, &pos_y, &radius, &start, &end)) {
        return NULL;
    }
    draw_arc(self, pos_x, pos_y, radius, start, end);

    Py_INCREF(Py_None);
    return Py_None;
}

int is_jpeg(char* filename) {
    char *dot = strrchr(filename, '.');
    if (dot && (!strcmp(dot, ".jpeg") || !strcmp(dot, ".jpg"))) {
        return 1;
    }

    return 0;
}

int is_png(char* filename) {
    char *dot = strrchr(filename, '.');
    if (dot && (!strcmp(dot, ".png"))) {
        return 1;
    }

    return 0;
}

PyObject *ili_draw_image(ILIObject *self, PyObject *args) {
    PyObject *o;
    char *filename;
    int pos_x, pos_y;
    FILE * image;

    if (PyObject_Length(args) == 3) {
        if (PyUnicode_Check(PyTuple_GetItem(args, 2))) {
            if (!PyArg_ParseTuple(args, "IIs", &pos_x, &pos_y, &filename)) {
                return NULL;
            }
            if ((image = fopen(filename, "rb")) == NULL) {
                PyErr_Format(PyExc_AttributeError, "can't open file %s.", filename);
                return NULL;
            }
            if (is_jpeg(filename)) {
                draw_jpeg_file_image(self, pos_x, pos_y, image);
            } else if (is_png(filename)) {
                draw_png_file_image(self, pos_x, pos_y, image);
            } else {
                PyErr_Format(PyExc_AttributeError, "unsupported file %s.", filename);
                return NULL;
            }
            fclose(image);
        } else {
            if (!PyArg_ParseTuple(args, "IIO", &pos_x, &pos_y, &o)) {
                return NULL;
            }
            PyObject *image = PyObject_CallMethodObjArgs(o, PyUnicode_FromString("convert"), PyUnicode_FromString("RGB"), NULL);
            draw_object_image(self, pos_x, pos_y, image);
//            Py_DECREF(o);
            Py_DECREF(image);
        }
    }

    Py_INCREF(Py_None);
    return Py_None;
}