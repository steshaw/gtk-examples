typedef struct {
    double xx, xy, xz, xo;
    double yx, yy, yz, yo;
    double zx, zy, zz, zo;
} typMatrix3D;


typMatrix3D *NewMatrix3D ();
void scale (typMatrix3D *mat, double f);
void scale3 (typMatrix3D *mat, double xf, double yf, double zf);
void translate (typMatrix3D *mat, double x, double y, double z);
void yrot (typMatrix3D *mat, double theta);
void xrot (typMatrix3D *mat, double theta); 
void zrot (typMatrix3D *mat, double theta);
void mult (typMatrix3D *mat, typMatrix3D *rhs);
void unit (typMatrix3D *mat);
void transform (typMatrix3D *mat, double *v, int *tv, int nvert);
void Transform (typMatrix3D *mat, typAtom *atom);
