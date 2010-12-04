#include <Python.h>

double noise(int x, int y) {
    int n = x + y * 57;
	n = (n << 13) ^ n;
	return (1.0 - ((n * (n * n * 15731 + 789221) + 1376312589) & 0x7fffffff) /  1073741824.0);
}

double smooth(double x, double y) {
	double corners = (noise(x-1, y-1) + noise(x+1, y-1) + noise(x-1, y+1), noise(x+1, y+1)) / 16;
	double sides = (noise(x-1, y) + noise(x+1, y) + noise(x, y-1), noise(x, y+1)) / 8;
	double center = noise(x,y)/4;
	return corners + sides + center;
}

double interpolate(double a, double b, double x) {
	// Cosine version
	double ft = x * 3.1415927;
	double f = (1 - cos(ft)) * 0.5;

	return a*(1-f) + b * f;
}

double interpolated(double x, double y) {
	int int_x = x;
	int int_y = y;
	double frac_x = x - int_x;
	double frac_y = y - int_y;
	double v1, v2, v3, v4, i1, i2;

	v1 = smooth(x,y);
	v2 = smooth(x+1,y);
	v3 = smooth(x,y+1);
	v4 = smooth(x+1,y+1);

	i1 = interpolate(v1, v2, frac_x);
	i2 = interpolate(v3, v4, frac_x);
	
	return interpolate(i1, i2, frac_y);
}

double perlin2d(double x, double y, int octaves) {
	int n;
	double value = 0;
    double amplitude = 1;
	double lacunarity = 2;
	double persistence = 0.3;

	for (n = 0; n < octaves; n++) {
		value += interpolated(x, y) * amplitude;
		x *= lacunarity;
		y *= lacunarity;
		amplitude *= persistence;
	}
	
	return value;
}

static PyObject *
pnoise2d(PyObject *self, PyObject *args) {
	double x, y;
	int octaves = 8;
	if (!PyArg_ParseTuple(args, "dd|i", &x, &y, &octaves))
		return NULL;

	return Py_BuildValue("d", perlin2d(x, y, octaves));
}

PyMethodDef methods[] = {
    {"pnoise2d", pnoise2d, METH_VARARGS, "Calculate perlin noise for a coordinate."},
    {NULL, NULL, 0, NULL}
};

PyMODINIT_FUNC 
initnoise() {
    (void) Py_InitModule("noise", methods);
}
