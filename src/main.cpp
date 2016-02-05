#include <stdio.h>
#include <Python.h>
#include "structmember.h"

#include "astar.h"


typedef struct {
    PyObject_HEAD
        PyObject* dict;
    int count;
} Astar;

static int Astar_init(Astar *self, PyObject *args, PyObject *kwds);
static PyObject *Astar_set(Astar *self, PyObject *args);
static void Astar_dealloc(Astar *self);


static PyObject *Astar_set(Astar *self, PyObject *args)
{
    const char *key;
    PyObject *value;

    if (!PyArg_ParseTuple(args, "sO:set", &key, &value)) {
        return NULL;
    }

    if (PyDict_SetItemString(self->dict, key, value) < 0) {
        return NULL;
    }

    self->count++;

    return Py_BuildValue("i", self->count);
}

static PyObject *Astar_search(Astar *self, PyObject *args)
{
    int sx, sy, dx, dy = 0;
    int width, height = 1;
    int diagonal = 0;

    PyObject *world;

    if (!PyArg_ParseTuple(args, "iiiiOiii", &sx, &sy, &dx, &dy, &world, &width, &height, &diagonal)) {
        return NULL;
    }

    PyObject *iter = PyObject_GetIter(world);
    if (!iter) {
        // error not iterator
    }

    int i=0;
    uint32_t* map = (uint32_t*) malloc(sizeof(uint32_t) * (width * height));
    uint32_t* path = (uint32_t*) malloc(sizeof(uint32_t) * (width * height));
    //uint32_t map[width * height];
    memset(map, 0, sizeof(uint32_t) * (width * height));
    memset(path, 0, sizeof(uint32_t) * (width * height));
    
    while (true) {
        PyObject *next = PyIter_Next(iter);
        if (!next) {
            // nothing left in the iterator
            break;
        }

        char* s = PyString_AsString(next);
        map[i++] = s[0];
    }

    //printf("We got following ints\n");
    //printf("sx=%d sy=%d dx=%d dy=%d width=%d height=%d \n",sx ,sy ,dx ,dy ,width ,height);
    int a = init(sx, sy, dx, dy, map, width, height, path);
    
    PyObject *lst = PyList_New(a);
    for(i=0; i<a; i++){
        PyList_SET_ITEM(lst, i, Py_BuildValue("i", path[i]) );
    }

    free(map);
    free(path);
    return lst;
}

static int Astar_init(Astar *self, PyObject *args, PyObject *kwds) {
    self->dict = PyDict_New();
    self->count = 0;
    return 0;
}

static void Astar_dealloc(Astar *self) {
    Py_XDECREF(self->dict);
    self->ob_type->tp_free((PyObject*)self);
}

static PyMemberDef Astar_members[] =
{
    {"dict", T_OBJECT, offsetof(Astar, dict), 0, "Description"},
    {"count", T_INT, offsetof(Astar, count), 0, "Description"},
    { NULL }
};

static PyMethodDef Astar_methods[] = 
{
    { "set_params", (PyCFunction) Astar_set, METH_VARARGS, "Set Astar parameters" },
    { "search", (PyCFunction) Astar_search, METH_VARARGS, "Searchi!" },
    { NULL }
};

/* Annoying BEHOMT object for defining a 'type' */
static PyTypeObject AstarType = 
{
    PyObject_HEAD_INIT(NULL)
        0,                         /* ob_size */
    "Astar",                   /* tp_name */
    sizeof(Astar),             /* tp_basicsize */
    0,                         /* tp_itemsize */
    (destructor)Astar_dealloc, /* tp_dealloc */
    0,                         /* tp_print */
    0,                         /* tp_getattr */
    0,                         /* tp_setattr */
    0,                         /* tp_compare */
    0,                         /* tp_repr */
    0,                         /* tp_as_number */
    0,                         /* tp_as_sequence */
    0,                         /* tp_as_mapping */
    0,                         /* tp_hash */
    0,                         /* tp_call */
    0,                         /* tp_str */
    0,                         /* tp_getattro */
    0,                         /* tp_setattro */
    0,                         /* tp_as_buffer */
    Py_TPFLAGS_DEFAULT | Py_TPFLAGS_BASETYPE, /* tp_flags*/
    "Astar object",            /* tp_doc */
    0,                         /* tp_traverse */
    0,                         /* tp_clear */
    0,                         /* tp_richcompare */
    0,                         /* tp_weaklistoffset */
    0,                         /* tp_iter */
    0,                         /* tp_iternext */
    Astar_methods,             /* tp_methods */
    Astar_members,             /* tp_members */
    0,                         /* tp_getset */
    0,                         /* tp_base */
    0,                         /* tp_dict */
    0,                         /* tp_descr_get */
    0,                         /* tp_descr_set */
    0,                         /* tp_dictoffset */
    (initproc)Astar_init,      /* tp_init */
    0,                         /* tp_alloc */
    0,                         /* tp_new */
};

PyMODINIT_FUNC initpathfinding(void)
{
    PyObject* mod;
    mod = Py_InitModule("pathfinding", Astar_methods);
    // Fill in some slots in the type, and make it ready
    AstarType.tp_new = PyType_GenericNew;
    if (PyType_Ready(&AstarType) < 0) {
        return;
    }

    // Add the type to the module.
    Py_INCREF(&AstarType);
    PyModule_AddObject(mod, "Astar", (PyObject*)&AstarType);

}
