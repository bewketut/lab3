double **dmatrix(int nrl, int nrh, int ncl, int nch);
double *dvector(int nl, int nh);
void free_dvector(double *v, int nl, int nh);
void free_dmatrix(double **m, int nrl, int nrh, int ncl, int nch);

double sinsin(double a, double b, double ki, double kj);
double sincos(double a, double b, double ki, double kj);
double intsin( double a, double b, double k );

