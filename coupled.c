#include <stdlib.h>
#include "coupled.h"

/**
* state shape = N, 2
* dstate shape = N, 2
* there are 2 doubles x, v for each node
* the 2 stride is assumed in the code below
*/
static void dfun(double *state, double *dstate) {
    int i, j;
    double kl, kr;
    for (i = 1; i < N - 1; ++i) {
        j = i * 2;
        kl = k[i];
        kr = k[i + 1];
        dstate[j] = state[j + 1]; // x' = v
        // and now the coupled variable
        dstate[j + 1] = kl * state[j - 2] - (kl + kr) * state[j] + kr * state[j + 2];
    }
    //left edge
    kl = k[0];
    kr = k[1];
    dstate[0] = state[1];
    dstate[1] = -(kl + kr) * state[0] + kr * state[2];
    //right edge
    i = N - 1;
    j = i * 2;
    kl = k[i];
    kr = k[i + 1];
    dstate[j] = state[j + 1];
    dstate[j + 1] = kl * state[j - 2] - (kl + kr) * state[j];
}


/**
* oh heun
* we'll need some temporary buffers
* We allocate them as globals once before invoking heun to avoid malloc cost on every call
* As N might be big the stack is not an option
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
    dfun(state, dleft);
    for (i = 0; i < N * NVAR; i += NVAR) {
        for (nv = 0; nv < NVAR; ++nv) {
            nexteuler[i + nv] = state[i + nv] + dt * dleft[i + nv];
        }
    }
    dfun(nexteuler, dright);
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