#include <Python.h>
#include "noise.h"

float dot2(float2 m, float2 n) {
	return m.x*n.x + m.y*n.y;
}

float dot3(float3 m, float3 n) {
	return m.x*n.x + m.y*n.y + m.z*n.z;
}

float dot4(float4 m, float4 n) {
	return m.x*n.x + m.y*n.y + m.z*n.z + m.w*n.w;
}

static PyObject *
snoise(PyObject *self, PyObject *args) {
	int x, y, octaves;
	float noise;
	if (!PyArg_ParseTuple(args, "i|i|i", &x, &y, &octaves))
		return NULL;

	// Stub for now
	
	return Py_BuildValue("i", 0);
}

PyMethodDef methods[] = {
    {"snoise", snoise, METH_VARARGS, "Calculate simplex noise for a coordinate."},
    {NULL, NULL, 0, NULL}
};

PyMODINIT_FUNC 
initnoise() {
    (void) Py_InitModule("noise", methods);
}
