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

//***************************
// drawing funcions
PyObject *ili_fill_rect(ILIObject *self, PyObject *args) {
    int pos_x1, pos_x2, pos_y1, pos_y2;
    if (!PyArg_ParseTuple(args, "IIII", &pos_x1, &pos_y1, &pos_x2, &pos_y2)) {
        return NULL;
    }
    fill_rect(self, pos_x1, pos_y1, pos_x2, pos_y2);

    return Py_None;
}

PyObject *ili_draw_rect(ILIObject *self, PyObject *args) {
    int pos_x1, pos_x2, pos_y1, pos_y2;
    if (!PyArg_ParseTuple(args, "IIII", &pos_x1, &pos_y1, &pos_x2, &pos_y2)) {
        return NULL;
    }
    draw_rect(self, pos_x1, pos_y1, pos_x2, pos_y2);

    return Py_None;
}

PyObject *ili_set_color(ILIObject *self, PyObject *args) {
    int r, g, b;
    if (!PyArg_ParseTuple(args, "III", &r, &g, &b)) {
        return NULL;
    }
    set_color(self, r,g,b);

    return Py_None;
}

PyObject *ili_set_backgroundcolor(ILIObject *self, PyObject *args) {
   int r, g, b;
    if (!PyArg_ParseTuple(args, "III", &r, &g, &b)) {
        return NULL;
    }

    set_backgroundcolor(self, r,g,b);

    return Py_None;
}

PyObject *ili_draw_pixel(ILIObject *self, PyObject *args) {
    int pos_x1, pos_y1;
    if (!PyArg_ParseTuple(args, "II", &pos_x1, &pos_y1)) {
        return NULL;
    }
    draw_pixel(self, pos_x1, pos_y1);

    return Py_None;
}

PyObject *ili_draw_line(ILIObject *self, PyObject *args) {
    int pos_x1, pos_x2, pos_y1, pos_y2;
    if (!PyArg_ParseTuple(args, "IIII", &pos_x1, &pos_y1, &pos_x2, &pos_y2)) {
        return NULL;
    }
    draw_line(self, pos_x1, pos_y1, pos_x2, pos_y2);

    return Py_None;
}

PyObject *ili_draw_circle(ILIObject *self, PyObject *args) {
    int pos_x, pos_y, radius;
    if (!PyArg_ParseTuple(args, "III", &pos_x, &pos_y, &radius)) {
        return NULL;
    }
    draw_circle(self, pos_x, pos_y, radius);

    return Py_None;
}

PyObject *ili_draw_arc(ILIObject *self, PyObject *args) {
    int pos_x, pos_y, radius, start, end;
    if (!PyArg_ParseTuple(args, "IIIII", &pos_x, &pos_y, &radius, &start, &end)) {
        return NULL;
    }
    draw_arc(self, pos_x, pos_y, radius, start, end);

    return Py_None;
}