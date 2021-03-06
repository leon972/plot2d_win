/**Quaternion*/
#ifndef QUAT2_H_INCLUDED
#define QUAT2_H_INCLUDED

#include <cmath>
#include "vectors3.h"
#include "matrix2.h"

namespace mathengine {

    const double pi_2=1.57079632679489661923; //pi greco/2

    template<class N> class CQuaternion {
public:

        N x,y,z; //parte immaginaria
        N r; //parte reale

        void set(N r,N x,N y,N z) {
            this->r=r;
            this->x=x;
            this->y=y;
            this->z=z;
        }

        CQuaternion() {
            set(0,0,0,0);
        }

        CQuaternion(N r,N x,N y,N z) {
            set(r,x,y,z);
        }

        /**imposta il quaternion corrente a partire dal vettore*/
        void from_vector(const CHVector3<N>& v) {
            x=v.x;
            y=v.y;
            z=v.z;
            r=0;
        }

        /**crea un vettore a partire dal quaternion*/
        void to_vector(CHVector3<N>* v) {
            v->x=x;
            v->y=y;
            v->z=z;
        }

        CHVector3<N> to_vector() {
            CHVector3<N> v;
            to_vector(&v);
            return v;
        }

        /**aggiunge il quaternion corrente*/
        void add(const CQuaternion<N>& q) {
            x+=q.x;
            y+=q.y;
            z+=q.z;
            r+=q.r;
        }

        /**sottrae q dal quaternion corrente*/
        void sub(const CQuaternion<N>& q) {
            x-=q.x;
            y-=q.y;
            z-=q.z;
            r-=q.r;
        }

        void normalize() {
            N d=1/sqrt(r*r+x*x+y*y+z*z);

            r=r*d;
            x=x*d;
            y=y*d;
            z=z*d;
        }

        N magnitude() {
            return sqrt(r*r+x*x+y*y+z*z);
        }

        /**trasforma il quaternion corrente nel suo coniugato*/
        void conjugate() {
            x=-x;
            y=-y;
            z=-z;
        }

        /**moltiplica per il quaternion q*/
        void mul(const CQuaternion<N>& q) {

            //riduce il numero di moltiplicazioni ragruppando
            N prd_0 = (z - y) * (q.y - q.z);
            N prd_1 = (r + x) * (q.r + q.x);
            N prd_2 = (r - x) * (q.y + q.z);
            N prd_3 = (y + z) * (q.r - q.x);
            N prd_4 = (z - x) * (q.x - q.y);
            N prd_5 = (z + x) * (q.x + q.y);
            N prd_6 = (r + y) * (q.r - q.z);
            N prd_7 = (r - y) * (q.r + q.z);
            N prd_8 = prd_5 + prd_6 + prd_7;
            N prd_9 = 0.5 * (prd_4 + prd_8);

            r = prd_0 + prd_9 - prd_5;
            x = prd_1 + prd_9 - prd_8;
            y = prd_2 + prd_9 - prd_7;
            z = prd_3 + prd_9 - prd_6;
        }

        /**ottiene un quatenion a partire dal versore della direzione di rotazione e dall'angolo in radianti*/
        void set_rotation(const CHVector3<N>& versAxis,N theta) {

            N theta_div_2 = 0.5*theta;

            N st2 = sin(theta_div_2);

            x = st2 * versAxis.x;
            y = st2 * versAxis.y;
            z = st2 * versAxis.z;
            r = cos(theta_div_2);
        }

        /**triplo prodotto:il quaternion corrente viene sostituito dal prodotto q1*q2*q3*/

        void triple_product(const CQuaternion<N>& q1,const CQuaternion<N>& q2,const CQuaternion<N>& q3) {

            *this=q1;
            mul(q2);
            mul(q3);
        }

        /**ottiene la marice di rotazione a partire da un quaternion di rotazione
             Matrix =  [ 1 - 2y2 - 2z2    2xy - 2wz      2xz + 2wy
             2xy + 2wz    1 - 2x2 - 2z2    2yz - 2wx
             2xz - 2wy      2yz + 2wx    1 - 2x2 - 2y2 ]
        */

        void to_matrix(CHMatrix33<N>* m) {

            N el[16];

            el[3]=el[7]=el[11]=0;
            el[15]=1;

            N xy=x*y;
            N xz=x*z;
            N yz=y*z;
            N wx=x*r;
            N wy=y*r;
            N wz=z*r;

            N x2=x*x;
            N y2=y*y;
            N z2=z*z;

            el[0]=1-2*y2-2*z2;
            el[4]=2*xy-2*wz;
            el[8]=2*xz+2*wy;
            el[12]=0;

            el[1]=2*xy+2*wz;
            el[5]=1-2*x2-2*z2;
            el[9]=2*yz-2*wx;
            el[13]=0;

            el[2]=2*xz-2*wy;
            el[6]=2*yz+2*wx;
            el[10]=1-2*x2-2*y2;
            el[14]=0;

            m->set_elements(el);
        }

        CHMatrix33<N> to_matrix() {
            CHMatrix33<N> m;
            to_matrix(&m);
            return m;
        }

        /**ottiene il quaternion da una matrice*/
        void from_matrix(const CHMatrix33<N>& matrix) {

            N m[16];

            matrix.copy_elements(m);

            N T=1.0+m[0]+m[5]+m[10]; //1+ diagonale
            N S;

            if (T>EPSILON) { //esegue questo controllo per evitare distorsioni

                S=sqrt(T)*2;
                r=0.25*S;
                x=(m[6]-m[9])/S;
                y=(m[8]-m[2])/S;
                z=(m[1]-m[4])/S;

            } else {
                //determina l'elemento diagonale maggiore
                if (m[0]>m[5] && m[0]>m[10]) {

                    S=sqrt(1.0+m[0]-m[5]-m[10])*2;
                    x = 0.25 * S;
                    y = (m[1] + m[4]) / S;
                    z = (m[8] + m[2] ) / S;
                    r = (m[6] - m[9]) / S;

                } else if (m[5]>m[10]) {

                    S  = sqrt( 1.0 + m[5] - m[0] - m[10]) * 2;
                    x = (m[1] + m[4]) / S;
                    y = 0.25 * S;
                    z = (m[6] + m[9]) / S;
                    r = (m[8] - m[2]) / S;

                } else {
                    S  = sqrt( 1.0 + m[10] - m[0] - m[5]) * 2;
                    x = (m[8] + m[2] ) / S;
                    y = (m[6] + m[9] ) / S;
                    z = 0.25 * S;
                    r = (m[1] - m[4]) / S;
                }
            }

        }

        /**SLERP:(spherical interpolation)
        ottiene un quaternion compreso fra il quaternion iniziale qa e quello finale qb
        basato sul parametro t compreso fra 0 e 1.t=0 rende qa t=1 rende qb
        */
        void slerp(const CQuaternion<N>& qa,const CQuaternion<N>& qb,N t) {

            N theta,costheta,sintheta,ratio1,ratio2;

            //calcola il coseno fra gli angoli dei due quaternion
            costheta=qa.x*qb.x+qa.y*qb.y+qa.z*qb.z+qa.r*qb.r;

            //controlla che si stia percorrendo la strada piu' corta
            if ((1.0+costheta)>EPSILON) {

                // se l'angolo non � troppo piccolo si usa lo slerp
                if ((1.0 - costheta) > EPSILON) {

                    theta = acos(costheta);
                    sintheta = sin(theta);
                    ratio1 = sin((1.0 - t) * theta) / sintheta;
                    ratio2 = sin(t * theta) / sintheta;

                } else {
                    // per piccoli angoli usa il LERP (interpolazione lineare)
                    ratio1 = 1.0 - t;
                    ratio2 = t;
                }

                x = ratio1 * qa.x + ratio2 * qb.x;
                y = ratio1 * qa.y + ratio2 * qb.y;
                z = ratio1 * qa.z + ratio2 * qb.z;
                r = ratio1 * qa.r + ratio2 * qb.r;

            } else {

                x = -qb.y;
                y = qb.x;
                z = -qb.r;
                r = qb.z;

                ratio1 = sin((1.0 - t) * static_cast<N>(pi_2));
                ratio2 = sin(t * static_cast<N>(pi_2));

                x = ratio1 * qa.x + ratio2 * x;
                y = ratio1 * qa.y + ratio2 * y;
                z = ratio1 * qa.z + ratio2 * z;
                r = ratio1 * qa.r + ratio2 * r;
            }
        }

    };
};



#endif // QUAT2_H_INCLUDED
