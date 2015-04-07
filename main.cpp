#include <Python.h>

static PyObject* say_hello(PyObject* self, PyObject* args)
{
	const char* name;

	if (!PyArg_ParseTuple(args, "s", &name))
		return NULL;

	printf("Hello %s!\n", name);

	Py_RETURN_NONE;
}

static PyObject* ret4(PyObject* self, PyObject* args)
{
	Py_RETURN_NONE;
}

static PyMethodDef HelloMethods[] =
{
	{ "say_hello", say_hello, METH_VARARGS, "Greet somebody." },
	{ "ret4", ret4, METH_VARARGS, "Return random" },
	{ NULL, NULL, 0, NULL }
};

PyMODINIT_FUNC initastar(void)
{
	(void)Py_InitModule("astar", HelloMethods);
}