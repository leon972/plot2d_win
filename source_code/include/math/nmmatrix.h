/***************************************
Matrice nxm
Code by L.Berti (c) 2009
****************************************/

#ifndef NMMATRIX_H_INCLUDED
#define NMMATRIX_H_INCLUDED

#include "math/mathdefs.h"
#include "libmath.h"
#include "math/nvector.h"
#include <stddef.h>
#include <iostream>

#ifdef _USENAMESPACE_
namespace mathengine {
    #endif

    typedef enum
    {
        ABOVE_DIAGONAL=0,  //elementi sopra la diagonale
        UNDER_DIAGONAL=1   //elementi sotto la diagonale

    } MATRIX_ELEMS;

    using namespace std;

    MATH_EXPORT class NMMatrix {
protected:

        int n; //numero di righe
        int m; //numero colonne
        size_t matrix_size;
        REAL* data; //elementi della matrice (row major)

        NMMatrix();

public:

        NMMatrix(int nrows,int ncolumns,bool set_to_zero);
        virtual ~NMMatrix();

        /**assegnamento*/
        NMMatrix& operator = (const NMMatrix& m);

        /**Resetta la matrice e ne cambia le dimensioni*/
        void Reset(int nrows,int ncolumns,bool set_to_zero);

        /**restituisce un elemento*/
        REAL get_value(int row,int col) const;
        /**Imposta un valore della matrice*/
        void set_value(int row,int col,REAL value);
        /**Imposta una intera riga*/
        void set_row(int row,const REAL* values);
        /**Imposta una parte di riga*/
        void set_row(int row,const REAL* values,int row_size) throw (MathException);
        /**Acquisisce una intera riga*/
        const REAL* get_row(int row) const;
        /**Imposta una colonna*/
        void set_column(int col,const REAL* values);
        void set_column(int col,const NVector& col_data);
        /**scambia due righe*/
        void SwapRows(int row1,int row2);
        /**scambia due colonne*/
        void SwapColumns(int col1,int col2);
        /**Restituisce una riga
        //nota � possibile usando questo operatore accedere agli elementi in questo modo m[i][j]
        */
        REAL* operator [] (size_t row);
        /**Restituisce le dimensioni della matrice*/
        int GetRows() const;
        int GetColumns() const;
        /**Restituisce le dimensioni [bytes] della matrice*/
        size_t GetDataSize() const;
        /**Moltiplica la matrice corrente per m e mette il risultato in result*/
        void mul(NMMatrix* result,const NMMatrix& m) const;
        NMMatrix operator * (const NMMatrix& m) const;
        /**moltiplica la matrice per il vettore v e restituisce un vettore*/
        void mul(NVector* res,const NVector& v) const throw (MathException);

        NVector operator * (const NVector& v) const throw (MathException);

        /**Somma la matrice corrente a m e mette il risultato in result*/
        void add(NMMatrix* result,const NMMatrix& m) const;

        /**Sottrae m dalla matrice corrente e mette il risultato in result*/
        void sub(NMMatrix* result,const NMMatrix& m) const;
        NMMatrix operator - (const NMMatrix& m) const;
        /**Setta a zero tutti gli elementi*/
        void ZeroMatrix();

    };

    /* nota:
       � possibile evitare di copiare il valore di ritorno dall'operatore
       definendo l'operatore come
       NMMatrix& operator + (const NMMatrix& m1,const NMMatrix& m2);
       restituendo cio� un reference.In questo modo pero' non si puo' restituire
       un reference ad un oggetto locale.Occorre definire uno stack di matrici
       temporanee e restituire un elemento di quello stack.Questo mnetodo pero' non funziona
       se i calcoli coinvolgono piu' matrici temporanee di quanti sono gli elementi dello stack
    */

    //NMMatrix operator + (const NMMatrix& m1,const NMMatrix& m2);


    /**matrice quadrata*/
MATH_EXPORT class Matrix:public NMMatrix {
protected:

public:

        Matrix(int n,bool set_to_zero);
        virtual ~Matrix();

        // Matrix& Matrix::operator = (const Matrix& mat);

        /**Carica la matrice identica (solo se la matrice � quadrata)*/
        void LoadIdentity();
        /**Estrae la diagonale*/
        void GetDiagonal(REAL* diag);

        Matrix operator * (const NMMatrix& m) const;
        Matrix operator + (const NMMatrix& m) const;
        Matrix operator - (const NMMatrix& m) const;
        NVector operator * (const NVector& v) const throw (MathException);

        /**Rende la matrice simmetrica*/
        void MakeSymmetric(MATRIX_ELEMS elems);
        /**Rende la matrice triangolare*/
        void MakeTriangular(MATRIX_ELEMS elems);
        /**Rende la matrice a banda
        width=numero di bande/2 se width=0 la matrice � diagonale se n=1 tridiagonale ecc...
        */
        void MakeDiagonal(int width);

    };

 //operatori & utility

 ostream& operator<<(ostream& s,const NMMatrix& m);

    #ifdef _USENAMESPACE_
};
#endif

#endif // NMMATRIX_H_INCLUDED
