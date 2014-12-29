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
*/

#define N 3
#define NVAR 2
#define NSTEPS 2000
#define STEP 0.1

static const double INITIAL[] = {1.0, 0.0,  0.0, 0.0, 0.0, 0.0};

// coupling matrix. a adjacency matrix. the values are not k's, derived from them!
static const double km[N][N] = {
        {-2.0, 1.0, 0.0},
        {1.0, -2.0, 1.0},
        {0.0, 1.0, -2.0},
};

#ifdef __cplusplus
extern "C"{
#endif

extern double * sim_coupled();

#ifdef __cplusplus
}
#endif
