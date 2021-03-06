#ifndef VECTORS2_H_INCLUDED
#define VECTORS2_H_INCLUDED

/**Vettori 2D */

#include "mathdefs.h"
#include <cmath>

#ifdef _USENAMESPACE_
namespace mathengine {
#endif

    /**Vettore 2D*/
    template<class N> class CVector2 {

private:

        void init(N x,N y) {
            this->x=x;
            this->y=y;
        }

public:

        N x,y;

        CVector2(N x,N y) {
            init(x,y);
        }
        CVector2() {
            init(0,0);
        }

        N magnitude() const {
            return sqrt(x*x+y*y);
        }

        /**setta il vettore opposto*/
        void negate() {
            x=-x;
            y=-y;
        }

        /**imposta res come vettore opposto a questo*/
        void get_neg_vector(CVector2<N>& res) const {
            res.x=-x;
            res.y=-y;

        }

        /**trasforma in versore*/
        void normalize() {
            N m=sqrt(x*x+y*y);
            x/=m;
            y/=m;
        }

        /**imposta res come vettore normalizzato di quello corrente*/
        void get_normalized_vector(CVector2<N>& res) const {
            N m=sqrt(x*x+y*y);
            res.x=x/m;
            res.y=y/m;

        }

        N dot_product(const CVector2& v) {
            return x*v.x+y*v.y;
        }

        /**somma un vettore*/
        void add(const CVector2& v) {
            x+=v.x;
            y+=v.y;
        }

        /**sottrae un vettore*/
        void sub(const CVector2& v) {
            x-=v.x;
            y-=v.y;
        }

        /**moltiplica per uno scalare*/
        void mul_scalar(N s) {
            x*=s;
            y*=s;
        }

        void set(N x,N y,N z) {
            this->x=x;
            this->y=y;
        }

        /**Ruota il vettore in senso antiorario*/
        void rotate(const float theta) {

            double ct=cos(theta);
            double st=sin(theta);

            N x1=x;
            N y1=y;

            x=x1*ct-y1*st;
            y=x1*st+y1*ct;

        }

        /**restituisce l'angolo in radianti formato fra il vettore e l'asse x*/
        N getTheta() const {
            return acos(x/sqrt(x*x+y*y));
        }

    };

    /**somma tra vettori result=v1+v2*/
    template<class N> void add_vectors(CVector2<N>* result,const CVector2<N>& v1,const CVector2<N>& v2) {
        result->x=v1.x+v2.x;
        result->y=v1.y+v2.y;

    }

    template<class N> CVector2<N> add_vectors(const CVector2<N>& v1,const CVector2<N>& v2) {

        CVector2<N> r;

        r.x=v1.x+v2.x;
        r.y=v1.y+v2.y;

        return r;
    }

    /**somma tra vettori result=v1+v2*/
    template<class N> CVector2<N> operator+(const CVector2<N>& v1,const CVector2<N>& v2) {
        CVector2<N> r;

        r.x=v1.x+v2.x;
        r.y=v1.y+v2.y;

        return r;
    }


    /**sottrazione fra vettori result=v1-v2*/
    template<class N> void sub_vectors(CVector2<N>* result,const CVector2<N>& v1,const CVector2<N>& v2) {
        result->x=v1.x-v2.x;
        result->y=v1.y-v2.y;

    }

    template<class N> CVector2<N> sub_vectors(const CVector2<N>& v1,const CVector2<N>& v2) {
        CVector2<N> r;

        r.x=v1.x-v2.x;
        r.y=v1.y-v2.y;


        return r;
    }

    template<class N> CVector2<N> operator-(const CVector2<N>& v1,const CVector2<N>& v2) {
        CVector2<N> r;

        r.x=v1.x-v2.x;
        r.y=v1.y-v2.y;


        return r;
    }


    /**restituisce il versore*/
    template<class N> CVector2<N> norm(const CVector2<N>& v)
    {
        N d=sqrt(v.x*v.x+v.y*v.y);
        CVector2<N> r;
        r.x=v.x/d;
        r.y=v.y/d;


        return r;
    }

    template<class N> CVector2<N> create_vector(N x,N y)
    {
        CVector2<N> v;
        v.x=x;
        v.y=y;
        return v;
    }


#ifdef _USENAMESPACE_
};
#endif

#endif // VECTORS2_H_INCLUDED



