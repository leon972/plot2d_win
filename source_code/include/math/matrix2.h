/** Matrici di trasformazione geometrica 3x3 in coordinate omogenee*/

#ifndef MATRIX2_H_INCLUDED
#define MATRIX2_H_INCLUDED

#include "vectors3.h"
#include "vectors2.h"

namespace mathengine {

    template <class N> class CHMatrix33 {

    private:

        //NB: column major!
        N a1[16];
        N a2[16];

        N* cur;
        N* next;

        void reset() {
            memset(a1,0,sizeof(N)*16);
            cur=a1;
            next=a2;
        }

public:

        CHMatrix33() {
            reset();
        }

        //copy constructor
        CHMatrix33(const CHMatrix33<N>& m) {
            memcpy(a1,m.a1,16*sizeof(N));
            cur=a1;
        }

        //copy assignment
        CHMatrix33<N>& operator=(const CHMatrix33<N>& m) {
            if (&m!=this) {
                memcpy(cur,m.cur,16*sizeof(N));
            }

            return *this;
        }

        /**restituisce un elemento*/
        N get_value(int row,int col) const {
            return cur[col*4+row];
        }

        /**imposta un elemento*/
        void set_value(int row,int col,N value) {
            cur[col*4+row]=value;
        }

        /**restituisce la rappresentazione interna della matrice (formato column major)*/
        void copy_elements(N* dest) const {
            memcpy(dest,cur,16*sizeof(N));
        }

        /**Restituisce il puntatore diretto all'areqa di memoria degli elementi
        è usata per velocizzare alcune procedure critiche*/
        const N* get_col_major_elements()
        {
            return cur;
        }

        /**imposta direttamente gli elementi della matrice (formato column major)*/
        void set_elements(const N* elements)
        {
            memcpy(cur,elements,16*sizeof(N));
        }

        /**imposta la matrice identica*/
        void set_identity() {

            cur[0]=1;
            cur[5]=1;
            cur[10]=1;
            cur[15]=1;
            cur[1]=cur[2]=cur[3]=cur[4]=0;
            cur[6]=cur[7]=cur[8]=cur[9]=0;
            cur[11]=cur[12]=cur[13]=cur[14]=0;
        }

        /**moltiplica la matrice corrente per la matrice m*/
        void mul(const CHMatrix33<N>& m) {

            //1° riga
            next[0]=cur[0]*m.cur[0]+cur[4]*m.cur[1]+cur[8]*m.cur[2]+cur[12]*m.cur[3];
            next[4]=cur[0]*m.cur[4]+cur[4]*m.cur[5]+cur[8]*m.cur[6]+cur[12]*m.cur[7];
            next[8]=cur[0]*m.cur[8]+cur[4]*m.cur[9]+cur[8]*m.cur[10]+cur[12]*m.cur[11];
            next[12]=cur[0]*m.cur[12]+cur[4]*m.cur[13]+cur[8]*m.cur[14]+cur[12]*m.cur[15];

            //2° riga
            next[1]=cur[1]*m.cur[0]+cur[5]*m.cur[1]+cur[9]*m.cur[2]+cur[13]*m.cur[3];
            next[5]=cur[1]*m.cur[4]+cur[5]*m.cur[5]+cur[9]*m.cur[6]+cur[13]*m.cur[7];
            next[9]=cur[1]*m.cur[8]+cur[5]*m.cur[9]+cur[9]*m.cur[10]+cur[13]*m.cur[11];
            next[13]=cur[1]*m.cur[12]+cur[5]*m.cur[13]+cur[9]*m.cur[14]+cur[13]*m.cur[15];

            //3° riga
            next[2]=cur[2]*m.cur[0]+cur[6]*m.cur[1]+cur[10]*m.cur[2]+cur[14]*m.cur[3];
            next[6]=cur[2]*m.cur[4]+cur[6]*m.cur[5]+cur[10]*m.cur[6]+cur[14]*m.cur[7];
            next[10]=cur[2]*m.cur[8]+cur[6]*m.cur[9]+cur[10]*m.cur[10]+cur[14]*m.cur[11];
            next[14]=cur[2]*m.cur[12]+cur[6]*m.cur[13]+cur[10]*m.cur[14]+cur[14]*m.cur[15];

            //4° riga
            next[3]=cur[3]*m.cur[0]+cur[7]*m.cur[1]+cur[11]*m.cur[2]+cur[15]*m.cur[3];
            next[7]=cur[3]*m.cur[4]+cur[7]*m.cur[5]+cur[11]*m.cur[6]+cur[15]*m.cur[7];
            next[11]=cur[3]*m.cur[8]+cur[7]*m.cur[9]+cur[11]*m.cur[10]+cur[15]*m.cur[11];
            next[15]=cur[3]*m.cur[12]+cur[7]*m.cur[13]+cur[11]*m.cur[14]+cur[15]*m.cur[15];

            N* temp=next;
            next=cur;
            cur=temp;
        }

        /**moltiplicazione solo delle prime 3 righe e tre colonne
        (è utile quando si moltiplica per una matrice di rotazione*/

        void mul3x3(const CHMatrix33<N>& m)
        {
            //1° riga
            next[0]=cur[0]*m.cur[0]+cur[4]*m.cur[1]+cur[8]*m.cur[2];
            next[4]=cur[0]*m.cur[4]+cur[4]*m.cur[5]+cur[8]*m.cur[6];
            next[8]=cur[0]*m.cur[8]+cur[4]*m.cur[9]+cur[8]*m.cur[10];
            next[12]=cur[12];

            //2° riga
            next[1]=cur[1]*m.cur[0]+cur[5]*m.cur[1]+cur[9]*m.cur[2];
            next[5]=cur[1]*m.cur[4]+cur[5]*m.cur[5]+cur[9]*m.cur[6];
            next[9]=cur[1]*m.cur[8]+cur[5]*m.cur[9]+cur[9]*m.cur[10];
            next[13]=cur[13];

            //3° riga
            next[2]=cur[2]*m.cur[0]+cur[6]*m.cur[1]+cur[10]*m.cur[2];
            next[6]=cur[2]*m.cur[4]+cur[6]*m.cur[5]+cur[10]*m.cur[6];
            next[10]=cur[2]*m.cur[8]+cur[6]*m.cur[9]+cur[10]*m.cur[10];
            next[14]=cur[14];

            //4° riga
            next[3]=cur[3]*m.cur[0]+cur[7]*m.cur[1]+cur[11]*m.cur[2];
            next[7]=cur[3]*m.cur[4]+cur[7]*m.cur[5]+cur[11]*m.cur[6];
            next[11]=cur[3]*m.cur[8]+cur[7]*m.cur[9]+cur[11]*m.cur[10];
            next[15]=cur[15];

            N* temp=next;
            next=cur;
            cur=temp;

        }

        /**somma alla matrice corrente la matrice m*/
        void add(const CHMatrix33<N>& m) {
            cur[0] += m.cur[0];
            cur[1] += m.cur[1];
            cur[2] += m.cur[2];
            cur[3] += m.cur[3];
            cur[4] += m.cur[4];
            cur[5] += m.cur[5];
            cur[6] += m.cur[6];
            cur[7] += m.cur[7];
            cur[8] += m.cur[8];
            cur[9] += m.cur[9];
            cur[10] += m.cur[10];
            cur[11] += m.cur[11];
            cur[12] += m.cur[12];
            cur[13] += m.cur[13];
            cur[14] += m.cur[14];
            cur[15] += m.cur[15];
        }

        /**sottrae dalla matrice corrente la matrice m*/
        void sub(const CHMatrix33<N>& m) {
            cur[0] -= m.cur[0];
            cur[1] -= m.cur[1];
            cur[2] -= m.cur[2];
            cur[3] -= m.cur[3];
            cur[4] -= m.cur[4];
            cur[5] -= m.cur[5];
            cur[6] -= m.cur[6];
            cur[7] -= m.cur[7];
            cur[8] -= m.cur[8];
            cur[9] -= m.cur[9];
            cur[10] -= m.cur[10];
            cur[11] -= m.cur[11];
            cur[12] -= m.cur[12];
            cur[13] -= m.cur[13];
            cur[14] -= m.cur[14];
            cur[15] -= m.cur[15];
        }

        void mul_vect(CHVector3<N>* result,const CHVector3<N>& v) const {
            result->x=cur[0]*v.x+cur[4]*v.y+cur[8]*v.z+cur[12];
            result->y=cur[1]*v.x+cur[5]*v.y+cur[9]*v.z+cur[13];
            result->z=cur[2]*v.x+cur[6]*v.y+cur[10]*v.z+cur[14];
            result->w=1; //m->a30*v->x+m->a31*v->y+m->a32*v->z+m->a33*v->w;
        }

        CHVector3<N> mul_vect(const CHVector3<N>& v) const {
            CHVector3<N> r;
            mul_vect(&r,v);
            return r;
        }

        /**Moltiplica per un vettore a due componenti imponendo z=0*/
        void mul_vect(CVector2<N>* result,const CVector2<N>&v) const {

            result->x=cur[0]*v.x+cur[4]*v.y+cur[12];
            result->y=cur[1]*v.x+cur[5]*v.y+cur[13];
        }

        CVector2<N> mul_vect(const CVector2<N>& v) const {

            CVector2<N> r;
            mul_vect(&r,v);
            return r;
        }


        /**sostituisce la matrice corrente con la trasposta*/
        void transpose() {
            next[0]=cur[0];
            next[5]=cur[5];
            next[10]=cur[10];
            next[15]=cur[15];

            next[4]=cur[1];
            next[1]=cur[4];
            next[8]=cur[2];
            next[2]=cur[8];
            next[12]=cur[3];
            next[3]=cur[12];
            next[9]=cur[6];
            next[6]=cur[9];
            next[13]=cur[7];
            next[7]=cur[13];
            next[14]=cur[11];
            next[11]=cur[14];

            N* temp=next;
            next=cur;
            cur=temp;
        }

        /**moltiplica pèer uno scalare*/
        void mul_scalar(N scalar) {
            cur[0] *= scalar;
            cur[1] *= scalar;
            cur[2] *= scalar;
            cur[3] *= scalar;
            cur[4] *= scalar;
            cur[5] *= scalar;
            cur[6] *= scalar;
            cur[7] *= scalar;
            cur[8] *= scalar;
            cur[9] *= scalar;
            cur[10] *= scalar;
            cur[11] *= scalar;
            cur[12] *= scalar;
            cur[13] *= scalar;
            cur[14] *= scalar;
            cur[15] *= scalar;
        }

        /**calcola il determinante considerando la matrice come 3x3*/
        N det3x3 () const {
            return cur[0]*(cur[5]*cur[10]-cur[6]*cur[9])-cur[4]*(cur[1]*cur[10]-cur[2]*cur[9])+cur[8]*(cur[1]*cur[6]-cur[2]*cur[5]);
        }

        /**sostituisce la matrice corrente con l'inversa (la matrice è considerata 3 x 3).Se il det. è zero
        e quindi non si puo' calcolare l'inversa, rende false*/

        bool inverse3x3 () {

            N d=this->det3x3();

            if (d==0) return false; //non invertibile

            d=1.0/d;

            next[0]=d*(cur[5]*cur[10]-cur[6]*cur[9]);
            next[4]=d*(-cur[4]*cur[10]+cur[6]*cur[8]);
            next[8]=d*(cur[4]*cur[9]-cur[5]*cur[8]);

            next[1]=d*(-cur[1]*cur[10]+cur[2]*cur[9]);
            next[5]=d*(cur[0]*cur[10]-cur[2]*cur[8]);
            next[9]=d*(-cur[0]*cur[9]+cur[1]*cur[8]);

            next[2]=d*(cur[1]*cur[6]-cur[2]*cur[5]);
            next[6]=d*(-cur[0]*cur[6]+cur[2]*cur[4]);
            next[10]=d*(cur[0]*cur[5]-cur[4]*cur[1]);

            //coord. omogenee
            next[3]=next[7]=next[11]=next[14]=next[13]=next[12]=0;
            next[15]=1;

            N* temp=next;
            next=cur;
            cur=temp;

            return true;
        }

        /**trasforma la matrice corrente nella matrice di rotazione di theta radianti
        intorno al versore vers*/
        void make_rot_matrix(const CHVector3<N>& vers,const N theta) {

            N st=sin(theta);
            N ct=cos(theta);
            N ct1=1-ct;
            N xy=vers.x*vers.y;
            N xz=vers.x*vers.z;
            N yz=vers.y*vers.z;
            N x2=vers.x*vers.x;
            N y2=vers.y*vers.y;
            N z2=vers.z*vers.z;

            cur[3]=cur[7]=cur[11]=cur[14]=cur[13]=cur[12]=0;
            cur[15]=1;

            cur[0]=ct+ct1*x2;
            cur[4]=ct1*xy-st*vers.z;
            cur[8]=ct1*xz+st*vers.y;

            cur[1]=ct1*xy+st*vers.z;
            cur[5]=ct+ct1*y2;
            cur[9]=ct1*yz-st*vers.x;

            cur[2]=ct1*xz-st*vers.y;
            cur[6]=ct1*yz+st*vers.x;
            cur[10]=ct+ct1*z2;

        }

        /**la matrice corrente diventa la matrice di rotazione intorno a z di theta radianti*/
        void make_rot_z_matrix(N theta) {

            N ct=cos(theta);
            N st=sin(theta);

            cur[0]=ct;
            cur[4]=st;
            cur[8]=0;
            cur[12]=0;
            cur[1]=-st;
            cur[5]=ct;
            cur[9]=0;
            cur[13]=0;
            cur[2]=cur[6]=0;
            cur[10]=1;
            cur[14]=0;

            cur[3]=cur[7]=cur[11]=0;
            cur[15]=1;
        }

        /**la matrice corrente diventa la matrice di rotazione intorno a y di theta radianti*/
        void make_rot_y_matrix(N theta) {

            N ct=cos(theta);
            N st=sin(theta);
            cur[0]=ct;
            cur[4]=0;
            cur[8]=-st;
            cur[12]=0;
            cur[1]=0;
            cur[5]=1;
            cur[9]=0;
            cur[13]=0;
            cur[2]=st;
            cur[6]=0;
            cur[10]=ct;
            cur[14]=0;
            cur[3]=cur[7]=cur[11]=0;
            cur[15]=1;
        }

        /**la matrice corrente diventa la matrice di rotazione intorno a x di theta radianti*/
        void make_rot_x_matrix(N theta) {

            N ct=cos(theta);
            N st=sin(theta);

            cur[0]=1;
            cur[4]=0;
            cur[8]=0;
            cur[12]=0;
            cur[1]=0;
            cur[5]=ct;
            cur[9]=st;
            cur[13]=0;
            cur[2]=0;
            cur[6]=-st;
            cur[10]=ct;
            cur[14]=0;
            cur[3]=cur[7]=cur[11]=0;
            cur[15]=1;
        }

        /**Imposta la parte rotazionale della matrice rendendola uguale alla matrice di rotazione x
        lascia inalterata la parte traslazionale*/
        void set_rot_x(N theta)
        {
            N ct=cos(theta);
            N st=sin(theta);

            cur[0]=1;
            cur[4]=0;
            cur[8]=0;

            cur[1]=0;
            cur[5]=ct;
            cur[9]=st;

            cur[2]=0;
            cur[6]=-st;
            cur[10]=ct;
        }

        /**Imposta la parte rotazionale della matrice rendendola uguale alla matrice di rotazione z
        lascia inalterata la parte traslazionale*/
        void set_rot_z(N theta)
        {
            N ct=cos(theta);
            N st=sin(theta);

            cur[0]=ct;
            cur[4]=st;
            cur[8]=0;

            cur[1]=-st;
            cur[5]=ct;
            cur[9]=0;

            cur[2]=cur[6]=0;
            cur[10]=1;

        }

        /**Imposta la parte rotazionale della matrice rendendola uguale alla matrice di rotazione y
        lascia inalterata la parte traslazionale*/
        void set_rot_y(N theta)
        {
            N ct=cos(theta);
            N st=sin(theta);
            cur[0]=ct;
            cur[4]=0;
            cur[8]=-st;

            cur[1]=0;
            cur[5]=1;
            cur[9]=0;

            cur[2]=st;
            cur[6]=0;
            cur[10]=ct;

        }

        /**moltiplica la matrice corrente per la matrice di rotazione intorno ad x*/
        void rotate_x(N theta)
        {
            N ct,st;

            ct=cos(theta);
            st=sin(theta);

            next[4]=cur[4];
            next[5]=cur[5];
            next[6]=cur[6];
            next[8]=cur[8];
            next[9]=cur[9];
            next[10]=cur[10];

            cur[4]=next[4]*ct-next[8]*st;
            cur[5]=next[5]*ct-next[9]*st;
            cur[6]=next[6]*ct-next[10]*st;
            cur[8]=next[4]*st+next[8]*ct;
            cur[9]=next[5]*st+next[9]*ct;
            cur[10]=next[6]*st+next[10]*ct;
        }

        /**moltiplica la matrice corrente per la matrice di rotazione intorno ad y*/
        void rotate_y(N theta)
        {
            N ct,st;

            ct=cos(theta);
            st=sin(theta);

            next[0]=cur[0];
            next[1]=cur[1];
            next[2]=cur[2];
            next[8]=cur[8];
            next[9]=cur[9];
            next[10]=cur[10];

            cur[0]=next[0]*ct+next[8]*st;
            cur[1]=next[1]*ct+next[9]*st;
            cur[2]=next[2]*ct+next[10]*st;
            cur[8]=next[8]*ct-next[0]*st;
            cur[9]=next[9]*ct-next[1]*st;
            cur[10]=next[10]*ct-next[2]*st;

        }

        /**moltiplica la matrice corrente per la matrice di rotazione intorno a z*/
        void rotate_z(N theta)
        {
            N ct,st;

            ct=cos(theta);
            st=sin(theta);

            next[0]=cur[0];
            next[1]=cur[1];
            next[2]=cur[2];
            next[4]=cur[4];
            next[5]=cur[5];
            next[6]=cur[6];

            cur[0]=next[0]*ct-next[4]*st;
            cur[1]=next[1]*ct-next[5]*st;
            cur[2]=next[2]*ct-next[6]*st;

            cur[4]=next[0]*st+next[4]*ct;
            cur[5]=next[1]*st+next[5]*ct;
            cur[6]=next[2]*st+next[6]*ct;
        }

        void make_euler_matrix(const N psi,const N theta,const N phi) {

            N cp=cos(psi);
            N sp=sin(psi);

            N ct=cos(theta);
            N st=sin(theta);

            N cf=cos(phi);
            N sf=sin(phi);

            cur[0]=cf*cp-sf*ct*sp;
            cur[4]=cf*sp+sf*ct*cp;
            cur[8]=sf*st;
            cur[12]=0;

            cur[1]=-sf*cp-cf*ct*sp;
            cur[5]=-sf*sp+cf*ct*cp;
            cur[9]=cf*st;
            cur[13]=0;

            cur[2]=st*sp;
            cur[6]=-st*cp;
            cur[10]=ct;
            cur[14]=0;

            cur[3]=0;
            cur[7]=0;
            cur[11]=0;
            cur[15]=1;
        }

        /**Imposta la matrice corrente come matrice di traslazione*/
        void make_translation_matrix(const N tx,const N ty,const N tz) {

            cur[0]=1;
            cur[4]=0;
            cur[8]=0;
            cur[12]=tx;

            cur[1]=0;
            cur[5]=1;
            cur[9]=0;
            cur[13]=ty;

            cur[2]=0;
            cur[6]=0;
            cur[10]=1;
            cur[14]=tz;

            cur[3]=cur[7]=cur[11]=0;
            cur[15]=1;
        }

        void translate(const N tx,const N ty,const N tz)
        {
            next[12]=cur[12];
            next[13]=cur[13];
            next[14]=cur[14];

            cur[12]=cur[0]*tx+cur[4]*ty+cur[8]*tz+next[12];
            cur[13]=cur[1]*tx+cur[5]*ty+cur[9]*tz+next[13];
            cur[14]=cur[2]*tx+cur[6]*ty+cur[10]*tz+next[14];
        }

        void set_position(const N tx,const N ty,const N tz)
        {
            cur[12]=tx;
            cur[13]=ty;
            cur[14]=tz;
        }

        void make_scale_matrix(const N sx,const N sy,const N sz) {

            cur[0]=sx;
            cur[4]=0;
            cur[8]=0;
            cur[12]=0;

            cur[1]=0;
            cur[5]=sy;
            cur[9]=0;
            cur[13]=0;

            cur[2]=0;
            cur[6]=0;
            cur[10]=sz;
            cur[14]=0;

            cur[3]=0;
            cur[7]=0;
            cur[11]=0;
            cur[15]=1;
        }

    }; //end class

};

#endif // MATRIX2_H_INCLUDED
