class   FDM_Solver {
    
  protected:
  
    Tiling *t;      // associated Tiling
    int    mx, mxe; // sine numbers in x
    int    my, mye; // sine numbers in y
    double wl;      // wavelength
    int    nmodes;  // # of modes found
    int    mo;      // mode offset

    int    nm;      // matrix order
    double **m;     // system matrix
    double *ne;     // effective indices
    double *kx;     // Spatial frequencies in x
    double *ky;     // Spatial frequencies in y

  public:
  
    FDM_Solver(Tiling *t, int mxe, int mxo, int mye, int myo, double wl);
    FDM_Solver(   FDM_Solver *copy );
    virtual ~FDM_Solver( void );
    
    int Solve( double nmin = 0.0, double nmax = 0.0 );
    FDM_Mode *GetMode( int n );
    double EffectiveIndex( int n );

  private:
    
    void BuildLinearSystem( void );
    void BuildSpatialFrequencies( void );
    void PrepareTiling( void );
};
