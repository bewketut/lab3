#include <math.h>
#include <assert.h>
#include "Tiling.h"

#define max(a,b) 	( (a) < (b) ?  (b) : (a) )
#define min(a,b) 	( (a) > (b) ?  (b) : (a) )
#define nil 0

Tile::Tile( void )
{
  xmin = xmax = ymin = ymax = 0.0;
  ind = 1.0;
}

Tile::Tile( double xmin, double xmax, double ymin, double ymax, double ind )
{
  this->xmin = xmin;
  this->xmax = xmax;
  this->ymin = ymin;
  this->ymax = ymax;
  this->ind = ind;
}

double Tile::Area( void )
{
  return fabs( (xmax - xmin) * (ymax - ymin) );
}

void Tile::Offset( double dx, double dy )
{
  xmin += dx; xmax += dx;
  ymin += dy; ymax += dy;
}

bool Tile::IsInTile( double x, double y )
{
  return ( x >= xmin ) && ( x <= xmax ) && ( y >= ymin ) && ( y <= ymax );
}

bool Tile::Intersect( Tile &t1, Tile &t2, Tile &tr )
{
  tr.xmin = max( t1.xmin, t2.xmin );
  tr.xmax = min( t1.xmax, t2.xmax );
  if ( tr.xmin >= tr.xmax )
    return false;
  tr.ymin = max( t1.ymin, t2.ymin );
  tr.ymax = min( t1.ymax, t2.ymax );
  if ( tr.ymin >= tr.ymax )
    return false;
  return true;
}

Tiling::Tiling( int n )
{
  this->n = n;
  p = new Tile[n]; assert( p != nil );
}

Tiling::Tiling( Tiling *copy )
{
  n = copy->n;
  p = new Tile[n]; assert( p != nil );
  for ( int i = 0; i < n; i++ )
    p[i] = copy->p[i];
}

Tiling::~Tiling( void )
{
  delete []p;
}

void Tiling::SetTile( int n, Tile &t )
{
  p[n] = t;
}

void Tiling::Offset( double dx, double dy )
{
  for ( int i = 0; i < n; i++ )
    p[i].Offset( dx, dy );
}

