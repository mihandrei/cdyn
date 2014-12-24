#include <stdlib.h>
#include "dyn.h"

/*dampened v' = -kx - gv; v = x'*/
void dfun(struct State *src, struct State *dest){
    double k = 9.0;
    double damp = 0.0;
    dest->x = src->vx;
    dest->vx = - k * src->x - damp * src->vx;
}


void euler(struct State initial, int nsteps, double dt, struct State *h){
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

//ye_{i+1} = y_i + h * f(t_i,y_i)
//
//y_{i+1} = y_i + h/2 *( f(t_i, y_i) + f(t_{i+1},ye_{i+1}) )

void heun(struct State initial, int nsteps, double dt, struct State *h){
    struct State de;
    struct State dn;

    struct State yi;
    struct State ye;

    yi = initial;

    int i;
    for (i = 0; i < nsteps; i++){
        dfun(&yi, &de); // euler prediction

        ye.x = yi.x + dt * de.x;
        ye.vx = yi.vx + dt * de.vx;

        dfun(&ye, &dn); // eval in predicted point

        yi.x += 0.5 * dt * (de.x + dn.x);   // correct the euler estimate by averaging
        yi.vx += 0.5 * dt * (de.vx + dn.vx);

        h[i] = yi;
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

    euler(initial, nsteps, dt, hist.h + 1);

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
