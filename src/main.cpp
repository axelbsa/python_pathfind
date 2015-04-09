#include <stdio.h>
#include "uthash.h"
#include <Python.h>

int items_added = 0;

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

static PyObject* ret4(PyObject* self, PyObject* args) {
    int i;
    int len = 0;
	if (!PyArg_ParseTuple(args, "i", &len))
		return NULL;
    PyObject *lst = PyList_New(len);
    for(i=0; i<len; i++){
        PyList_SET_ITEM(lst, i, Py_BuildValue("i", i) );
    }
    return lst;
	//return Py_BuildValue("i", 4);
}

static PyMethodDef HelloMethods[] =
{
	{ "say_hello", say_hello, METH_VARARGS, "Greet somebody." },
	{ "ret4", ret4, METH_VARARGS, "Return random" },
	{ "set_map", set_map, METH_VARARGS, "Set a map" },
	{ "get_map", get_map, METH_VARARGS, "Set a map" },
	{ NULL, NULL, 0, NULL }
};

PyMODINIT_FUNC initastar(void)
{
	(void)Py_InitModule("astar", HelloMethods);
}
