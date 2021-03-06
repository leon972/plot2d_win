/***************************************
Alcune definizioni usate dal math engine
*****************************************/



#ifndef MATHDEFS_H_INCLUDED
#define MATHDEFS_H_INCLUDED

/**
 *Fattore di conversione da gradi in radianti
 */
#define M_CONV 0.01745329251994329577

/**
 *Fattore di conversione da radianti in gradi
 */
#define M_CONV1 57.2957795130823208768

/**
 *Converte da gradi in radianti
 */
#define DEG_TO_RAD(x) x*M_CONV

/**
 *Converte da radianti in gradi
 */
#define RAD_TO_DEG(x) x*M_CONV1

//usato per le approssimazioni

#define _USENAMESPACE_

#ifdef _USENAMESPACE_
namespace mathengine
{
#endif

typedef double REAL;

const double EPSILON=0.0000000001;

    //funzione di singola variabile
typedef REAL (*FUNCT_X)(REAL);

//funzione di due variabili
typedef REAL (*FUNCT_XY)(REAL,REAL);

//funzione di tre variabili
typedef REAL (*FUNCT_XYZ)(REAL,REAL,REAL);

//eccezione divisione per zero
class DivideByZeroVideoException {};

//argomento non valido per una funzione (es. radici negative)
class InvalidArgumentVideoException {};

//functors
/**
interfaccia funzioni di una variabile
*/
class IFunctionX
{
    public:
    virtual double operator()(double x)=0;
};

/**
Funzioni di due variabili
*/
class IFunctionXY
{
    public:
    virtual double operator()(double x,double y)=0;
};

/**
Funzioni di tre variabili
*/
class IFunctionXYZ
{
    public:
    virtual double operator()(double x,double y,double z)=0;
};

#ifdef _USENAMESPACE_
};
#endif

#endif // MATHDEFS_H_INCLUDED
