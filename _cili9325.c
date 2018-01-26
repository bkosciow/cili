#include <Python.h>
#include "ili.h"
#include "_cili.h"

static char module_docstring[] =
    "GfxLCD ILI9325 in C";

static PyTypeObject ILIType = {
    PyVarObject_HEAD_INIT(NULL, 0)
    "cili9325.ili",             /* tp_name */
    sizeof(ILIObject),          /* tp_basicsize */
    0,                          /* tp_itemsize */
    (destructor)ili_dealloc,    /* tp_dealloc */
    0,                          /* tp_print */
    0,                         /* tp_getattr */
    0,                         /* tp_setattr */
    0,                         /* tp_reserved */
    0,                         /* tp_repr */
    0,                         /* tp_as_number */
    0,                         /* tp_as_sequence */
    0,                         /* tp_as_mapping */
    0,                         /* tp_hash  */
    0,                         /* tp_call */
    0,                         /* tp_str */
    0,                         /* tp_getattro */
    0,                         /* tp_setattro */
    0,                         /* tp_as_buffer */
    Py_TPFLAGS_DEFAULT |
        Py_TPFLAGS_BASETYPE,   /* tp_flags */
    module_docstring,          /* tp_doc */
    0,                         /* tp_traverse */
    0,                         /* tp_clear */
    0,                         /* tp_richcompare */
    0,                         /* tp_weaklistoffset */
    0,                         /* tp_iter */
    0,                         /* tp_iternext */
    module_methods,            /* tp_methods */
    0,                         /* tp_members */
    ili_getset,                /* tp_getset */
    0,                         /* tp_base */
    0,                         /* tp_dict */
    0,                         /* tp_descr_get */
    0,                         /* tp_descr_set */
    0,                         /* tp_dictoffset */
    (initproc)ili_init,        /* tp_init */
    0,                         /* tp_alloc */
    ili_new,                    /* tp_new */
};

PyMODINIT_FUNC PyInit_cili9325(void) {
    PyObject *module;

    if (PyType_Ready(&ILIType) < 0)
        return NULL;

    static struct PyModuleDef moduledef = {
        PyModuleDef_HEAD_INIT,
        "cili9325",
        module_docstring,
        -1,
        NULL, //module_methods,
        NULL,
        NULL,
        NULL,
        NULL
    };
    module = PyModule_Create(&moduledef);
    if (!module) return NULL;

    Py_INCREF(&ILIType);
    PyModule_AddObject(module, "ili", (PyObject *)&ILIType);

    return module;
}
