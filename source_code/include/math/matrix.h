/****************************************************************
 Code by Leonardo Berti (C) 2007
 Portable Math Engine - matrici 2x2 3x3 e 4x4 (coordinate omogenee)
 definizioni e operazioni principali

 ****************************************************************/

#ifndef MATRIX33_H_INCLUDED
#define MATRIX33_H_INCLUDED

//questo modulo dipende dal modulo base vectors
//real è un tipo di dati definito in vectors.h
#include "libmath.h"
#include "vectors.h"

#ifdef _USENAMESPACE_
namespace mathengine
{
#endif

#ifdef __cplusplus
//quando si compila in c++ questa serve a fare in modo che non vengano 'decorati'
//i nomi dal linker
extern "C"
{
#endif

/**MATRIX22 - matrice 2x2*/
typedef struct MATRIX22_TYP
{
    real a00,a01;
    real a10,a11;

} MATRIX22,*MATRIX22_PTR;

/**MATRIX33 -  matrice 3x3*/
typedef struct MATRIX33_TYP
{
    real a00,a01,a02;
    real a10,a11,a12;
    real a20,a21,a22;

} MATRIX33,*MATRIX33_PTR;

/**MATRIX33 - matrice 4x4 (coordinate omogenee)*/
typedef struct HMATRIX33_TYP
{
    real a00,a01,a02,a03;
    real a10,a11,a12,a13;
    real a20,a21,a22,a23;
    real a30,a31,a32,a33;

} HMATRIX33,*HMATRIX33_PTR;

/**restituisce la matrice identità*/
MATH_EXPORT void getIdentityMatrix22Ex(MATRIX22_PTR m);
MATH_EXPORT void getIdentityMatrix33Ex(MATRIX33_PTR m);
MATH_EXPORT void getIdentityHMatrix33Ex(HMATRIX33_PTR m);

MATH_EXPORT MATRIX22 getIdentityMatrix22(void);
MATH_EXPORT MATRIX33 getIdentityMatrix33(void);
MATH_EXPORT HMATRIX33 getIdentityHMatrix33(void);

/**somma fra matrici*/
MATH_EXPORT void addMatrix33(MATRIX33_PTR result,const MATRIX33& m1,const MATRIX33&m2);
/**aggiunge alla matrice dest la matrice m*/
MATH_EXPORT void addMatrix33Ex(MATRIX33_PTR dest,const MATRIX33& m);
/**somma fra matrici in coordinate omogenee*/
MATH_EXPORT void addHMatrix33Ex(HMATRIX33_PTR result,const HMATRIX33& m1,const HMATRIX33&m2);
/**aggiunge alla matrice dest la matrice m*/
MATH_EXPORT void addHMatrix33(HMATRIX33_PTR dest,const HMATRIX33& m);

/**sottrazione fra matrici*/
MATH_EXPORT void subMatrix33(MATRIX33_PTR result,const MATRIX33& m1,const MATRIX33&m2);
/**aggiunge alla matrice dest la matrice m*/
MATH_EXPORT void subMatrix33Ex(MATRIX33_PTR dest,const MATRIX33& m);
/**somma fra matrici in coordinate omogenee*/
MATH_EXPORT void subHMatrix33(HMATRIX33_PTR result,const HMATRIX33& m1,const HMATRIX33&m2);
/**aggiunge alla matrice dest la matrice m*/
MATH_EXPORT void subHMatrix33Ex(HMATRIX33_PTR dest,const HMATRIX33& m);

/**calcola il determinante della matrice*/
MATH_EXPORT real det(const MATRIX33& m);

/**Calcola l'inversa.Se non è invertibile (det(m)=0) rende false altrimenti rende true*/
MATH_EXPORT bool inverse(MATRIX33_PTR r,const MATRIX33& m);

/**moltiplicazione di uno scalare per una matrice a*M*/
MATH_EXPORT void mulScalMatrix33(MATRIX33_PTR result,real a,const MATRIX33& m);
MATH_EXPORT void mulScalMatrix33Ex(MATRIX33_PTR dest,real a);

MATH_EXPORT void mulScalHMatrix33(HMATRIX33_PTR result,real a,const HMATRIX33& m);
MATH_EXPORT void mulScalHMatrix33Ex(HMATRIX33_PTR dest,real a);

/**moltiplicazioni fra matrici (m1 x m2)*/
MATH_EXPORT void mulMatrix33(MATRIX33_PTR result,const MATRIX33& m1,const MATRIX33& m2);
MATH_EXPORT void mulHMatrix33(HMATRIX33_PTR result,const HMATRIX33& m1,const HMATRIX33& m2);

MATH_EXPORT void transposeMatrixEx(HMATRIX33_PTR result,const HMATRIX33& m);
MATH_EXPORT HMATRIX33 transposeMatrix(const HMATRIX33& m);

/**prodotto di una matrice per un vettore m x v (restituisce un vettore)*/
MATH_EXPORT void mulMatrixVect2(VECTOR2D_PTR result,const MATRIX22& m,const VECTOR2D& v);
MATH_EXPORT void mulMatrixVect3(VECTOR3D_PTR result,const MATRIX33& m,const VECTOR3D& v);
MATH_EXPORT void mulMatrixHVect3Ex(HVECTOR3D_PTR result,const HMATRIX33& m,const HVECTOR3D& v);
MATH_EXPORT HVECTOR3D mulMatrixHVect3(const HMATRIX33& m,const HVECTOR3D& v);

/**trsforma una matrice omogenee in una semplice 3x3*/
MATH_EXPORT void toMatrix33(MATRIX33_PTR result,const HMATRIX33& m);
/**trasforma una 3x3 in una matrice in coordinate omogenee*/
MATH_EXPORT void toHMatrix33(HMATRIX33_PTR result,const MATRIX33& m);

#ifdef __cplusplus
}
#endif

#ifdef _USENAMESPACE_
};
#endif

#endif // MATRIX33_H_INCLUDED
