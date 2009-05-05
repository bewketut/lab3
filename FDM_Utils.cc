#include <assert.h>
#include <iostream>
#include <fstream>

#include "Tiling.h"
#include "FDM_Mode.h"
#include "FDM_Utils.h"

using namespace std;

Tiling *Tiling_Read( const char *filename )
{

	int nb;				// number of tiles
	Tile t;				// tile variable for i/o
	Tiling *guide = 0;	// tiling structure

	ifstream in(filename, ifstream::in);
	assert( in.good() );
	
	in >> nb; assert( nb > 0 );
	
	guide = new Tiling(nb);

	for( int i = 0; i < nb; i++ ) {		// input all tiles
		in >> t.xmin;
		in >> t.xmax;
		in >> t.ymin;
		in >> t.ymax;
		in >> t.ind;
		guide->SetTile(i, t);
	}

	in.close();
	return guide;
}

void FDM_Mode_Write( FDM_Mode *mode, Tiling *guide, const char *filename, int nx, int ny )
{
	Tile domain = guide->p[0];
	double dx = (domain.xmax - domain.xmin) / (nx - 1);
	double dy = (domain.ymax - domain.ymin) / (ny - 1);
	
	ofstream out(filename, ofstream::out);
	
	double y = domain.ymin;
	for( int i = 0; i < ny; i++ ) {
		double x = domain.xmin;
		for ( int j = 0; j < nx; j++ ) {
			out << mode->Val(x,y) << " ";
			x += dx;
		}
		out << "\n";
		y += dy;
	}
	
	out.close();
}




