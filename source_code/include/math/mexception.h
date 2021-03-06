#ifndef MVideoException_H_INCLUDED
#define MVideoException_H_INCLUDED

#include "mathdefs.h"
#include "libmath.h"

#ifdef _USENAMESPACE_
namespace mathengine {
#endif

MATH_EXPORT class MathException
{
    protected:

    const char* msg;

    public:

    MathException(const char* msg);
    const char* GetMessage();
};

#ifdef _USENAMESPACE_
};
#endif

#endif // MVideoException_H_INCLUDED
