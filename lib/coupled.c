#include <stdlib.h>
#include "coupled.h"


/**
* implements a coupled oscillator net
* relies on the km coupling matrix
* x' = v
* v' = km * x
*/
static void dfunm(double *state, double *dstate){
    int i, j, l;
    for (i = 0; i < N; ++i) {
        j = i * 2; // hardcoded stride NVAR 2
        dstate[j] = state[j + 1]; // x'
        dstate[j + 1] = 0; // y'
        for (l = 0; l < N; ++l) {
            dstate[j + 1] += km[i][l] * state[2*l];
        }
    }
}

/**
* oh heun
* we'll need some temporary buffers
* We allocate them as globals once before invoking heun to avoid malloc cost on every call
* As N might be big the stack is not an option
* N will need to be dynamic. Then a init integrator fun is needed to allocate these.
*/
static double dleft[N * NVAR];
static double dright[N * NVAR];
static double nexteuler[N * NVAR];

/**
* just a step of the heun
* ye_{i+1} = y_i + h * f(t_i,y_i)
* y_{i+1} = y_i + h/2 *( f(t_i, y_i) + f(t_{i+1},ye_{i+1}) )
* the N, NVAR stride is assumed
*/
static void heun(double *state, double *next, double dt) {
    int i, nv;
    dfunm(state, dleft);
    for (i = 0; i < N * NVAR; i += NVAR) {
        for (nv = 0; nv < NVAR; ++nv) {
            nexteuler[i + nv] = state[i + nv] + dt * dleft[i + nv];
        }
    }
    dfunm(nexteuler, dright);
    for (i = 0; i < N * NVAR; i+= NVAR) {
        for (nv = 0; nv < NVAR; ++nv) {
            next[i + nv] = state[i + nv] + 0.5 * dt * (dleft[i + nv] + dright[i + nv]);
        }
    }
}

/**
* stride assumption N, 2
*/
double *sim_coupled() {
    int stride = N * NVAR;
    double *hist = malloc(sizeof(double) * NSTEPS * N * NVAR);

    double *state = hist;
    double *next;
    int i;

    for (i = 0; i < N * NVAR; ++i) {
        state[i] = INITIAL[i];
    }

    for (i = 0; i < NSTEPS - 1; ++i) {
        next = state + stride;
        heun(state, next, STEP);
        state = next;
    }
    return hist;
}