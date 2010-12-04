from distutils.core import setup, Extension

setup(name = "Noise",
      version = "1.0",
      ext_modules = [Extension("noise", ["noise.c"], include_dirs=['include'])])


