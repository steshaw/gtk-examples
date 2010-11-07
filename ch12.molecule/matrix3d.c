/*
 * File: matrix3d.c
 * Auth: Eric Harlow
 * 
 * Converted from a Java Matrix 3d class file.
 */

#include "atom.h"
#include "matrix3d.h"
#include <math.h>


static double pi = 3.14159265;

/*
 * NewMatrix3D
 *
 * Create a new matrix
 */
typMatrix3D *NewMatrix3D () 
{
    typMatrix3D *mat;

    mat = (typMatrix3D *) g_malloc (sizeof (typMatrix3D));

    unit (mat);
    
    return (mat);
}

/*
 * scale
 *
 * Scale the object
 */
void scale (typMatrix3D *mat, double f) 
{
    mat->xx *= f;
    mat->xy *= f;
    mat->xz *= f;
    mat->xo *= f;
    mat->yx *= f;
    mat->yy *= f;
    mat->yz *= f;
    mat->yo *= f;
    mat->zx *= f;
    mat->zy *= f;
    mat->zz *= f;
    mat->zo *= f;
}


/*
 * scale3
 *
 * Scale each direction by a different factor
 */
void scale3 (typMatrix3D *mat, double xf, double yf, double zf) 
{
    mat->xx *= xf;
    mat->xy *= xf;
    mat->xz *= xf;
    mat->xo *= xf;
    mat->yx *= yf;
    mat->yy *= yf;
    mat->yz *= yf;
    mat->yo *= yf;
    mat->zx *= zf;
    mat->zy *= zf;
    mat->zz *= zf;
    mat->zo *= zf;
}


/*
 * translate
 *
 * Translate the point represented by the matrix by (x, y, z)
 */
void translate (typMatrix3D *mat, double x, double y, double z) 
{
    mat->xo += x;
    mat->yo += y;
    mat->zo += z;
}

/*
 * yrot
 *
 * Add in a y rotation of angle (theta) to the matrix.
 */
void yrot (typMatrix3D *mat, double theta) 
{
    double ct;
    double st;
    double Nxx;
    double Nxy;
    double Nxz;
    double Nxo;
    double Nzx;
    double Nzy;
    double Nzz;
    double Nzo;

    theta *= (pi / 180);
    ct = cos (theta);
    st = sin (theta);

    Nxx = (double) (mat->xx * ct + mat->zx * st);
    Nxy = (double) (mat->xy * ct + mat->zy * st);
    Nxz = (double) (mat->xz * ct + mat->zz * st);
    Nxo = (double) (mat->xo * ct + mat->zo * st);

    Nzx = (double) (mat->zx * ct - mat->xx * st);
    Nzy = (double) (mat->zy * ct - mat->xy * st);
    Nzz = (double) (mat->zz * ct - mat->xz * st);
    Nzo = (double) (mat->zo * ct - mat->xo * st);

    mat->xo = Nxo;
    mat->xx = Nxx;
    mat->xy = Nxy;
    mat->xz = Nxz;
    mat->zo = Nzo;
    mat->zx = Nzx;
    mat->zy = Nzy;
    mat->zz = Nzz;
}



/*
 * xrot
 *
 * Add in a x rotation of angle (theta) to the matrix.
 */
void xrot (typMatrix3D *mat, double theta) 
{
    double ct;
    double st;
    double Nyx;
    double Nyy;
    double Nyz;
    double Nyo;
    double Nzx;
    double Nzy;
    double Nzz;
    double Nzo;

    theta *= (pi / 180);
    ct = cos (theta);
    st = sin (theta);

    Nyx = (double) (mat->yx * ct + mat->zx * st);
    Nyy = (double) (mat->yy * ct + mat->zy * st);
    Nyz = (double) (mat->yz * ct + mat->zz * st);
    Nyo = (double) (mat->yo * ct + mat->zo * st);

    Nzx = (double) (mat->zx * ct - mat->yx * st);
    Nzy = (double) (mat->zy * ct - mat->yy * st);
    Nzz = (double) (mat->zz * ct - mat->yz * st);
    Nzo = (double) (mat->zo * ct - mat->yo * st);

    mat->yo = Nyo;
    mat->yx = Nyx;
    mat->yy = Nyy;
    mat->yz = Nyz;
    mat->zo = Nzo;
    mat->zx = Nzx;
    mat->zy = Nzy;
    mat->zz = Nzz;
}


/*
 * zrot
 *
 * Add in a z rotation of angle (theta) to the matrix.
 */
void zrot (typMatrix3D *mat, double theta) 
{
    double ct;
    double st;
    double Nyx;
    double Nyy;
    double Nyz;
    double Nyo;
    double Nxx;
    double Nxy;
    double Nxz;
    double Nxo;
 
    theta *= (pi / 180);
    ct = cos(theta);
    st = sin(theta);

    Nyx = (double) (mat->yx * ct + mat->xx * st);
    Nyy = (double) (mat->yy * ct + mat->xy * st);
    Nyz = (double) (mat->yz * ct + mat->xz * st);
    Nyo = (double) (mat->yo * ct + mat->xo * st);

    Nxx = (double) (mat->xx * ct - mat->yx * st);
    Nxy = (double) (mat->xy * ct - mat->yy * st);
    Nxz = (double) (mat->xz * ct - mat->yz * st);
    Nxo = (double) (mat->xo * ct - mat->yo * st);

    mat->yo = Nyo;
    mat->yx = Nyx;
    mat->yy = Nyy;
    mat->yz = Nyz;
    mat->xo = Nxo;
    mat->xx = Nxx;
    mat->xy = Nxy;
    mat->xz = Nxz;
}

/*
 * mult
 *
 * Multiply the first matrix by the second matrix.
 * The first matrix has the new value.
 */
void mult (typMatrix3D *mat, typMatrix3D *rhs) 
{
    double lxx = mat->xx * rhs->xx + 
                 mat->yx * rhs->xy + 
                 mat->zx * rhs->xz;
    double lxy = mat->xy * rhs->xx + 
                 mat->yy * rhs->xy + 
                 mat->zy * rhs->xz;
    double lxz = mat->xz * rhs->xx + 
                 mat->yz * rhs->xy + 
                 mat->zz * rhs->xz;
    double lxo = mat->xo * rhs->xx + 
                 mat->yo * rhs->xy + 
                 mat->zo * rhs->xz + rhs->xo;

    double lyx = mat->xx * rhs->yx + 
                 mat->yx * rhs->yy + 
                 mat->zx * rhs->yz;
    double lyy = mat->xy * rhs->yx + 
                 mat->yy * rhs->yy + 
                 mat->zy * rhs->yz;
    double lyz = mat->xz * rhs->yx + 
                 mat->yz * rhs->yy + 
                 mat->zz * rhs->yz;
    double lyo = mat->xo * rhs->yx + 
                 mat->yo * rhs->yy + 
                 mat->zo * rhs->yz + rhs->yo;

    double lzx = mat->xx * rhs->zx + 
                 mat->yx * rhs->zy + 
                 mat->zx * rhs->zz;
    double lzy = mat->xy * rhs->zx + 
                 mat->yy * rhs->zy + 
                 mat->zy * rhs->zz;
    double lzz = mat->xz * rhs->zx + 
                 mat->yz * rhs->zy + 
                 mat->zz * rhs->zz;
    double lzo = mat->xo * rhs->zx + 
                 mat->yo * rhs->zy + 
                 mat->zo * rhs->zz + rhs->zo;

    mat->xx = lxx;
    mat->xy = lxy;
    mat->xz = lxz;
    mat->xo = lxo;

    mat->yx = lyx;
    mat->yy = lyy;
    mat->yz = lyz;
    mat->yo = lyo;

    mat->zx = lzx;
    mat->zy = lzy;
    mat->zz = lzz;
    mat->zo = lzo;
}


/*
 * unit
 *
 * Make the matrix be a unit matrix
 */
void unit (typMatrix3D *mat) 
{
    mat->xo = 0;
    mat->xx = 1;
    mat->xy = 0;
    mat->xz = 0;
    mat->yo = 0;
    mat->yx = 0;
    mat->yy = 1;
    mat->yz = 0;
    mat->zo = 0;
    mat->zx = 0;
    mat->zy = 0;
    mat->zz = 1;
}

/*
 * Transform 
 *
 * Translate the atom's coordinates into the tranlated 
 * coordinates.
 */
void Transform (typMatrix3D *mat, typAtom *atom)
{
    double lxx = mat->xx, lxy = mat->xy, lxz = mat->xz, lxo = mat->xo;
    double lyx = mat->yx, lyy = mat->yy, lyz = mat->yz, lyo = mat->yo;
    double lzx = mat->zx, lzy = mat->zy, lzz = mat->zz, lzo = mat->zo;

    double x = atom->x;
    double y = atom->y;
    double z = atom->z;

    atom->tx = (x * lxx + y * lxy + z * lxz + lxo);
    atom->ty = (x * lyx + y * lyy + z * lyz + lyo);
    atom->tz = (x * lzx + y * lzy + z * lzz + lzo);
}

