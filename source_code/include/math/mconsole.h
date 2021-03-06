/*********************************
Utilit� per la visualizzazione e
acquisizione tramite stadout e stdin
Code by Leonardo Berti (c) 2010
**********************************/

#ifndef MCONSOLE_H_INCLUDED
#define MCONSOLE_H_INCLUDED

#include "mathdefs.h"
#include "libmath.h"
#include "vectors.h"
#include "vectors2.h"
#include "vectors3.h"
#include "matrix2.h"
#include "quat2.h"
#include <cmath>
#include <iostream>

using namespace std;

#ifdef _USENAMESPACE_
namespace mathengine {
#endif

bool readReal(istream& s,REAL& r) {
    if (!(s>>r)) {
        cout<<"Bad input!A real is required."<<endl;
        s.clear();
        s.sync();
        return false;
    } else return true;
}

bool readComma(istream& s) {

    char ch;

    if (!(cin>>ch)) {
        cout<<"Bad input!A comma (,) is required."<<endl;
        s.clear();
        s.sync();
        return false;
    }

    if (ch==',') return true;
    cout<<"Bad input!A comma (,) is required."<<endl;
    s.clear();
    s.sync();
    return false;
}

/**Operatore >> usato per l'acquisizione del vettore dall'input stream*/
istream& operator>>(istream& s,CHVector3<REAL>& vi) {
    char ch;

    CHVector3<REAL> v;

    if (!(s>>v.x)) {
        cout<<"Bad Input!"<<endl;
        return s;
    }

    s>>ch;

    if (ch==',') {
        if (!(s>>v.y)) {
            cout<<"Bad Input!"<<endl;
            return s;
        }

        s>>ch;

        if (ch==',') {
            if (!(s>>v.z)) {
                cout<<"Bad Input!"<<endl;
                return s;
            }
            v.w=1;//input ok
        } else {
            cout<<"Bad input."<<endl;
            return s;
        }
    } else {
        cout<<"Bad input."<<endl;
        return s;
    }

    vi.x=v.x;
    vi.y=v.y;
    vi.z=v.z;
    vi.w=v.w;

    return s;
}

ostream& operator<<(ostream& s,const VECTOR2D &v) {
    return s<<"("<<v.x<<" , "<<v.y<<" )";
}

ostream& operator<<(ostream& s,const VECTOR3D &v) {
    return s<<"("<<v.x<<" , "<<v.y<<" , "<<v.z<<")";
}

/**Operatore di stampa su stdout del vettore 3D*/
ostream& operator<<(ostream& s,const CHVector3<REAL>& v) {
    return s<<"("<<v.x<<" , "<<v.y<<" , "<<v.z<<")";
}

/**Operatore di output vettore 2D*/
ostream& operator<<(ostream& s,const CVector2<REAL>& v)
{
    return s<<"("<<v.x<<" , "<<v.y<<")";
}

/**Stampa una matrice 3x3*/
ostream& operator<<(ostream& s,const CHMatrix33<REAL>& m) {

    const int width=20; //width (numero di carateri usati per ogni elemento)
    const int prec=6; //precision
    //salva lo stato di formattazione dell'output corrente
    ios_base::fmtflags oldf=s.flags();

    s.setf(ios_base::left,ios_base::adjustfield);

    streamsize oldp=s.precision(prec); //usa 6 cifre

    s.width(width); //usa sempre 8 caratteri
    s.fill(' '); //padding con spazio
    s<<m.get_value(0,0);
    s.width(width); //usa sempre 8 caratteri
    s<<m.get_value(0,1);
    s.width(width); //usa sempre 8 caratteri
    s<<m.get_value(0,2);
    s.width(4);
    s<<"|";
    s.width(width);
    s<<m.get_value(0,3)<<endl;

    s.width(width);
    s<<m.get_value(1,0);
    s.width(width);
    s<<m.get_value(1,1);
    s.width(width);
    s<<m.get_value(1,2);
    s.width(4);
    s<<"|";
    s.width(width);
    s<<m.get_value(1,3)<<endl;

    s.width(width);
    s<<m.get_value(2,0);
    s.width(width);
    s<<m.get_value(2,1);
    s.width(width);
    s<<m.get_value(2,2);
    s.width(4);
    s<<"|";
    s.width(width);
    s<<m.get_value(2,3)<<endl;

    s<<"----------------------------------------------------------------"<<endl;

    s.width(width);
    s<<m.get_value(3,0);
    s.width(width);
    s<<m.get_value(3,1);
    s.width(width);
    s<<m.get_value(3,2);
    s.width(4);
    s<<"|";
    s.width(width);
    s<<m.get_value(3,3)<<endl;

    //ripristina la precisione
    s.precision(oldp);
    //ripristina lo stato della formattazione
    s.flags(oldf);

    return s;
}

/**Legge una matrice dall'input*/
istream& operator>>(istream& s,CHMatrix33<REAL>& m) {
    CHVector3<REAL> row;

    m.set_identity();

    for (int i=0;i<3;++i) {
        cout<<"Insert row: a"<<(i+1)<<"1,a"<<(i+2)<<"2,a"<<(i+3)<<"3"<<endl;
        while (!(cin>>row)) {
            cout<<"Insert again: a"<<(i+1)<<"1,a"<<(i+2)<<"2,a"<<(i+3)<<"3"<<endl;
            cin.clear();
            cin.sync();
        }

        switch (i) {
        case 0:

            m.set_value(0,0,row.x);
            m.set_value(0,1,row.y);
            m.set_value(0,2,row.z);

            break;

        case 1:

            m.set_value(1,0,row.x);
            m.set_value(1,1,row.y);
            m.set_value(1,2,row.z);

            break;

        case 2:

            m.set_value(2,0,row.x);
            m.set_value(2,1,row.y);
            m.set_value(2,2,row.z);

            break;

        }
    }

    return s;

}

/**input di un quaternion*/
istream& operator>>(istream& s,CQuaternion<REAL>& q) {
    if (!readReal(s,q.r)) return s;
    if (!readComma(s)) return s;
    if (!readReal(s,q.x)) return s;
    if (!readComma(s)) return s;
    if (!readReal(s,q.y)) return s;
    if (!readComma(s)) return s;
    if (!readReal(s,q.z)) return s;

    return s;
}

ostream& operator<<(ostream& s,const CQuaternion<REAL>& q) {
    cout<<q.r<<" ,"<<q.x<<","<<q.y<<","<<q.z;
    return s;
}

#ifdef _USENAMESPACE_
};
#endif

#endif // MCONSOLE_H_INCLUDED
