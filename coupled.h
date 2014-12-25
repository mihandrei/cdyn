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

#define N 3
#define NVAR 2
#define NSTEPS 800

double * sim_coupled();