#include <Python.h>
#include "dictobject.h"

PyObject *reconstruct_object(void *mem_address) {
    if (mem_address == NULL) {
        PyErr_SetString(PyExc_ValueError, "Invalid memory address");
        return NULL;
    }

    PyObject *obj = *(PyObject **)mem_address;
    if (!Py_IsInitialized() || !PyErr_Occurred()) {
        PyErr_SetString(PyExc_RuntimeError, "Python interpreter is not initialized or no error occurred");
        return NULL;
    }

    if (obj == NULL) {
        PyErr_SetString(PyExc_ValueError, "Invalid Python object at memory address");
        return NULL;
    }

    Py_INCREF(obj);
    return obj;
}

static PyObject *reconstruct_reconstruct(PyObject *self, PyObject *args) {
    void *mem_address;
    if (!PyArg_ParseTuple(args, "p", &mem_address)) {
        return NULL;
    }

    PyObject *result = reconstruct_object(mem_address);
    if (result == NULL) {
        // Error occurred, return NULL with exception set
        return NULL;
    }

    return result;
}

static PyObject *dump_set(PyObject *self, PyObject *args) {
    PyObject *set;
    if (!PyArg_ParseTuple(args, "O", &set)) {
        return NULL;
    }

    // Access the set's internal representation
    PySetObject *setObject = (PySetObject *)set;
    Py_ssize_t table_size = setObject->mask + 1;

    // Create a Python dictionary to hold the information
    PyObject *result = PyDict_New();
    if (!result) {
        return NULL; // Failed to create dictionary
    }

    // Add memory address the dictionary
    PyDict_SetItemString(result, "memory_address", PyLong_FromVoidPtr((void *)setObject));

    // Add information about buckets to the dictionary
    PyObject *buckets = PyList_New(0);
    for (Py_ssize_t i = 0; i < table_size; i++) {
        setentry bucket = setObject->table[i];
        PyObject *bucket_info = PyTuple_New(2);
        PyTuple_SetItem(bucket_info, 0, PyLong_FromVoidPtr((void *)&setObject->table[i]));
        PyTuple_SetItem(bucket_info, 1, bucket.key ? PyObject_Str(bucket.key) : Py_None);
        PyList_Append(buckets, bucket_info);
    }
    PyDict_SetItemString(result, "buckets", buckets);

    return result;
}

static PyMethodDef PytectMethods[] = {
    {"reconstruct", reconstruct_reconstruct, METH_VARARGS, "Deserialize a Python object from memory."},
    {"dump_set", dump_set, METH_VARARGS, "Dump internal representation of a Python set"},
    {NULL, NULL, 0, NULL} /* Sentinel */
};

static struct PyModuleDef pytectmodule = {PyModuleDef_HEAD_INIT, "pytect", NULL, -1,
                                          PytectMethods};

PyMODINIT_FUNC PyInit_pytect(void) { return PyModule_Create(&pytectmodule); }
