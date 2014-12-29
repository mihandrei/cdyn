#!/usr/bin/env python
from ctypes import c_double, CDLL

lib = CDLL('../concrete_osc-build/libcoupled.so')
lib.sim_coupled.restype = c_double*100
d = lib.sim_coupled()
print list(d)
