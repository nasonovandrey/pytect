#include <Python.h>

#define PY_ABS(x) ((x) < 0 ? -(x) : (x))

static PyObject* get_int_info(PyObject* self, PyObject* args) {
    PyObject* obj;

    // Parse the argument as a Python object
    if (!PyArg_ParseTuple(args, "O", &obj)) {
        return NULL;
    }

    // Check if the object is an integer
    if (!PyLong_Check(obj)) {
        PyErr_SetString(PyExc_TypeError, "Argument must be an integer");
        return NULL;
    }

    // Access the PyLongObject structure
    PyLongObject* pyint = (PyLongObject*)obj;

    // Access the fields of the PyLongObject structure
    Py_ssize_t ob_size = pyint->ob_base.ob_size;
    digit* ob_digit = pyint->ob_digit;

    // Print information about the integer
    printf("Integer Info:\n");
    printf("Size: %zd\n", ob_size);
    printf("Digits: ");
    for (Py_ssize_t i = 0; i < PY_ABS(ob_size); i++) {
        printf("%u ", (unsigned int)ob_digit[i]);
    }
    printf("\n");

    // Return None
    Py_RETURN_NONE;
}

static PyMethodDef methods[] = {
    {"get_int_info", get_int_info, METH_VARARGS, "Get information about an integer object."},
    {NULL, NULL, 0, NULL} // Sentinel
};

static struct PyModuleDef module = {
    PyModuleDef_HEAD_INIT,
    "int_inspector",
    NULL,
    -1,
    methods
};

PyMODINIT_FUNC PyInit_int_inspector(void) {
    return PyModule_Create(&module);
}

