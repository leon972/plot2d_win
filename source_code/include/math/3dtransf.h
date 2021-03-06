/****************************************************************
 Code by Leonardo Berti (C) 2007
 Math engine - trasformazioni 3D , rotazioni,proiezioni ecc...

 ****************************************************************/

#ifndef TRANSF_H_INCLUDED
#define TRANSF_H_INCLUDED

#include "libmath.h"
#include "vectors.h"  //modulo base per i vettori
#include "matrix.h"   //matrici
#include "quat.h"     //quaternions

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

//Esegue la rotazione del vettore v intorno al versore vers
MATH_EXPORT HVECTOR3D rotateVector(const HVECTOR3D& v,const HVECTOR3D& vers,real theta);

//restituisce l'angolo in radianti della longitudine cioè l'angolo formato dal piano che contiene il vettore
//ed è perpendicolare al piano XY e il piano XZ
MATH_EXPORT real getLongitudeZ(const HVECTOR3D& vers);

//restituisce l'angolo in radianti fra il vettore vers e il piano XY
MATH_EXPORT real getAzimutZ(const HVECTOR3D& vers);

//acquisisce la matrice di rotazione intorno all'asse Z,Y e X
MATH_EXPORT HMATRIX33 getRotZMatrix(real theta);
MATH_EXPORT HMATRIX33 getRotYMatrix(real theta);
MATH_EXPORT HMATRIX33 getRotXMatrix(real theta);

//Ottiene la matrice che esegue la rotazione di theta radianti intorno al versore vers
//(fomula di Rodrigues)
MATH_EXPORT HMATRIX33 getRotMatrix(const HVECTOR3D& vers,real theta);
MATH_EXPORT void getRotMatrixEx(HMATRIX33_PTR r,const HVECTOR3D& vers,real theta);
//Ottiene la matrice euleriana di trasformazione da sistema fisso a mobile (assi corpo)
//le rotazioni vengono eseguite nella sequenza:
//psi=rotazione intorno a Z,rot. theta intorno alla linea dei nodi e phi intorno al nuovo asse Z (Z')
MATH_EXPORT HMATRIX33 getEulerMatrix(const real psi,const real theta,const real phi);
MATH_EXPORT void getEulerMatrixEx(HMATRIX33* m,const real psi,const real theta,const real phi);
//Ottiene la matrice di traslazione
HMATRIX33 getTranslationMatrix(const real tx,const real ty,const real tz);
MATH_EXPORT void getTranslationMatrixEx(HMATRIX33* m,const real tx,const real ty,const real tz);
//Ottiene la matrice di scala
MATH_EXPORT HMATRIX33 getScaleMatrix(const real sx,const real sy,const real sz);
MATH_EXPORT void getScaleMatrixEx(HMATRIX33* m,const real sx,const real sy,const real sz);

#ifdef __cplusplus
//quando si compila in c++ questa serve a fare in modo che non vengano 'decorati'
//i nomi dal linker
}
#endif

#ifdef _USENAMESPACE_
};
#endif

#endif // 3DTRANSF_H_INCLUDED
