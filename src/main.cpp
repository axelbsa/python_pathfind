#include <stdio.h>
#include "uthash.h"
#include <Python.h>
#include "structmember.h"

#include "common.h"

int items_added = 0;
int fdsfdsf = 0;

static long int map[20];

int path[10][10] = {
    {1,1,1,1,1,1,1,1,1,1},
    {1,1,1,1,1,1,1,1,1,1},
    {1,1,1,1,1,1,1,1,1,1},
    {1,1,1,1,1,1,1,1,1,1},
    {1,1,1,1,1,1,1,1,1,1},
    {1,1,1,1,1,1,1,1,1,1},
    {1,1,1,1,1,1,1,1,1,1},
    {1,1,1,1,1,1,1,1,1,1},
    {1,1,1,1,1,1,1,1,1,1},
    {1,1,1,1,1,1,1,1,1,1},
};

typedef struct {
    PyObject_HEAD
    PyObject * dict;
    int count;
} CDict;

typedef struct {
    PyObject_HEAD
    PyObject * list;
    int count;
} Map;

struct Points {
    int x;
    int y;
    int id;
    int fcost;
    int hcost;
    int gcost;
    struct Points *parent;
    UT_hash_handle hh;
};

struct Points *point = NULL;

void add_user(int user_id, int x, int y) {
    struct Points *p = NULL;

    p = (struct Points*)malloc(sizeof(struct Points));
    p->id = user_id;
    p->x = 22;
    p->y = 22;
    HASH_ADD_INT( point, id, p );  /* id: name of key field */
}

void add_items() {
    int i,j;
    for(i=0; i<10; i++){
        for(j=0; j<10; j++){
            printf("%i ", path[i][j]);
            add_user(i+j, j, i);
            items_added++;
        }
        printf("\n");
    }
}



static PyObject* set_map(PyObject* self, PyObject* args) {

    int x;
    int y;
    PyObject *obj;


    if (!PyArg_ParseTuple(args, "iiO", &y, &x, &obj)) {
        // error
        PyErr_SetString(PyExc_TypeError, "set_map() takes 3 arguments");
        return NULL;
    }

    PyObject *iter = PyObject_GetIter(obj);
    if (!iter) {
        // error not iterator
        return Py_BuildValue("i", -1);
    }

    int i = 0;

    while (true) {
        PyObject *next = PyIter_Next(iter);
        if (!next) {
            // nothing left in the iterator
            break;
        }

        if (!PyInt_Check(next)) {
            // error, we were expecting a floating point value
            PyErr_SetString(PyExc_AttributeError, "expecting integer list");
            return NULL;
            //return Py_BuildValue("s", "expexting integer");
        }

        // do something with foo
        long int foo = PyInt_AsLong(next);
        map[i] = foo;

    }
    return Py_BuildValue("i", 0);
}

static PyObject* get_map(PyObject* self, PyObject* args){
    int i;
    int len = 0;

    PyObject *lst = PyList_New(len);
    for(i=0; i<20; i++){
        PyList_SET_ITEM(lst, i, Py_BuildValue("i", map[i]) );
    }
    return lst;
}

static PyObject* say_hello(PyObject* self, PyObject* args) {
	const char* name;

	if (!PyArg_ParseTuple(args, "s", &name))
		return NULL;

	printf("Hello %s!\n", name);

	Py_RETURN_NONE;
}

static PyObject* ret4(PyObject* self, PyObject* args) 
{
    int i;
    int len = 0;

	if (!PyArg_ParseTuple(args, "i", &len)) {
		return NULL;
    }

    PyObject *lst = PyList_New(len);

    for(i=0; i<len; i++){
        PyList_SET_ITEM(lst, i, Py_BuildValue("i", i) );
    }

    return lst;
	//return Py_BuildValue("i", 4);
}


static PyObject *CDict_set(CDict *self, PyObject *args)
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

static int CDict_init(CDict *self, PyObject *args, PyObject *kwds) {
    self->dict = PyDict_New();
    self->count = 0;
    return 0;
}

static void CDict_dealloc(CDict *self) {
    Py_XDECREF(self->dict);
    self->ob_type->tp_free((PyObject*)self);
}

static PyMemberDef CDict_members[] =
{
    {"dict", T_OBJECT, offsetof(CDict, dict), 0, "HABBLAHABBLA"},
    {"count", T_INT, offsetof(CDict, count), 0, "HIBBLOGIBBLO"},
    { NULL }
};

static PyMethodDef
CDict_methods[] = {
   { "set", (PyCFunction) CDict_set, METH_VARARGS, "Set a key and increment the count." },
   { NULL }
};

static PyMethodDef HelloMethods[] =
{
	{ "say_hello", say_hello, METH_VARARGS, "Greet somebody." },
	{ "ret4", ret4, METH_VARARGS, "Return random" },
	{ "set_map", set_map, METH_VARARGS, "Set a map" },
	{ "get_map", get_map, METH_VARARGS, "Set a map" },
    { "set", (PyCFunction) CDict_set, METH_VARARGS, "Set a key and increment the count." },
	{ NULL, NULL, 0, NULL }
};

static PyTypeObject CDictType = {
   PyObject_HEAD_INIT(NULL)
   0,                         /* ob_size */
   "CDict",               /* tp_name */
   sizeof(CDict),         /* tp_basicsize */
   0,                         /* tp_itemsize */
   (destructor)CDict_dealloc, /* tp_dealloc */
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
   "CDict object",        /* tp_doc */
   0,                         /* tp_traverse */
   0,                         /* tp_clear */
   0,                         /* tp_richcompare */
   0,                         /* tp_weaklistoffset */
   0,                         /* tp_iter */
   0,                         /* tp_iternext */
   CDict_methods,         /* tp_methods */
   CDict_members,         /* tp_members */
   0,                         /* tp_getset */
   0,                         /* tp_base */
   0,                         /* tp_dict */
   0,                         /* tp_descr_get */
   0,                         /* tp_descr_set */
   0,                         /* tp_dictoffset */
   (initproc)CDict_init,  /* tp_init */
   0,                         /* tp_alloc */
   0,                         /* tp_new */
};


PyMODINIT_FUNC initpathfinding(void)
{
    PyObject* mod;
    (void)Py_InitModule("pathfinding", HelloMethods);
    mod = Py_InitModule("pathfinding", CDict_methods);
    // Fill in some slots in the type, and make it ready
    CDictType.tp_new = PyType_GenericNew;
    if (PyType_Ready(&CDictType) < 0) {
        return;
    }

    // Add the type to the module.
    Py_INCREF(&CDictType);
    PyModule_AddObject(mod, "CDict", (PyObject*)&CDictType);

}
