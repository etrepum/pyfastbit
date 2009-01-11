/*
http://crd.lbl.gov/~kewu/fastbit/doc/html/group__FastBitCAPI.html
*/
#include "capi.h"

#include "Python.h"
#include "structmember.h"

#ifdef __GNUC__
#define UNUSED __attribute__((__unused__))
#else
#define UNUSED
#endif

#define DEFAULT_ENCODING "utf-8"

#define PyFastBitQuery_Check(op) PyObject_TypeCheck(op, &PyFastBitQueryType)
#define PyFastBitQuery_CheckExact(op) (Py_TYPE(op) == &PyFastBitQueryType)
#define PyFastBitResultSet_Check(op) PyObject_TypeCheck(op, &PyFastBitResultSetType)
#define PyFastBitResultSet_CheckExact(op) (Py_TYPE(op) == &PyFastBitResultSetType)

static PyTypeObject PyFastBitQueryType;
static PyTypeObject PyFastBitResultSetType;

typedef struct _PyFastBitQueryObject {
    PyObject_HEAD
    FastBitQueryHandle query;
} PyFastBitQueryObject;

typedef struct _PyFastBitResultSetObject {
    PyObject_HEAD
    FastBitResultSetHandle result_set;
} PyFastBitResultSetObject;

static void
query_dealloc(PyObject *self)
{
    /* Deallocate query object */
    PyFastBitQueryObject *s;
    assert(PyFastBitQuery_Check(self));
    s = (PyFastBitQueryObject *)self;
    if (s->query) {
        fastbit_destroy_query(s->query);
        s->query = NULL;
    }
    self->ob_type->tp_free(self);
}

static int
query_init(PyObject *self, PyObject *args, PyObject *kwds)
{
    /* Initialize Scanner object */
    char *encoding = DEFAULT_ENCODING;
    char *selectClause;
    char *indexLocation;
    char *queryConditions;
    static char *kwlist[] = {
        "selectClause", "indexLocation", "queryConditions", NULL};
    PyFastBitQueryObject *s;

    assert(PyFastBitQuery_Check(self));
    s = (PyFastBitQueryObject *)self;

    if (!PyArg_ParseTupleAndKeywords(args, kwds, "etetet:Query", kwlist, 
            encoding, &selectClause,
            encoding, &indexLocation,
            encoding, &queryConditions))
        return -1;

    s->query = fastbit_build_query(
        selectClause, indexLocation, queryConditions);

    if (!s->query) {
        goto bail;
    }

    return 0;

bail:
    PyErr_SetString(PyExc_ValueError, "fastbit_build_query failed");
    return -1;
}

PyDoc_STRVAR(query_doc, "Query");

static
PyTypeObject PyFastBitQueryType = {
    PyObject_HEAD_INIT(0)
    0,                    /* tp_internal */
    "Query",       /* tp_name */
    sizeof(PyFastBitQueryObject), /* tp_basicsize */
    0,                    /* tp_itemsize */
    query_dealloc, /* tp_dealloc */
    0,                    /* tp_print */
    0,                    /* tp_getattr */
    0,                    /* tp_setattr */
    0,                    /* tp_compare */
    0,                    /* tp_repr */
    0,                    /* tp_as_number */
    0,                    /* tp_as_sequence */
    0,                    /* tp_as_mapping */
    0,                    /* tp_hash */
    0,         /* tp_call */
    0,                    /* tp_str */
    0,/* PyObject_GenericGetAttr, */                    /* tp_getattro */
    0,/* PyObject_GenericSetAttr, */                    /* tp_setattro */
    0,                    /* tp_as_buffer */
    Py_TPFLAGS_DEFAULT,   /* tp_flags */
    query_doc,          /* tp_doc */
    0,                    /* tp_traverse */
    0,                    /* tp_clear */
    0,                    /* tp_richcompare */
    0,                    /* tp_weaklistoffset */
    0,                    /* tp_iter */
    0,                    /* tp_iternext */
    0,                    /* tp_methods */
    0,                    /* tp_members */
    0,                    /* tp_getset */
    0,                    /* tp_base */
    0,                    /* tp_dict */
    0,                    /* tp_descr_get */
    0,                    /* tp_descr_set */
    0,                    /* tp_dictoffset */
    query_init,                    /* tp_init */
    0,/* PyType_GenericAlloc, */        /* tp_alloc */
    0,/* PyType_GenericNew, */          /* tp_new */
    0,/* _PyObject_Del, */              /* tp_free */
};

static int
result_set_init(PyObject *self, PyObject *args, PyObject *kwds)
{
    /* initialize ResultSet object */
    static char *kwlist[] = {"query", NULL};

    PyFastBitResultSetObject *s;
    PyFastBitQueryObject *query_obj;

    assert(PyFastBitResultSet_Check(self));
    s = (PyFastBitResultSetObject *)self;

    s->result_set = NULL;
    
    if (!PyArg_ParseTupleAndKeywords(args, kwds, "O!:ResultSet", kwlist,
        &PyFastBitQueryType, &query_obj))
        return -1;

    s->result_set = fastbit_build_result_set(query_obj->query);
    if (!s->result_set) {
        goto bail;
    }
    
    return 0;

bail:
    PyErr_SetString(PyExc_ValueError, "fastbit_build_result_set failed");
    return -1;
}

static void
result_set_dealloc(PyObject *self)
{
    /* Deallocate ResultSet */
    PyFastBitResultSetObject *s;
    assert(PyFastBitResultSet_Check(self));
    s = (PyFastBitResultSetObject *)self;
    if (s->result_set) {
        fastbit_destroy_result_set(s->result_set);
        s->result_set = NULL;
    }
    self->ob_type->tp_free(self);
}

PyDoc_STRVAR(result_set_doc, "ResultSet");

static
PyTypeObject PyFastBitResultSetType = {
    PyObject_HEAD_INIT(0)
    0,                    /* tp_internal */
    "ResultSet",       /* tp_name */
    sizeof(PyFastBitResultSetObject), /* tp_basicsize */
    0,                    /* tp_itemsize */
    result_set_dealloc, /* tp_dealloc */
    0,                    /* tp_print */
    0,                    /* tp_getattr */
    0,                    /* tp_setattr */
    0,                    /* tp_compare */
    0,                    /* tp_repr */
    0,                    /* tp_as_number */
    0,                    /* tp_as_sequence */
    0,                    /* tp_as_mapping */
    0,                    /* tp_hash */
    0,                    /* tp_call */
    0,                    /* tp_str */
    0,/* PyObject_GenericGetAttr, */                    /* tp_getattro */
    0,/* PyObject_GenericSetAttr, */                    /* tp_setattro */
    0,                    /* tp_as_buffer */
    Py_TPFLAGS_DEFAULT,   /* tp_flags */
    result_set_doc,          /* tp_doc */
    0,                    /* tp_traverse */
    0,                    /* tp_clear */
    0,                    /* tp_richcompare */
    0,                    /* tp_weaklistoffset */
    0,                    /* tp_iter */
    0,                    /* tp_iternext */
    0,                    /* tp_methods */
    0,                    /* tp_members */
    0,                    /* tp_getset */
    0,                    /* tp_base */
    0,                    /* tp_dict */
    0,                    /* tp_descr_get */
    0,                    /* tp_descr_set */
    0,                    /* tp_dictoffset */
    result_set_init,                    /* tp_init */
    0,/* PyType_GenericAlloc, */       /* tp_alloc */
    0,/* PyType_GenericNew, */         /* tp_new */
    0,/* _PyObject_Del, */             /* tp_free */
};

static PyMethodDef speedups_methods[] = {
    {NULL, NULL, 0, NULL}
};

PyDoc_STRVAR(module_doc,
"FastBit C API\n");

void
init_pyfastbit(void)
{
    PyObject *m;
    PyFastBitQueryType.tp_new = PyType_GenericNew;
    PyFastBitResultSetType.tp_new = PyType_GenericNew;
    if (PyType_Ready(&PyFastBitQueryType) < 0)
        return;
    if (PyType_Ready(&PyFastBitResultSetType) < 0)
        return;
    m = Py_InitModule3("_pyfastbit", speedups_methods, module_doc);
    Py_INCREF((PyObject*)&PyFastBitQueryType);
    PyModule_AddObject(m, "Query", (PyObject*)&PyFastBitQueryType);
    Py_INCREF((PyObject*)&PyFastBitResultSetType);
    PyModule_AddObject(m, "ResultSet", (PyObject*)&PyFastBitResultSetType);
}
