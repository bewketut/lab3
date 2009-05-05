#include <assert.h>
#include <math.h>

#include "utils.h"
#include "QL.h"
#include "Tiling.h"
#include "FDM_Mode.h"
#include "FDM_Solver.h"

#define sqr(x)     ((x)*(x))
#define min(x,y)   (x) < (y) ? (x) : (y)
#define max(x,y)   (x) > (y) ? (x) : (y)
#define nil 0
#define pi  3.14159265358979323846


FDM_Solver::FDM_Solver(Tiling *t, int mxe, int mxo, int mye, int myo, double wl)
{
  this->t = new Tiling( t );
  this->mx = mxe + mxo;
  this->mxe = mxe;
  this->my = mye + myo;
  this->mye = mye;
  this->wl = wl;
  nmodes = 0;  
  mo = 0;
  
  nm = mx * my;
  m = dmatrix(1, nm, 1, nm);
  for ( int i = 1; i <= nm; i++ )
    for ( int j = 1; j <= nm; j++ )
      m[i][j] = 0.0;
      
  ne = dvector(1, nm);
  
  kx    = new double[nm]; assert( kx != nil );
  ky    = new double[nm]; assert( ky != nil );
  
  BuildSpatialFrequencies();
  PrepareTiling();
}

FDM_Solver::FDM_Solver( FDM_Solver *copy )
{
  t      = new Tiling( copy->t );
  mx     = copy->mx;
  mxe    = copy->mxe;  
  my     = copy->my;
  mye    = copy->mye;      
  wl     = copy->wl;
  nmodes = copy->nmodes;
  mo     = copy->mo;

  nm     = copy->nm;
  
  m = dmatrix(1, nm, 1, nm ); assert( m != nil );
  for ( int i = 1; i <= nm; i++ )
    for ( int j = 1; j <= nm; j++ )
      m[i][j] = copy->m[i][j];
      
  ne = dvector(1, nm);
  for ( int i = 1; i <= nm; i++ )
    ne[i] = copy->ne[i];
    
  kx = new double[nm]; assert( kx != nil );
  ky = new double[nm]; assert( ky != nil );
  for ( int i = 0; i < nm; i++ ) {
    kx[i] = copy->kx[i];
    ky[i] = copy->ky[i];
  }

}

FDM_Solver::~FDM_Solver( void )
{
  if ( m != nil ) free_dmatrix(m, 1, nm, 1, nm);
  if ( ne != nil ) free_dvector(ne, 1, nm);
  delete t;
  delete []kx; delete []ky;
}

int FDM_Solver::Solve( double nmin, double nmax )
{
  if ( nmin == 0.0 && nmax == 0.0 ) {
    nmin = nmax = sqrt(t->p[0].ind);
    for ( int i = 1; i < t->n; i++ ) {
      nmin = min( nmin, sqrt(t->p[i].ind + t->p[0].ind) );
      nmax = max( nmax, sqrt(t->p[i].ind + t->p[0].ind) );
    }
  } else if ( fabs(nmin - nmax) == 0 ) 
    return nmodes;

  double  *e = dvector(1, nm);
  
  
  double ox = t->p[0].xmin;
  double oy = t->p[0].ymin;
  t->Offset( -ox, -oy );

  BuildLinearSystem();

  tred2( m, nm, ne, e );    // Householder reduction
  tqli( ne, e, nm, m );     // eigenvalue calculation
  eigsrt(ne, m, nm);        // eigenvalue sort (descending)
  
  mo = -1;
  for ( int i = 1; i <= nm; i++ )
    if ( ne[i] > sqr(nmin) && ne[i] < sqr(nmax) ) {
      nmodes++;
      if ( mo == -1 ) mo = i;
    }
  
  t->Offset( ox, oy );

  free_dvector(e, 1, nm);
  
  return nmodes;
}

FDM_Mode *FDM_Solver::GetMode( int n )
{
  if ( nmodes == 0 ) return nil;
  if ( n >= nmodes ) return nil;
  n += mo;

  double *e = new double[nm]; assert( e != nil );

    for ( int i = 1; i <= nm; i++ ) e[i-1] = m[i][n];
      return new FDM_Mode( sqrt(ne[n]), wl, t->p[0], mxe, mx - mxe, mye, my - mye,  e );
    
  delete []e;
}

double FDM_Solver::EffectiveIndex( int n )
{
  return sqrt(ne[n + mo]);
}

void FDM_Solver::BuildLinearSystem( void )
{
  double Lx = (t->p[0].xmax - t->p[0].xmin);
  double Ly = (t->p[0].ymax - t->p[0].ymin);
  double kf = 4.0 / Lx / Ly;
  
  Tile *Re = t->p;

  // Loop over all elements of the upper-half of the system matrix 'm'
  for ( int i = 1; i <= nm ; i++ ) {
    for ( int j = 1 ; j <= i ; j++ ) {

      for ( int k = 0 ; k < t->n ; k++ ) {
      
        double ssx = sinsin(Re[k].xmin, Re[k].xmax, kx[i-1], kx[j-1]);
        double ssy = sinsin(Re[k].ymin, Re[k].ymax, ky[i-1], ky[j-1]);
        
        m[i][j] += kf * ssx * ssy * Re[k].ind;
        
      }
      
      // This matrix is symmetric...
      m[j][i] = m[i][j];
    
    }
  }

  kf = sqr(wl/2.0/pi);
  for ( int i = 1 ; i <= nm ; i++ )
    m[i][i]   -= kf * ( sqr(kx[i-1]) + sqr(ky[i-1]) );

}

void FDM_Solver::BuildSpatialFrequencies( void )
{
  double pix = pi / (t->p[0].xmax - t->p[0].xmin);
  double piy = pi / (t->p[0].ymax - t->p[0].ymin);
  for ( int i = 0 ; i < mx ; i++ )
    for ( int j = 0 ; j < my ; j++ ) {
      int k = my * i + j;
      kx[k] = pix * ( i < mxe ? 2*i+1 : 2*(i+1-mxe) );   
      ky[k] = piy * ( j < mye ? 2*j+1 : 2*(j+1-mye) );
  }
}

void FDM_Solver::PrepareTiling( void )
{
  t->p[0].ind = sqr(t->p[0].ind);
  for ( int i = 1 ; i < t->n ; i++ )
    t->p[i].ind = sqr(t->p[i].ind) - t->p[0].ind;
}