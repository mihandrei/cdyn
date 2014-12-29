#!/usr/bin/env python
from ctypes import *
import numpy as np

class Sim(Structure):
     pass

NVAR = 2
lib = CDLL('build/libcoupled.so')

lib.sim_init.argtypes = [c_int, POINTER(c_double), c_double]
lib.sim_init.restype = POINTER(Sim)

lib.sim_free.argtypes = [POINTER(Sim)]
lib.sim_free.restype = None

lib.sim_run.argtypes=[POINTER(Sim), POINTER(c_double), POINTER(c_double), c_int]
lib.sim_run.restype = None

class Wr:
    def __init__(self, nodes, km, stepsize):
        self.nodes = nodes
        self._km = km  # extrremly important . otherwise km gets gc collected and we are bout to pass pointers to it into c
        if km.shape != (nodes, nodes):
            raise ValueError()

        km = np.ascontiguousarray(km)
        km = km.ctypes.data_as(POINTER(c_double))

        self._s = lib.sim_init(nodes, km, stepsize)


    def run(self, out, initial, nsteps):
        if initial.shape != (self.nodes, NVAR):
            raise ValueError()
        if out.shape != (nsteps, self.nodes, NVAR):
            raise ValueError()

        out = np.ascontiguousarray(out)
        out = out.ctypes.data_as(POINTER(c_double))

        initial = np.ascontiguousarray(initial)
        initial = initial.ctypes.data_as(POINTER(c_double))
        lib.sim_run(self._s, out, initial, nsteps)
    #
    # def __del__(self):
    #     lib.sim_free(self._s)

def test():
    w = Wr(3,
           np.array([ [-2.0, 1.0, 0.0],
                      [1.0, -2.0, 1.0],
                      [0.0, 1.0, -2.0]]),
           0.1)
    nt = 1024 
    out = np.zeros((nt, 3, NVAR))
    for i in xrange(10000): # 10M point
        w.run(out,
              np.array([[1.0, 0.0], [0.0, 0.0], [0.0, 0.0]]),
              nt)
    #print out
    #import matplotlib.pyplot as plt
    #plt.plot(out[:, 1, 0])
    #plt.show()

if __name__ == '__main__':
    test()
