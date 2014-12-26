/**
* A system of coupled harmonic oscillators.
*
* Let N be the number of oscillators
* x and v are N length vectors.
*
* Dynamic described by
* v' = M x
* x' = I v
*
* where M is a N by N coupling matrix and I is the identity
*
* We will simulate the 3 object system below
* |------G1------G2-----G3------|
*    k1      k2     k3      k4
* For this kind of coupling we have
* Fi = - ki ( x_i - x_{i-1} ) + k_{i+1} ( x{i+1} - x_i )
* Thus M = diag( ki * x{i-1} , - (k_i + k_{i+1}) * x_i , k_{i+1} * x{i+1} )
*
* Implementation
*
* Coupling means we have to take the whole system into account
* For linear systems like the one implemented we really only have to consider the neighbours: M is tri diagonals
* So linear systems can be implemented more efficiently.
* More arbitrary couplings will require a full sparse matrix multiply
*/

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
static double dleft[N*2];
static double dright[N*2];
static double nexteuler[N*2];

/**
* just a step of the heun
* ye_{i+1} = y_i + h * f(t_i,y_i)
* y_{i+1} = y_i + h/2 *( f(t_i, y_i) + f(t_{i+1},ye_{i+1}) )
* the N, 2 stride is assumed
*/
static void heun(double *state, double *next, double dt){
    int i,j;
    dfun(state, dleft);
    for (i = 0; i < N; ++i) {
        j=i*2; // this is effectively an unrolled loop over NVAR. should use a loop?
        nexteuler[j] = state[j] + dt * dleft[j];
        nexteuler[j+1] = state[j+1] + dt * dleft[j+1];
    }
    dfun(nexteuler, dright);
    for (i = 0; i < N; ++i) {
        j=i*2;
        next[j] = state[j] + 0.5 * dt * (dleft[j] + dright[j]);
        next[j+1] = state[j+1] + 0.5 * dt * (dleft[j+1] + dright[j+1]);
    }
}

/**
* stride assumption n, 2
*/
double * sim_coupled(){
    int stride = N * 2;
    double *hist = malloc(sizeof(double) * NSTEPS  * N * 2);

    double *state = hist;
    double *next;
    int i;

    for (i = 0; i < N; ++i) {
        state[i] = INITIAL[i];
    }

    for (i = 0; i < NSTEPS - 1 ; ++i) {
        next = state + stride;
        heun(state, next, STEP);
        state = next;
    }
    return hist;
}