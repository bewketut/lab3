#include <assert.h>
#include <math.h>

#include "utils.h"
#include "Tiling.h"
#include "FDM_Mode.h"

#define s(x) sin(x)
#define c(x) cos(x)
#define nil 0
#define pi  3.14159265358979323846

FDM_Mode::FDM_Mode( FDM_Mode *copy )
{
  neff = copy->neff;
  wl = copy->wl;
  d = copy->d;
  mx = copy->mx;
  mxe = copy->mxe;
  my = copy->my;
  mye = copy->mye;
  c = new double[mx * my]; assert ( c != nil );
  for ( int i = 0; i < mx * my; i++ )
    c[i] = copy->c[i];
  kx = new double[mx * my]; assert ( kx != nil );
  for ( int i = 0; i < mx * my; i++ )
    kx[i] = copy->kx[i];
  ky = new double[mx * my ]; assert ( ky != nil );
  for ( int i = 0; i < mx * my; i++ )
    ky[i] = copy->ky[i];
}

FDM_Mode::FDM_Mode( double neff, double wl, Tile d, int mxe, int mxo, int mye, int myo,  double *c )
{
  this->neff = neff;
  this->wl = wl;
  this->d = d;
  mx = mxe + mxo;
  this->mxe = mxe;
  my = mye + myo;
  this->mye = mye;
  this->c = new double[mx * my]; assert ( c != nil );
  for ( int i = 0; i < mx * my; i++ )
    this->c[i] = c[i];
  kx = new double[mx * my]; assert ( kx != nil );
  ky = new double[mx * my]; assert ( ky != nil );

  double pix = pi / (d.xmax - d.xmin);
  double piy = pi / (d.ymax - d.ymin);
  for ( int i = 0 ; i < mx ; i++ )
    for ( int j = 0 ; j < my ; j++ ) {
      int k = my * i + j;
      kx[k] = pix * ( i < mxe ? 2*i+1 : 2*(i+1-mxe) );   
      ky[k] = piy * ( j < mye ? 2*j+1 : 2*(j+1-mye) );
  }
}

FDM_Mode::~FDM_Mode( void )
{
  delete []c;
  delete []kx;
  delete []ky;
}

void FDM_Mode::Offset( double dx, double dy )
{
  d.Offset( dx, dy );
}


double FDM_Mode::Val( double x, double y )
{
  double s = 0.0;
  if ( ! d.IsInTile( x, y ) ) return s;
  x -= d.xmin;
  y -= d.ymin;
  for ( int k = 0; k < mx * my; k++ )
    s += c[k] * sin( kx[k] * x ) * sin( ky[k] * y );
  return s;
}

void FDM_Mode::Norm( double s )
{
  double norm = 0.0;
  for ( int k = 0; k < mx * my; k++ )
    norm += c[k] * c[k];
  norm = sqrt(norm/s);
  for ( int k = 0; k < mx * my; k++ )
    c[k] /= norm;
}

double FDM_Mode::Power( Tile &q )
{
  double po = 0.0;
  Tile r;
  if ( ! Tile::Intersect(d, q, r)  ) return po;
  
  double ox = d.xmin;
  double oy = d.ymin;
  d.Offset( -ox, -oy );
  r.Offset( -ox, -oy );
  
  for ( int i = 0; i < mx * my; i++ )
    for ( int j = 0; j < mx * my; j++ )
      po += c[i] * c[j] * sinsin( r.xmin, r.xmax, kx[i], kx[j] ) * 
                          sinsin( r.ymin, r.ymax, ky[i], ky[j] );
  po *= 4.0 / d.xmax / d.ymax;
  
  d.Offset( ox, oy );
  return po;
}

double FDM_Mode::Power( void )
{
  double po = 0.0;

  double ox = d.xmin;
  double oy = d.ymin;  
  d.Offset( -ox, -oy );
  
  for ( int i = 0; i < mx * my; i++ )
    po += c[i] * c[i];
  
  d.Offset( ox, oy );
  return po;
}

double FDM_Mode::Overlap( FDM_Mode *m1, FDM_Mode *m2 )
{
  double po = 0.0;  // overlap
  Tile r;           // intersecting region
  
  // Check if the two domain intersect at all
  if ( ! Tile::Intersect(m1->d, m2->d, r)  ) return po;
  
  // Keep track of the origin of the first domain
  double ox = m1->d.xmin;
  double oy = m1->d.ymin;
  
  // Offset domains and intersecting region such that the origin
  // is at the corner of 'lhs' domain
  m1->d.Offset( -ox, -oy );
  m2->d.Offset( -ox, -oy );
  r.Offset( -ox, -oy );

  // Loop over all spatial frequencies
  for ( int i = 0; i < m1->mx * m1->my; i++ )
    for ( int j = 0; j < m2->mx * m2->my; j++ )
      po += m1->c[i] * m2->c[j] * (
          c( m2->kx[j] * r.xmin ) *
          sinsin( r.xmin, r.xmax, m1->kx[i], m2->kx[j] ) -
          s( m2->kx[j] * r.xmin ) *
          sincos( r.xmin, r.xmax, m1->kx[i], m2->kx[j] ) ) * (
          c( m2->ky[j] * r.ymin ) *
          sinsin( r.ymin, r.ymax, m1->ky[i], m2->ky[j] ) -
          s( m2->ky[j] * r.ymin ) *
          sincos( r.ymin, r.ymax, m1->ky[i], m2->ky[j] ) );
  
  // Restore the original domains for both 'lhs' and 'rhs'
  m1->d.Offset( ox, oy );
  m2->d.Offset( ox, oy );  

  
  return po * 4.0 / sqrt(m1->d.xmax - m1->d.xmin) / sqrt(m2->d.xmax - m2->d.xmin) /
                sqrt(m1->d.ymax - m1->d.ymin) / sqrt(m2->d.ymax - m2->d.ymin);

}


