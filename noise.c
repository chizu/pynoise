#include <Python.h>
#include "include/noise.h"

float dot2(const double * m, double2 n) {
	return m[0]*n.x + m[1]*n.y;
}

static PyObject *
snoise2d(PyObject *self, PyObject *args) {
	double x, y;
	if (!PyArg_ParseTuple(args, "d|d", &x, &y))
		return NULL;

	// Noise contributions from the three corners
	double n0, n1, n2;

	// Skew the input space to determine which simplex cell we're in
	double F2 = 0.5*(sqrt(3.0) - 1.0);
	double s = (x + y) * F2;
	int i = floor(x+s);
	int j = floor(y+s);

	double G2 = (3.0 - sqrt(3.0))/6.0;
	double t = (i+j) * G2;
	double X0 = i-t; // Unskew the cell origin back to (x,y) space
	double Y0 = j-t;
	// The x,y distances from the cell origin
	double2 zero = {{x-X0, y-Y0}};

	// For the 2D case, the simplex shape is an equilateral triangle
	// Determine which simplex we are in.
	int i1, j1;
	if (zero.x>zero.y) {
		i1=1; j1=0;
	}
	else {
		i1=0; j1=1;
	}

	double2 one = {{zero.x - i1 + G2, 
					zero.y - j1 + G2}};
	double2 two = {{zero.x - 1.0 + 2.0 * G2,	
					zero.y - 1.0 + 2.0 * G2}};

	// Work out the hashed gradient indices of the three simplex corners
	int ii = i & 255;
	int jj = j & 255;
	int gi0 = p[ii + p[jj]] % 12;
	int gi1 = p[ii + i1 + p[jj] + j1] % 12;
	int gi2 = p[ii + i1 + p[jj] + j1] % 12;

	// Calculate the contribution from the three corners
	double t0 = 0.5 - zero.x*zero.x-zero.y*zero.y;
	if (t0 < 0) {
		n0 = 0.0;
	}
	else {
		t0 *= t0;
		n0 = t0 * t0 * dot2(GRAD3[gi0], zero);
	}

	double t1 = 0.5 - one.x*one.x-one.y*one.y;
	if(t1<0) {
		n1 = 0.0;
	}
	else {
		t1 *= t1;
		n1 = t1 * t1 * dot2(GRAD3[gi1], one);
	}

	double t2 = 0.5 - two.x*two.x-two.y*two.y;
	if(t2<0) {
		n2 = 0.0;
	}
	else {
		t2 *= t2;
		n2 = t2 * t2 * dot2(GRAD3[gi2], two);
	}

	return Py_BuildValue("d", 70 * (n0 + n1 + n2));
}

PyMethodDef methods[] = {
    {"snoise2d", snoise2d, METH_VARARGS, "Calculate simplex noise for a coordinate."},
    {NULL, NULL, 0, NULL}
};

PyMODINIT_FUNC 
initnoise() {
    (void) Py_InitModule("noise", methods);
}
