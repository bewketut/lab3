#include <iostream>

#include "Tiling.h"
#include "FDM_Mode.h"
#include "FDM_Solver.h"
#include "FDM_Utils.h"
#define PI  (3.1415926)
#define LENGTH  (1.5)
using namespace std;			// syntactic short-cut

int main( void ) {


	double wl;					// Wavelength
	double neff;				// effective index
	int nxe, nxo;				// x-Fourier components
	int nye, nyo;				// y-Fourier components

	Tiling *guide;				// Guiding structures
	FDM_Solver *solver;			// Solver implementing the FDM
	
	cout.precision(10);
		
	guide = Tiling_Read( "guide1.dat" );

	nxe = nye = 12;
	nyo = nxo = 0;
	wl = 1.4;
        printf("PI:=%f\n",PI)	;
	solver = new FDM_Solver(guide, nxe, nxo, nye, nyo, wl);
	solver->Solve();
	neff = solver->EffectiveIndex(0);
        
	cout << "wavelength: " << wl   << endl;
	cout << "eff. index: " << neff << endl;

	delete solver;
		
    return 0;

}


static unsigned double accumlated_phase(double wl, double length, double neff){
          return (2*PI/wl) * neff * length;
}


