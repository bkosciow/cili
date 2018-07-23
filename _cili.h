
char init_display_docstring[] =
    "init_display()";
char fill_rect_docstring[] =
    "fill_rect(x, y, x1, y1)";
char set_color_docstring[] =
    "set_color";
char set_backgroundcolor_docstring[] =
    "set_backgroundcolor";
char draw_pixel_docstring[] =
    "draw_pixel(x, y)";
char draw_line_docstring[] =
    "draw_line(x, y, x1, y1)";
char draw_circle_docstring[] =
    "draw_circle(x, y, radius)";
char draw_arc_docstring[] =
    "draw_arc(x, y, radius, start, end)";
char draw_rect_docstring[] =
    "draw_rect(x, y, x1, y1)";
char draw_image_docstring[] =
    "draw_image(x, y, PIL Image)";

int ili_init(ILIObject *self, PyObject *args);
void ili_dealloc(ILIObject* self);
PyObject *ili_new(PyTypeObject *type, PyObject *args, PyObject *kwds);

PyObject *ili_init_display(ILIObject *self);
PyObject *ili_fill_rect(ILIObject *self, PyObject *args);
PyObject *ili_set_color(ILIObject *self, PyObject *args);
PyObject *ili_set_backgroundcolor(ILIObject *self, PyObject *args);
PyObject *ili_draw_pixel(ILIObject *self, PyObject *args);
PyObject *ili_draw_line(ILIObject *self, PyObject *args);
PyObject *ili_draw_circle(ILIObject *self, PyObject *args);
PyObject *ili_draw_arc(ILIObject *self, PyObject *args);
PyObject *ili_draw_rect(ILIObject *self, PyObject *args);
PyObject *ili_draw_image(ILIObject *self, PyObject *args);

PyObject *magic_get_width(ILIObject *self, void *closure);
PyObject *magic_get_height(ILIObject *self, void *closure);
PyObject *magic_get_color(ILIObject *self, void *closure);
int *magic_set_color(ILIObject *self, PyObject *value, void *closure);
PyObject *magic_get_background_color(ILIObject *self, void *closure);
int *magic_set_background_color(ILIObject *self, PyObject *value, void *closure);
PyObject *magic_get_auto_flush(ILIObject *self, void *closure);
int *magic_set_auto_flush(ILIObject *self, PyObject *value, void *closure);
PyObject *magic_get_rotation(ILIObject *self, void *closure);
int *magic_set_rotation(ILIObject *self, PyObject *value, void *closure);
PyObject *magic_get_transparency_color(ILIObject *self, void *closure);
int *magic_set_transparency_color(ILIObject *self, PyObject *value, void *closure);

PyMethodDef module_methods[] = {
    {"init", (PyCFunction)ili_init_display, METH_VARARGS, init_display_docstring},
    {"fill_rect", (PyCFunction)ili_fill_rect, METH_VARARGS, fill_rect_docstring},
    {"set_color", (PyCFunction)ili_set_color, METH_VARARGS, set_color_docstring},
    {"set_backgroundcolor", (PyCFunction)ili_set_backgroundcolor, METH_VARARGS, set_backgroundcolor_docstring},
    {"draw_pixel", (PyCFunction)ili_draw_pixel, METH_VARARGS, draw_pixel_docstring},
    {"draw_line", (PyCFunction)ili_draw_line, METH_VARARGS, draw_line_docstring},
    {"draw_circle", (PyCFunction)ili_draw_circle, METH_VARARGS, draw_circle_docstring},
    {"draw_arc", (PyCFunction)ili_draw_arc, METH_VARARGS, draw_arc_docstring},
    {"draw_rect", (PyCFunction)ili_draw_rect, METH_VARARGS, draw_rect_docstring},
    {"draw_image", (PyCFunction)ili_draw_image, METH_VARARGS, draw_image_docstring},
    {NULL, NULL, 0, NULL}
};

PyGetSetDef ili_getset[] = {
    {"width", (getter)magic_get_width, NULL, "width", NULL},
    {"height", (getter)magic_get_height, NULL, "height", NULL},
    {"color", (getter)magic_get_color, (setter)magic_set_color, "color", NULL},
    {"background_color", (getter)magic_get_background_color, (setter)magic_set_background_color, "background", NULL},
    {"auto_flush", (getter)magic_get_auto_flush, (setter)magic_set_auto_flush, "auto_flush", NULL},
    {"rotation", (getter)magic_get_rotation, (setter)magic_set_rotation, "rotation", NULL},
    {"transparency_color", (getter)magic_get_transparency_color, (setter)magic_set_transparency_color, "transparency_color", NULL},
    {NULL}
};
