#include <stdlib.h>
#include "coupled.h"

struct Sim{
    int n;
    double const *km;
    double step;

    //we'll need some temporary buffers for heun. used to avoid malloc cost for each heun call
    double *dleft;
    double *dright;
    double *nexteuler;
};

struct Sim* sim_init(int nodes, const double *km, double step){
    struct Sim *s = malloc(sizeof(struct Sim));
    s->n = nodes;
    s->km = km;
    s->step = step;
    s->dleft = malloc(sizeof(double) * nodes * NVAR);
    s->dright= malloc(sizeof(double) * nodes * NVAR);
    s->nexteuler= malloc(sizeof(double) * nodes * NVAR);
    return s;
}

void sim_free(struct Sim *s){
    free(s->dleft);
    free(s->dright);
    free(s->nexteuler);
    free(s);
}

/**
* implements a coupled oscillator net
* relies on the km coupling matrix
* x' = v
* v' = km * x
*/
static void dfunm(struct Sim *s, double *state, double *dstate){
    int i, j, l;
    for (i = 0; i < s->n; ++i) {
        j = i * 2; // hardcoded stride NVAR 2
        dstate[j] = state[j + 1]; // x'
        dstate[j + 1] = 0; // y'
        for (l = 0; l < s->n; ++l) {
            dstate[j + 1] += s->km[i * s->n + l] * state[2*l];
        }
    }
}

/**
* just a step of the heun
* ye_{i+1} = y_i + h * f(t_i,y_i)
* y_{i+1} = y_i + h/2 *( f(t_i, y_i) + f(t_{i+1},ye_{i+1}) )
* the N, NVAR stride is assumed
*/
static void heun(struct Sim *s, double *state, double *next, double dt) {
    int i, nv;
    dfunm(s, state, s->dleft);
    for (i = 0; i < s->n * NVAR; i += NVAR) {
        for (nv = 0; nv < NVAR; ++nv) {
            s->nexteuler[i + nv] = state[i + nv] + dt * s->dleft[i + nv];
        }
    }
    dfunm(s, s->nexteuler, s->dright);
    for (i = 0; i < s->n * NVAR; i+= NVAR) {
        for (nv = 0; nv < NVAR; ++nv) {
            next[i + nv] = state[i + nv] + 0.5 * dt * (s->dleft[i + nv] + s->dright[i + nv]);
        }
    }
}

/**
* stride assumption N, 2
*/
double *sim_coupled(struct Sim *s, const double *initial, int nsteps) {
    int stride = s->n * NVAR;
    double *hist = malloc(sizeof(double) * nsteps * s->n * NVAR);

    double *state = hist;
    double *next;
    int i;

    for (i = 0; i < s->n * NVAR; ++i) {
        state[i] = initial[i];
    }

    for (i = 0; i < nsteps - 1; ++i) {
        next = state + stride;
        heun(s, state, next, s->step);
        state = next;
    }
    return hist;
}