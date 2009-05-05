#include <stdlib.h>
#include <assert.h>
#include <math.h>

#define NR_END 1
#define FREE_ARG char*

#define s(x) sin(x)
#define c(x) cos(x)

#define nil 0

double **dmatrix(int nrl, int nrh, int ncl, int nch)
/* allocate a double matrix with subscript range m[nrl..nrh][ncl..nch] */
{
	int i, nrow=nrh-nrl+1,ncol=nch-ncl+1;
	double **m;

	/* allocate pointers to rows */
	m=(double **) malloc((size_t)((nrow+NR_END)*sizeof(double*)));
	assert( m != nil );
	m += NR_END;
	m -= nrl;

	/* allocate rows and set pointers to them */
	m[nrl]=(double *) malloc((size_t)((nrow*ncol+NR_END)*sizeof(double)));
	assert( m[nrl] != nil );
	m[nrl] += NR_END;
	m[nrl] -= ncl;

	for(i=nrl+1;i<=nrh;i++) m[i]=m[i-1]+ncol;

	/* return pointer to array of pointers to rows */
	return m;
}

double *dvector(int nl, int nh)
/* allocate a double vector with subscript range v[nl..nh] */
{
	double *v;

	v=(double *)malloc((size_t) ((nh-nl+1+NR_END)*sizeof(double)));
	assert( v != nil );
	return v-nl+NR_END;
}

void free_dvector(double *v, int nl, int nh)
/* free a double vector allocated with dvector() */
{
	free((FREE_ARG) (v+nl-NR_END));
}

void free_dmatrix(double **m, int nrl, int nrh, int ncl, int nch)
/* free a double matrix allocated by dmatrix() */
{
	free((FREE_ARG) (m[nrl]+ncl-NR_END));
	free((FREE_ARG) (m+nrl-NR_END));
}

double sinsin(double a, double b, double ki, double kj)
{
  double dk, sk;
  if ( ki == kj )
    return( (b-a)/2 - (s(2.0*ki*b)-s(2.0*ki*a))/(4*ki) );
  else {
    dk = ki - kj;
    sk = ki + kj;
    return( (s(dk*b) - s(dk*a))/(2*dk) - (s(sk*b) - s(sk*a))/(2*sk) );
  }
}

double sincos(double a, double b, double ki, double kj)
{
  double dk, sk;
  if ( ki == kj )
    return( (s(ki*b)*s(ki*b) - s(ki*a)*s(ki*a))/(2*ki) );
  else {
    dk = ki - kj;
    sk = ki + kj;
    return( (c(dk*a) - c(dk*b))/(2*dk) + (c(sk*a) - c(sk*b))/(2*sk) );
  }
}

double intsin( double a, double b, double k )
{
	return  ( cos( k * a ) - cos( k * b ) ) / k;
}
