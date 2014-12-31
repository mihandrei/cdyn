from flask import Flask
from flask import render_template

from cty import Wr, NVAR
import numpy as np
import json

app = Flask(__name__)
nt = 1024

# 3x2 oscillator array. oscillates in z 'direction'
cm32 = np.array([[-3.0, 1.0, 0.0, 1.0, 0.0, 0.0],
                 [1.0, -3.0, 1.0, 0.0, 1.0, 0.0],
                 [0.0, 1.0, -3.0, 1.0, 0.0, 1.0],
                 [1.0, 0.0, 1.0, -3.0, 1.0, 0.0],
                 [0.0, 1.0, 0.0, 1.0, -3.0, 1.0],
                 [0.0, 0.0, 1.0, 0.0, 1.0, -3.0]])

ini32 = np.array([[-1.1, 0.0], [0.0, 0.0], [0.0, 0.0], [0.0, 0.0], [0.0, 0.0], [0.0, 0.0]])

# 6 linear coupled oscillators
cm6l = np.array([[-2.0, 1.0, 0.0, 0.0, 0.0, 0.0],
                 [1.0, -2.0, 1.0, 0.0, 0.0, 0.0],
                 [0.0, 1.0, -2.0, 1.0, 0.0, 0.0],
                 [0.0, 0.0, 1.0, -2.0, 1.0, 0.0],
                 [0.0, 0.0, 0.0, 1.0, -2.0, 1.0],
                 [0.0, 0.0, 0.0, 0.0, 0.0, -2.0]])


@app.route("/")
def home():
    return render_template('draw.html')


@app.route("/step")
def step():
    w_lin.run(out_lin, ini32, nt)
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