/***************************************************************************
 *   Author: Philip Erdelsky's <pje@efgh.com>                              *
 *   http://efgh.com/software/fixed.htm                                    *
 *   Licence: Public Domain                                                *
 *   December 2, 1998                                                      *
 *   Modified by: Tomeu Borras Riera.                                      *
 *   Modified by: Fco. Javier M. C. (2008)                                 *
 ***************************************************************************/

#ifndef BLFIXED_H
#define BLFIXED_H

#include <QString>
#include <QChar>

#include "blfunctions.h"


const int MAX_FIXED_PRECISION = 15;
const int MAX_FIXED_LENGTH = 28;

typedef long long int BlFixed_numerator;

inline long long int BL_EXPORT integer ( BlFixed_numerator x )
{
    return ( int ) x;
}


class BL_EXPORT BlFixed
{

private:
    void fromBlFixed ( const char * );

public:
    BlFixed_numerator value;
    int precision;
    void equalize_precision ( BlFixed & );
    void setprecision ( int );
    class scale
    {
      public:
        BlFixed_numerator x[MAX_FIXED_PRECISION + 1];
        scale();
    };
    static scale SCALE;

public:
    BlFixed ( int x, int p );
    BlFixed ( QString a );
    BlFixed ( const char *a );
    BlFixed();
    BlFixed operator [] ( int ) const;
    BlFixed operator = ( BlFixed );
    BlFixed operator = ( int );
    QString toQString ( QChar separadorDecimal = '0', int precision = 2 );
    friend BlFixed operator + ( BlFixed, BlFixed );
    friend BlFixed operator - ( BlFixed, BlFixed );
    friend BlFixed operator * ( BlFixed, BlFixed );
    friend BlFixed operator / ( BlFixed, BlFixed );
    friend BlFixed operator - ( BlFixed );
    friend bool operator == ( BlFixed, BlFixed );
    friend bool operator < ( BlFixed, BlFixed );
    friend BlFixed operator - ( int, BlFixed );
    friend BlFixed operator / ( int, BlFixed );
    friend bool operator == ( int, BlFixed );
    friend bool operator < ( int, BlFixed );
    friend BlFixed operator + ( BlFixed, int );
    friend BlFixed operator - ( BlFixed, int );
    friend BlFixed operator * ( BlFixed, int );
    friend BlFixed operator / ( BlFixed, int );
    friend bool operator == ( BlFixed, int );
    friend bool operator < ( BlFixed, int );
    enum {ALIGN = 1, COMMAS = 2, DECIMAL = 4};
};


inline bool BL_EXPORT operator != ( BlFixed x, BlFixed y )
{
    return ! ( x == y );
}


inline bool BL_EXPORT operator != ( BlFixed x, int y )
{
    return ! ( x == y );
}


inline bool BL_EXPORT operator != ( int x, BlFixed y )
{
    return ! ( x == y );
}


inline bool BL_EXPORT operator >= ( BlFixed x, BlFixed y )
{
    return ! ( x < y );
}


inline bool BL_EXPORT operator >= ( BlFixed x, int y )
{
    return ! ( x < y );
}


inline bool BL_EXPORT operator >= ( int x, BlFixed y )
{
    return ! ( x < y );
}


inline bool BL_EXPORT operator > ( BlFixed x, BlFixed y )
{
    return y < x;
}


inline bool BL_EXPORT operator > ( BlFixed x, int y )
{
    return y < x;
}


inline bool BL_EXPORT operator > ( int x, BlFixed y )
{
    return y < x;
}


inline bool BL_EXPORT operator <= ( BlFixed x, BlFixed y )
{
    return ! ( y < x );
}


inline bool BL_EXPORT operator <= ( BlFixed x, int y )
{
    return ! ( y < x );
}


inline bool BL_EXPORT operator <= ( int x, BlFixed y )
{
    return ! ( y < x );
}

#endif

