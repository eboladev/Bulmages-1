/***************************************************************************
 *   Author: Philip Erdelsky's <pje@efgh.com>                              *
 *   http://efgh.com/software/fixed.htm                                    *
 *   Licence: Public Domain                                                *
 *   December 2, 1998                                                      *
 *   Modified by: Tomeu Borras Riera.                                      *
 ***************************************************************************/

#ifndef H__FIXED
#define H__FIXED

#include <QString>
#include <QChar>


const int MAX_FIXED_PRECISION = 12;
const int MAX_FIXED_LENGTH = 25;

typedef long long int Fixed_numerator;

inline long long int integer ( Fixed_numerator x )
{
    return ( int ) x;
}


class Fixed
{

private:
    void fromFixed ( const char * );

public:
    Fixed_numerator value;
    int precision;
    void equalize_precision ( Fixed & );
    void setprecision ( int );

public:
    Fixed ( int x, int p );
    Fixed ( QString a );
    Fixed ( const char *a );
    Fixed();
    Fixed operator [] ( int ) const;
    Fixed operator = ( Fixed );
    Fixed operator = ( int );
    QString toQString ( QChar separadorDecimal = ',', int precision = 2 );
    friend Fixed operator + ( Fixed, Fixed );
    friend Fixed operator - ( Fixed, Fixed );
    friend Fixed operator * ( Fixed, Fixed );
    friend Fixed operator / ( Fixed, Fixed );
    friend Fixed operator - ( Fixed );
    friend bool operator == ( Fixed, Fixed );
    friend bool operator < ( Fixed, Fixed );
    friend Fixed operator - ( int, Fixed );
    friend Fixed operator / ( int, Fixed );
    friend bool operator == ( int, Fixed );
    friend bool operator < ( int, Fixed );
    friend Fixed operator + ( Fixed, int );
    friend Fixed operator - ( Fixed, int );
    friend Fixed operator * ( Fixed, int );
    friend Fixed operator / ( Fixed, int );
    friend bool operator == ( Fixed, int );
    friend bool operator < ( Fixed, int );
    enum {ALIGN = 1, COMMAS = 2, DECIMAL = 4};
};


inline bool operator != ( Fixed x, Fixed y )
{
    return ! ( x == y );
}


inline bool operator != ( Fixed x, int y )
{
    return ! ( x == y );
}


inline bool operator != ( int x, Fixed y )
{
    return ! ( x == y );
}


inline bool operator >= ( Fixed x, Fixed y )
{
    return ! ( x < y );
}


inline bool operator >= ( Fixed x, int y )
{
    return ! ( x < y );
}


inline bool operator >= ( int x, Fixed y )
{
    return ! ( x < y );
}


inline bool operator > ( Fixed x, Fixed y )
{
    return y < x;
}


inline bool operator > ( Fixed x, int y )
{
    return y < x;
}


inline bool operator > ( int x, Fixed y )
{
    return y < x;
}


inline bool operator <= ( Fixed x, Fixed y )
{
    return ! ( y < x );
}


inline bool operator <= ( Fixed x, int y )
{
    return ! ( y < x );
}


inline bool operator <= ( int x, Fixed y )
{
    return ! ( y < x );
}

#endif

