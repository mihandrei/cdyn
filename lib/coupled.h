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

#define NVAR 2

#ifdef __cplusplus
extern "C"{
#endif

/* opaque type holding sim state */
struct Sim;

struct Sim* sim_init(int nodes, const double *km, double step);
void sim_free(struct Sim *);

/* utilities */
double* sim_alloc_hist(struct Sim *s, int nsteps);
double * build_linear_coupled_array(int n);

/**
* run a chunk of the simulation. fill results in hist buffer
*/
void sim_run(struct Sim *s, double *hist, const double *initial, int nsteps);

#ifdef __cplusplus
}
#endif
