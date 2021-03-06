/****************************************************************
 Code by Leonardo Berti (C) 2006
 Portable Math Engine - vettori e matrici

 ****************************************************************/


#include <stdio.h>
#include <math.h>
#include "libmath.h"
#include "mathdefs.h"

#ifndef _VECTORS_INCLUDE_
#define _VECTORS_INCLUDE_


// ---- engine switches ------------------------------------------------
// modificare queste definizioni per adattare l'engine al compilatore

//indica di utilizzare i namespace (per default � definito)
//#define _USENAMESPACE_

//indica di definire utilizzare il tipo double per rappresentare i numeri
//reali (per default non � definito e usa i float)
//#define _USEDOUBLE_

#define _USEDEFAULT_

//----------------------------------------------------------------------

//impostazioni di default
#ifdef _USEDEFAULT_
#undef _USEDOUBLE_
#define _USENAMESPACE_
#endif

#ifdef _USENAMESPACE_

namespace mathengine {

#endif

#ifdef __cplusplus
    //quando si compila in c++ questa serve a fare in modo che non vengano 'decorati'
    //i nomi dal linker
    extern "C" {
#endif


#ifdef _USEDOUBLE_

        //utilizza il tipo double per rappresentare i numeri reali
        typedef double real;
        //definisce le funzioni matemetiche da usare con il tipo double
        #define sinr(x) sin(x)
        #define cosr(x) cos(x)
        #define sqrtr(x) sqrt(x)
        #define acosr(x) acos(x)

#else

        //utilizza il tipo float per rappresentare i numeri reali (default perch� +veloce)
        typedef float real;
        //usa la versione float per le seguenti funzioni matematiche
#define sinr(x) sinf(x)
#define cosr(x) cosf(x)
#define sqrtr(x) sqrt(x)
#define acosr(x) acosf(x)

#endif

        /**vettore o punto bidimensionale*/
        typedef struct VECTOR2D_TYP {

            VECTOR2D_TYP() {

            }

            VECTOR2D_TYP(real x,real y) {
                this->x=x;
                this->y=y;
            }

            real x, y;
        } VECTOR2D, *VECTOR2D_PTR, POINT2D, *POINT2D_PTR;

        /**vettore o punto tridimensionale*/
        typedef struct VECTOR3D_TYP {

            VECTOR3D_TYP() {
            }

            VECTOR3D_TYP(real x, real y, real z) {
                this->x = x;
                this->y = y;
                this->z = z;
            }

            real x, y, z;

        } VECTOR3D, *VECTOR3D_PTR, POINT3D, *POINT3D_PTR;

        /**Vettore tridimensionale in coordinate omogenee*/
        typedef struct HVECTOR3D_TYP {
            real x, y, z, w;

        } HVECTOR3D, *HVECTOR3D_PTR, HPOINT3D, *HPOINT3D_PTR;

        /**Costruzione di vettori*/
        MATH_EXPORT VECTOR2D getVector2D(real x, real y);
        MATH_EXPORT VECTOR3D getVector3D(real x, real y, real z);
        MATH_EXPORT HVECTOR3D getHVector3D(real x, real y, real z);

        /**
         *Azzera il vettore
         */
        void zeroVector2(VECTOR2D &vec2);
        void zeroVector3(VECTOR3D &vec3);
        void zeroVector3H(HVECTOR3D &vec3);
        
        //nota: nell'esportazione si devono dichiarare le funzioni C like quindi
        //l'overloading non � supportato
        MATH_EXPORT void getVector2DEx(VECTOR2D_PTR result, real x, real y);
        MATH_EXPORT void getVector3DEx(VECTOR3D_PTR result, real x, real y, real z);
        MATH_EXPORT void getHVector3DEx(HVECTOR3D_PTR result, real x, real y, real z);

        /**Output su file dei vettori (ese. stdout)*/
        MATH_EXPORT void printVector2D(FILE* out, const VECTOR2D& v);
        MATH_EXPORT void printVector3D(FILE* out, const VECTOR3D& v);
        MATH_EXPORT void printVectorH3D(FILE* out, const HVECTOR3D& v);

        /**Identit�*/
        MATH_EXPORT void getUnitVector2D(VECTOR2D_PTR result);
        MATH_EXPORT void getUnitVector3D(VECTOR3D_PTR result);
        MATH_EXPORT void getUnitHVector3D(HVECTOR3D_PTR result);

        /**ottiene il vettore opposto*/
        MATH_EXPORT VECTOR2D getNegVector2D(const VECTOR2D& v);
        MATH_EXPORT VECTOR3D getNegVector3D(const VECTOR3D& v);
        MATH_EXPORT HVECTOR3D getNegHVector3D(const HVECTOR3D& v);

        MATH_EXPORT void negVector2D(VECTOR2D_PTR v);
        MATH_EXPORT void negVector3D(VECTOR3D_PTR v);
        MATH_EXPORT void negHVector3D(HVECTOR3D_PTR v);

        /**aggiunge a dest il vettore v dest+=v */
        MATH_EXPORT void addVector2D(VECTOR2D_PTR dest, const VECTOR2D& v);
        MATH_EXPORT void addVector3D(VECTOR3D_PTR dest, const VECTOR3D& v);
        MATH_EXPORT void addHVector3D(HVECTOR3D_PTR dest, const HVECTOR3D& v);

        /**somma di vettori result=v1+v2*/
        MATH_EXPORT void sumVector2DEx(VECTOR2D_PTR result, const VECTOR2D& v1, const VECTOR2D& v2);
        MATH_EXPORT void sumVector3DEx(VECTOR3D_PTR result, const VECTOR3D& v1, const VECTOR3D& v2);
        MATH_EXPORT void sumHVector3DEx(HVECTOR3D_PTR result, const HVECTOR3D& v1, const HVECTOR3D& v2);

        /**somma di vettori, restituisce v1+v2*/
        MATH_EXPORT VECTOR2D sumVector2D(const VECTOR2D& v1, const VECTOR2D& v2);
        MATH_EXPORT VECTOR3D sumVector3D(const VECTOR3D& v1, const VECTOR3D& v2);
        MATH_EXPORT HVECTOR3D sumHVector3D(const HVECTOR3D& v1, const HVECTOR3D& v2);

        /**differenza fra vettori result=v1-v2*/
        MATH_EXPORT void subVector2DEx(VECTOR2D_PTR result, const VECTOR2D& v1, const VECTOR2D& v2);
        MATH_EXPORT void subVector3DEx(VECTOR3D_PTR result, const VECTOR3D& v1, const VECTOR3D& v2);
        MATH_EXPORT void subHVector3DEx(HVECTOR3D_PTR result, const HVECTOR3D& v1, const HVECTOR3D& v2);

        /**differenza fra vettori restituisce v1-v2*/
        MATH_EXPORT VECTOR2D subVector2D(const VECTOR2D& v1, const VECTOR2D& v2);
        MATH_EXPORT VECTOR3D subVector3D(const VECTOR3D& v1, const VECTOR3D& v2);
        MATH_EXPORT HVECTOR3D subHVector3D(const HVECTOR3D& v1, const HVECTOR3D& v2);

        /** prodotto misto (fra uno scalare e un vettore)*/
        MATH_EXPORT void mulHVector3DEx(HVECTOR3D_PTR result, const HVECTOR3D&v, const real scalar);
        MATH_EXPORT HVECTOR3D mulHVector3D(const HVECTOR3D&v, const real scalar);

        /**prodotto scalare*/
        MATH_EXPORT real dotProduct2(const VECTOR2D& v1, const VECTOR2D& v2);
        MATH_EXPORT real dotProduct3(const VECTOR3D& v1, const VECTOR3D& v2);
        MATH_EXPORT real dotProduct3H(const HVECTOR3D& v1, const HVECTOR3D& v2);

        /**prodotto vettoriale v1 X v2*/
        MATH_EXPORT VECTOR3D crossProduct3(const VECTOR3D& v1, const VECTOR3D& v2);
        MATH_EXPORT HVECTOR3D crossProduct3H(const HVECTOR3D& v1, const HVECTOR3D& v2);

        MATH_EXPORT void crossProduct3Ex(VECTOR3D_PTR result, const VECTOR3D& v1, const VECTOR3D& v2);
        MATH_EXPORT void crossProduct3HEx(HVECTOR3D_PTR result, const HVECTOR3D& v1, const HVECTOR3D& v2);

        /**ottiene il versore del prodotto vettoriale tra due vettori*/
        MATH_EXPORT VECTOR3D normCrossProduct3(const VECTOR3D& v1, const VECTOR3D& v2);
        MATH_EXPORT HVECTOR3D normCrossProduct3H(const HVECTOR3D& v1, const HVECTOR3D& v2);

        MATH_EXPORT void normCrossProduct3HEx(HVECTOR3D_PTR result, const HVECTOR3D& v1, const HVECTOR3D& v2);
        MATH_EXPORT void normCrossProduct3Ex(VECTOR3D_PTR result, const VECTOR3D& v1, const VECTOR3D& v2);

        /**normalizza (ottiene un versore a partire dal vettore)*/
        MATH_EXPORT VECTOR2D normalizeVector2D(const VECTOR2D& v);
        MATH_EXPORT VECTOR3D normalizeVector3D(const VECTOR3D& v);

        MATH_EXPORT void normalizeVector2DEx(VECTOR2D_PTR v);
        MATH_EXPORT void normalizeVector3DEx(VECTOR3D_PTR v);

        MATH_EXPORT HVECTOR3D normalizeHVector3D(const HVECTOR3D& v);
        MATH_EXPORT void normalizeHVector3DEx(HVECTOR3D_PTR v);

        /**Calcola il modulo*/
        MATH_EXPORT real magnitude2(const VECTOR2D& v);
        MATH_EXPORT real magniture2Ex(VECTOR2D_PTR v);
        MATH_EXPORT real magnitude(const VECTOR3D& v);
        MATH_EXPORT real magnitudeEx(VECTOR3D_PTR v);
        MATH_EXPORT real magnitudeh(const HVECTOR3D& v);
        MATH_EXPORT real magnitudehEx(HVECTOR3D_PTR v);

#ifdef __cplusplus
        //quando si compila in c++ questa serve a fare in modo che non vengano 'decorati'
        //i nomi dal linker
    }
#endif


#ifdef _USENAMESPACE_

};

#endif

#endif
