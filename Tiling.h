class Tile {

  public:
  
    double xmin, xmax, ymin, ymax;  // extremas
    double ind;                     // refractive index
  
    Tile( void );
    Tile( double xmin, double xmax, double ymin, double ymax, double ind );
  
    double Area( void );
    void Offset( double dx, double dy );
    bool IsInTile( double x, double y );

    static bool Intersect( Tile &t1, Tile &t2, Tile &tr );
    
};

class Tiling {

  public:
  
    int  n;    // number of tiles
    Tile  *p;  // Array/pointer of tiles
  
    Tiling( int n );
    Tiling( Tiling *copy );
    virtual ~Tiling( void );
  
    void SetTile( int n, Tile &t );
    void Offset( double dx, double dy );
};

