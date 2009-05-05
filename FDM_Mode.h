class FDM_Mode {

  protected:
  
    double  neff;      // Effective index
    double  wl;        // Wavelength
    Tile    d;         // Domain of definition
    int    mx, mxe;    // decompostion order and even parity number
    int    my, mye;    // decompostion order and even parity number
    double  *c;        // Fourier coefficient
    double  *kx, *ky;  // Fourier frequencies
  
  public:
  
    FDM_Mode( FDM_Mode *copy );
    FDM_Mode( double neff, double wl, Tile d, int mxe, int mxo, int mye, int myo,  double *c );
    virtual ~FDM_Mode( void );
    
    void Offset( double dx, double dy );
    double Val( double x, double y );
    void Norm( double s = 1.0 );
    double Power( Tile &q );
    double Power( void );

    static double Overlap( FDM_Mode *m1, FDM_Mode *m2 ); 
};