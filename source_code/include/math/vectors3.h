/**Vettori 3D in coordinate omogenee
 Coded by Leonardo Berti
 */

#ifndef VECTORS3_H_INCLUDED
#define VECTORS3_H_INCLUDED

#include "mathdefs.h"
#include <cmath>

#ifdef _USENAMESPACE_
namespace mathengine {
#endif

    /**Vettore 3d in coordinate omogenee*/
    template<class N> class CHVector3 {

private:

        void init(N x,N y,N z) {
            this->x=x;
            this->y=y;
            this->z=z;
            this->w=1;
        }

public:

        N x,y,z,w;

        CHVector3(N x,N y,N z) {
            init(x,y,z); //coord. omogenee
        }
        CHVector3() {
            init(0,0,0);
        }

        N magnitude() const {
            return sqrt(x*x+y*y+z*z);
        }

        /**setta il vettore opposto*/
        void negate() {
            x=-x;
            y=-y;
            z=-z;
        }

        /**imposta res come vettore opposto a questo*/
        void get_neg_vector(CHVector3<N>& res) const {
            res.x=-x;
            res.y=-y;
            res.z=-z;
        }

        /**trasforma in versore*/
        void normalize() {
            N m=sqrt(x*x+y*y+z*z);
            x/=m;
            y/=m;
            z/=m;
            w=1;
        }

        /**imposta res come vettore normalizzato di quello corrente*/
        void get_normalized_vector(CHVector3<N>& res) const {
            N m=sqrt(x*x+y*y+z*z);
            res.x=x/m;
            res.y=y/m;
            res.z=z/m;
        }

        N dot_product(const CHVector3& v) {
            return x*v.x+y*v.y+z*v.z;
        }

        /**trasforma il vettore nel prodotto vettoriale v1 x v2
        NB: nessuno dei due vettori v1 e v2 deve conincidere con questo vettore
        */
        void cross_product(const CHVector3& v1,const CHVector3& v2) {
            x=(v1.y*v2.z-v1.z*v2.y);
            y=(v1.z*v2.x-v1.x*v2.z);
            z=(v1.x*v2.y-v1.y*v2.x);
            w=1;
        }

        /**trasforma questo vettore nel prodotto vettoriale fra se stesso e v*/
        void cross_product(const CHVector3& v) {
            N x1,y1,z1;

            x1=x;
            y1=y;
            z1=z;

            x=(y1*v.z-z1*v.y);
            y=(z1*v.x-x1*v.z);
            z=(x1*v.y-y1*v.x);
            w=1;
        }

        /**somma un vettore*/
        void add(const CHVector3& v) {
            x+=v.x;
            y+=v.y;
            z+=v.z;
        }

        /**sottrae un vettore*/
        void sub(const CHVector3& v) {
            x-=v.x;
            y-=v.y;
            z-=v.z;
        }

        /**moltiplica per uno scalare*/
        void mul_scalar(N s) {
            x*=s;
            y*=s;
            z*=s;
        }

        void set(N x,N y,N z) {
            this->x=x;
            this->y=y;
            this->z=z;
            w=1;
        }

        /**Esegue la rotazione del vettore intorno al versore vers di theta radianti
        (usa la formula di Rodrigues)*/

        void rotate(const CHVector3& vers,const float theta) {

            double ct=cos(theta);
            double st=sin(theta);

            //x= prodotto vettoriale
            //v*cos(theta)+vers x v*sin(theta)+vers.v*(1-cos(theta)) vers
            CHVector3<N> v1,v2,v3;

            v1=*this;
            v2=*this;

            v1.mul_scalar(ct);
            v2.mul_scalar(st);
            v3=vers;
            v3.cross_product(v2);

            add(v1);
            add(v3);

            v3=vers;
            v3.mul_scalar(this->dot_product(vers)*(1-ct));
            add(v3);

            w=1;
        }

        /**restituisce l'angolo in radianti della longitudine cio� l'angolo formato dal piano che contiene il versore
            ed � perpendicolare al piano XY e il piano XZ*/
        N getLongitudeZ() {
            return acos(x/sqrt(x*x+y*y));
        }

        /**restituisce l'angolo in radianti fra il versore e il piano XY*/
        N getAzimutZ() {
            N len=x*x+y*y+z*z;
            return acos(sqrt((x*x+y*y)/len));
        }

    };

    /**somma tra vettori result=v1+v2*/
    template<class N> void add_vectors(CHVector3<N>* result,const CHVector3<N>& v1,const CHVector3<N>& v2) {
        result->x=v1.x+v2.x;
        result->y=v1.y+v2.y;
        result->z=v1.z+v2.z;
        result->w=1;
    }

    template<class N> CHVector3<N> add_vectors(const CHVector3<N>& v1,const CHVector3<N>& v2) {

        CHVector3<N> r;

        r.x=v1.x+v2.x;
        r.y=v1.y+v2.y;
        r.z=v1.z+v2.z;
        r.w=1;

        return r;
    }

    /**somma tra vettori result=v1+v2*/
    template<class N> CHVector3<N> operator+(const CHVector3<N>& v1,const CHVector3<N>& v2) {

        CHVector3<N> r;

        r.x=v1.x+v2.x;
        r.y=v1.y+v2.y;
        r.z=v1.z+v2.z;
        r.w=1;

        return r;
    }


    /**sottrazione fra vettori result=v1-v2*/
    template<class N> void sub_vectors(CHVector3<N>* result,const CHVector3<N>& v1,const CHVector3<N>& v2) {
        result->x=v1.x-v2.x;
        result->y=v1.y-v2.y;
        result->z=v1.z-v2.z;
        result->w=1;
    }

    template<class N> CHVector3<N> sub_vectors(const CHVector3<N>& v1,const CHVector3<N>& v2) {

        CHVector3<N> r;

        r.x=v1.x-v2.x;
        r.y=v1.y-v2.y;
        r.z=v1.z-v2.z;
        r.w=1;

        return r;
    }

    template<class N> CHVector3<N> operator-(const CHVector3<N>& v1,const CHVector3<N>& v2) {

        CHVector3<N> r;

        r.x=v1.x-v2.x;
        r.y=v1.y-v2.y;
        r.z=v1.z-v2.z;
        r.w=1;

        return r;
    }

    /**esegue il prodotto vettoriale v1 x v2*/
    template<class N> void cross_product(CHVector3<N>* result,const CHVector3<N>& v1,const CHVector3<N>& v2) {
        result->x=(v1.y*v2.z-v1.z*v2.y);
        result->y=(v1.z*v2.x-v1.x*v2.z);
        result->z=(v1.x*v2.y-v1.y*v2.x);
        result->w=1;
    }

    template<class N> CHVector3<N> cross_product(const CHVector3<N>& v1,const CHVector3<N>& v2) {
        CHVector3<N> r;

        r.x=(v1.y*v2.z-v1.z*v2.y);
        r.y=(v1.z*v2.x-v1.x*v2.z);
        r.z=(v1.x*v2.y-v1.y*v2.x);
        r.w=1;

        return r;
    }

    /**restituisce il versore*/
    template<class N> CHVector3<N> norm(const CHVector3<N>& v)
    {
        N d=sqrt(v.x*v.x+v.y*v.y+v.z*v.z);
        CHVector3<N> r;
        r.x=v.x/d;
        r.y=v.y/d;
        r.z=v.z/d;
        r.w=1;

        return r;
    }

    template<class N> CHVector3<N> create_vector(N x,N y,N z)
    {
        CHVector3<N> v;
        v.x=x;
        v.y=y;
        v.z=z;

        return v;
    }


#ifdef _USENAMESPACE_
};//fine namespace
#endif

#endif // VECTORS2_H_INCLUDED
