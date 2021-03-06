/**************************************************
Implementa un vettore di REAL a dimensione fissa
code by L.Berti (c) 2009
***************************************************/

#ifndef NVECTOR_H_INCLUDED
#define NVECTOR_H_INCLUDED

#include "mathdefs.h"
#include "mexception.h"
#include "libmath.h"
#include <stddef.h>
#include <iostream>

#ifdef _USENAMESPACE_
namespace mathengine {
    #endif

    using namespace std;

    MATH_EXPORT class NVector
    {
        protected:

        REAL* values;
        size_t n_components; //numero di componenti
        size_t mem_size; //memoria allocata

        public:

        NVector(REAL* values,size_t size);

        NVector(size_t size);

        virtual ~NVector();

        /**Resetta la matrice e ne cambia le dimensioni*/
        void Reset(size_t size,bool set_to_zero);

        REAL get_value(size_t index) const;

        REAL& operator [] (size_t index);

        REAL set_value(size_t index,REAL value);

        void ZeroVector();

        void set_values(const REAL* val) throw (MathException);

        const REAL* get_values() const;

        NVector& operator = (const NVector& v) throw (MathException);

        void add(NVector* result,const NVector& v) const;

        NVector operator + (const NVector& v) const;

        void sub(NVector* result,const NVector& v) const;

        NVector operator - (const NVector& v) const;
        /**restituisce il numero di elementi*/
        size_t GetSize() const;

        REAL dot_product(const NVector& v) const;

        /**prodotto scalare (dot product)*/
        REAL operator * (const NVector& v) const;

        /**nega ogni componente*/
        void negate();

        /**normalizza (lo trasforma in versore)*/
        void normalize() throw (MathException);

        /**acquisisce il modulo*/
        REAL magnitude() const;

        /**moltiplica per uno scalare*/
        void mul_scalar(REAL a);

    };

    //operatori & utility
    ostream& operator<<(ostream& s,const NVector& v);

#ifdef _USENAMESPACE_
};
#endif

#endif // NVECTOR_H_INCLUDED
