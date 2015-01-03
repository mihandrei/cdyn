from flask import Flask
from flask import render_template

from cty import Wr, NVAR
import numpy as np
import json
import cProfile

app = Flask(__name__)
nt = 2*1024

def build_linear_coupled_array(n):
    d = -2.0 * np.ones(n)
    d2 = 1.0 * np.ones(n - 1)
    return np.diag(d) + np.diag(d2, 1) + np.diag(d2, -1)

def build_grid_coupled_array(n, m):
    d = -4.0 * np.ones(n * m)
    d2 = 1.0 * np.ones(n * m - 1)
    d2[n-1::n] = 0
    dv = 1.0 * np.ones(n*m - n)
    ret = np.diag(d) + np.diag(d2, 1) + np.diag(d2, -1)
    ret += np.diag(dv, n) + np.diag(dv, -n)
    return ret

def profile_func(func):
    def wrapper(*args, **kwargs):
        profile_file = func.__name__ + ".profile"
        print("profiling function %s. Profile stored in %s" % (func.__name__, profile_file))
        prof = cProfile.Profile()
        ret = prof.runcall(func, *args, **kwargs)
        prof.dump_stats(profile_file)
        return ret
    return wrapper

# oscillator grid. oscillates in z 'direction'
ini32 = np.zeros((20 * 20, NVAR))
ini32[0] = [2.0, 0.0]
cm32 = build_grid_coupled_array(20, 20)

# linear coupled oscillators
ini_l = np.zeros((200, NVAR))
ini_l[0:10] = [2.0, 0.0]
ini_l[100] = [-2.0, 0.0]
cm6l = build_linear_coupled_array(200)

@app.route("/")
def home():
    return render_template('draw.html')

@app.route("/step")
# @profile_func
def step():
    w_lin.run(out_lin, ini_l, nt)
    w_grid.run(out_grid, ini32, nt)
    return json.dumps({
        'lin': out_lin.tolist(),
        'grid': out_grid.tolist()
    })

if __name__ == "__main__":
    w_lin = Wr(cm6l, 0.1)
    w_grid = Wr(cm32, 0.1)

    out_lin = np.zeros((nt, len(cm6l), NVAR))
    out_grid = np.zeros((nt, len(cm32), NVAR))

    app.debug = True
    app.run()