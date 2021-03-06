/****************************************************************
 Code by Leonardo Berti (C) 2007
 Portable Math Engine - quaternions

 ****************************************************************/

#ifndef QUAT_H_INCLUDED
#define QUAT_H_INCLUDED

//questo modulo dipende dal modulo base vectors
//real è un tipo di dati definito in vectors.h
#include "libmath.h"
#include "vectors.h"
#include "matrix.h"

#ifdef _USENAMESPACE_
namespace mathengine
{
#endif

/**QUAT3D - quaternion 3D*/
typedef struct QUAT3D_TYP
{
    real r; //parte reale
    real x,y,z; //parte immaginaria

} QUAT3D,*QUAT3D_PTR;

#ifdef __cplusplus
//quando si compila in c++ questa serve a fare in modo che non vengano 'decorati'
//i nomi dal linker
extern "C"
{
#endif

//quaternions
//stampa un quaternion
MATH_EXPORT void printQuat3D(FILE* out,const QUAT3D& q);
//crea un quaternion
MATH_EXPORT QUAT3D getQuat3D(const real r,const real x,const real y,const real z);
//Crea un quaternion da un vettore
MATH_EXPORT QUAT3D vectorToQuat3D(HVECTOR3D_PTR v);
MATH_EXPORT void vectorToQuat3DEx(QUAT3D_PTR q,const HVECTOR3D& v);

//converte un quaternion in un vettore
MATH_EXPORT HVECTOR3D quatToVector(const QUAT3D& q);
MATH_EXPORT void quatToVectorEx(HVECTOR3D_PTR v,const QUAT3D& q);

//q1+q2
MATH_EXPORT QUAT3D sumQuat3D(const QUAT3D& q1,const QUAT3D& q2);
MATH_EXPORT void sumQuat3DEx(QUAT3D_PTR result,const QUAT3D& q1,const QUAT3D& q2);
//q1-q2
MATH_EXPORT QUAT3D subQuat3D(const QUAT3D& q1,const QUAT3D& q2);
MATH_EXPORT void subQuat3DEx(QUAT3D_PTR result,const QUAT3D& q1,const QUAT3D& q2);
//q1*q2
MATH_EXPORT QUAT3D mulQuat3D(const QUAT3D& q1,const QUAT3D& q2);
MATH_EXPORT void mulQuat3DEx(QUAT3D_PTR result,const QUAT3D& q1,const QUAT3D& q2);
//coniugato
MATH_EXPORT QUAT3D quatConjugate3D(const QUAT3D& q);
MATH_EXPORT void quatConjugateEx(QUAT3D_PTR result,const QUAT3D& q);

//normalizzazione
MATH_EXPORT QUAT3D normalizeQuat3D(const QUAT3D& q);
MATH_EXPORT void normalizeQuat3DEx(QUAT3D_PTR q);

//modulo
MATH_EXPORT real magnitudeQuat3D(const QUAT3D& q);
MATH_EXPORT real magnitudeQuat3DEx(QUAT3D_PTR q);

//ottiene un quaternion che esegue la rotazione di theta radianti intorno a versAxis
MATH_EXPORT QUAT3D getRotQuat3D(const HVECTOR3D& versAxis,real theta);
MATH_EXPORT void getRotQuat3DEx(QUAT3D_PTR q,const HVECTOR3D& versAxis,real theta);

//ottiene la matrice di rotazione a partire da un quaternion di rotazione
MATH_EXPORT void quatToRotMatrixEx(HMATRIX33_PTR m,const QUAT3D& q);
MATH_EXPORT HMATRIX33 quatToRotMatrix(const QUAT3D& q);

//converte da matrice di rotazione a quternion
//NB:la matrice deve essere una matrice di rotazione!
MATH_EXPORT void rotMatToQuatEx(QUAT3D_PTR q,const HMATRIX33& m);
MATH_EXPORT QUAT3D rotMatToQuat(const HMATRIX33& m);

/**SLERP:(spherical interpolation)
restituisce un quaternion compreso fra il quaternion iniziale qa e quello finale qb
basato sul parametro t compreso fra 0 e 1.t=0 rende qa t=1 rende qb
*/
MATH_EXPORT void quatSlerp(QUAT3D_PTR r,const QUAT3D& qa,const QUAT3D& qb,real t);

//Esegue il triplo prodotto fra quaternion (q1*q2*q3)
//(questo prodotto è utilizzabile per la rotazione di un vettore intorno ad un altro
//se q2 è ottenuto dal vettore che si vuol ruotare e q1 e q3 sono i quaternion di rotazione e il suo coniugato
//il risultato rappresenta il vettore ruotato)
MATH_EXPORT void quatTripleProduct(QUAT3D_PTR result,const QUAT3D& q1,const QUAT3D& q2,const QUAT3D& q3);

#ifdef __cplusplus
}
#endif

#ifdef _USENAMESPACE_
};
#endif

#endif // QUAT_H_INCLUDED
