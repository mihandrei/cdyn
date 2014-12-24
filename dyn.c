#include <stdlib.h>
#include "dyn.h"

/*dampened v' = -kx - gv; v = x'*/
void dfun(struct State *src, struct State *dest){
    double k = 9.0;
    dest->x = src->vx;
    dest->vx = - k * src->x - src->vx;
}

// euler
void integrate(struct State initial, int nsteps, double dt, struct State *h){
    struct State d;
    struct State s;
    s = initial;

    int i;
    for (i = 0; i < nsteps; i++){
        dfun(&s, &d);
        s.x += d.x * dt;
        s.vx += d.vx * dt;
        h[i] = s;
    }
}

struct Hist sim(struct State initial, double time_interv, double dt){
    if (time_interv <= 0 || dt <= 0 || dt >= time_interv) goto fail;
    struct Hist hist;
    int nsteps = (int) (time_interv / dt);
    hist.h = malloc(sizeof(struct State) * (nsteps + 1));
    if (hist.h == NULL) goto fail;
    hist.len = nsteps + 1;
    hist.h[0] = initial;

    integrate(initial, nsteps, dt, hist.h + 1);

    return hist;

    fail:
        hist.len = 0;
        hist.h = NULL;
        return hist;
}

double * timeline(double time_interv, double dt){
    int nsteps = (int) (time_interv / dt);
    double *ret = malloc(sizeof(double) * (nsteps + 1));
    if(ret == NULL) return NULL;

    int i;
    for (i = 0; i < nsteps + 1; i++){
        ret[i] = i*dt;
    }
    return ret;
}

void del_hist(struct Hist *pHist) {
    free(pHist->h);
}
