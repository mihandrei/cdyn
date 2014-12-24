
struct State{
    double x;
    double vx;
};

struct Hist {
    int len;
    struct State * h;
};

struct Hist sim(struct State initial, double time_interv, double dt);
double * timeline(double time_interv, double dt);

void del_hist(struct Hist *pHist);