/***************************************************************************
 *   Author: Philip Erdelsky's <pje@efgh.com>                              *
 *   http://efgh.com/software/fixed.htm                                    *
 *   Licence: Public Domain                                                *
 *   December 2, 1998                                                      *
 *   Modified by: Tomeu Borras Riera.                                      *
 ***************************************************************************/

#include <QString>
#include <QChar>

#include "stdio.h"

#include "fixed.h"
#include "funcaux.h"


///
/**
\param x
\param p
**/
Fixed::Fixed ( int x, int p )
{
    _depura ( "Fixed::Fixed", 0 );
    value = x;
    precision = p;
    _depura ( "END Fixed::Fixed", 0 );
}


///
/**
\param a
**/
Fixed::Fixed ( QString a )
{
    _depura ( "Fixed::Fixed", 0 );
    fromFixed ( a.toAscii() );
    _depura ( "END Fixed::Fixed", 0 );
}


///
/**
\param a
**/
Fixed::Fixed ( const char *a )
{
    _depura ( "Fixed::Fixed", 0 );
    fromFixed ( a );
    _depura ( "END Fixed::Fixed", 0 );
}


///
/**
**/
Fixed::Fixed()
{
    _depura ( "Fixed::Fixed", 0 );
    value = 0;
    precision = 1;
    _depura ( "END Fixed::Fixed", 0 );

}


Fixed operator + ( Fixed x, Fixed y )
{
    x.equalize_precision ( y );
    x.value = x.value + y.value;
    return x;
}



Fixed operator + ( Fixed x, int y )
{
    return x + Fixed ( y, 0 );
}



Fixed Fixed::operator = ( Fixed x )
{
    value = x.value;
    precision = x.precision;
    return *this;
}


Fixed Fixed::operator = ( int x )
{
    value = x;
    precision = 0;
    return *this;
}


Fixed operator / ( Fixed x, Fixed y )
{
    x.value = x.value / y.value;
    x.precision += y.precision;
    return x;
}


Fixed operator / ( Fixed x, int y )
{
    return x / Fixed ( y, 0 );
}


Fixed operator / ( int x, Fixed y )
{
    return Fixed ( x, 0 ) / y;
}


///
/**
\param separadorDecimal
\return
**/
QString Fixed::toQString ( QChar separadorDecimal, int precision )
{
    _depura ( "Fixed::toQString", 0 );
    setprecision ( precision );
    int options = COMMAS;
    Fixed_numerator x = value;
    bool negative;
    if ( x < 0 ) {
        x = -x;
        /// prevent buffer overflow if result is still negative.
        if ( x < 0 )
            x = x - 1;
        negative = true;
    } else
        negative = false;
    Fixed_numerator n = 0;
    Fixed_numerator units = 0;
    unsigned char buffer[MAX_FIXED_LENGTH + MAX_FIXED_PRECISION];
    for ( unsigned int i = 0; i <= sizeof ( buffer ); i++ )
        buffer[i] = 0;
    do {
        if ( n == precision ) {
            if ( n > 0 || options & DECIMAL )
                /// 10/07/2007
                /// Se cambio para poder poner otro caracter como separador decimal.
                //buffer[sizeof(buffer) - ++n] = ',';

                buffer[sizeof ( buffer ) - ++n] = separadorDecimal.toAscii();

            units = n;
        }
        Fixed_numerator y;
        y = ( Fixed_numerator ) x / 10;
        buffer[sizeof ( buffer ) - ++n] = integer ( x - y * 10 ) + '0';
        x = y;
    } while ( n <= precision || x != 0 );
    if ( negative )
        buffer[sizeof ( buffer ) - ++n] = '-';
    if ( options & ALIGN ) {
        while ( n - units < MAX_FIXED_LENGTH - 2 )
            buffer[sizeof ( buffer ) - ++n] = ' ';
    }
    QString a ( ( const char * ) buffer + sizeof ( buffer ) - n );
    _depura ( "END Fixed::toQString", 0 );
    return a;
}


bool operator == ( Fixed x, Fixed y )
{
    x.equalize_precision ( y );
    return x.value == y.value;
}


bool operator == ( Fixed x, int y )
{
    return x == Fixed ( y, 0 );
}



bool operator == ( int x, Fixed y )
{
    return Fixed ( x, 0 ) == y;
}


///
/**
\param x
**/
void Fixed::equalize_precision ( Fixed &x )
{
    _depura ( "Fixed::equalize_precision", 0 );
    while ( precision < x.precision )   {
        value *= 10;
        precision ++;
    } // end while
    while ( x.precision < precision )  {
        x.value *= 10 ;
        x.precision ++;
    } // end while
    _depura ( "END Fixed::equalize_precision", 0 );
}


///
/**
\param prec
**/
void Fixed::setprecision ( int prec )
{
    _depura ( "Fixed::setprecision", 0 );
    while ( precision < prec ) {
        value *= 10;
        precision ++;
    } // end while
    while ( prec < precision ) {
        Fixed_numerator aux;
        aux = value;
        value = ( Fixed_numerator ) ( value / 10 );
        if ( ( aux % 10 ) >= 5 ) {
            value++;
        } // end if
        precision--;
    } // end while
    _depura ( "END Fixed::setprecision", 0 );
}


///
/**
\param s
**/
void Fixed::fromFixed ( const char *s )
{
    _depura ( "Fixed::fromFixed", 0 );
    value = 0;
    precision = 0;
    int c;
    while ( ( c = *s++ ) == ' ' || c == '\t' )
        ;
    bool negative;
    if ( c == '-' ) {
        negative = true;
        c = *s++;
    } else {
        negative = false;
    } // end if
    bool decimal = false;
    while ( precision < MAX_FIXED_PRECISION ) {
        if ( '0' <= c && c <= '9' ) {
            value = value * 10 + ( c - '0' );
            if ( decimal )
                precision++;
        } else if ( c == '.' || c == ',' ) {
            if ( decimal )
                break;
            decimal = true;
        } else if ( c != ',' )
            break;
        c = *s++;
    } // end while
    if ( negative )
        value = - value;
    if ( value == 0 )
        precision = 1;
    _depura ( "END Fixed::fromFixed", 0 );
}


bool operator < ( Fixed x, Fixed y )
{
    x.equalize_precision ( y );
    return x.value < y.value;
}


bool operator < ( Fixed x, int y )
{
    return x < Fixed ( y, 0 );
}


bool operator < ( int x, Fixed y )
{
    return Fixed ( x, 0 ) < y;
}


Fixed operator - ( Fixed x )
{
    x.value = -x.value;
    return x;
}


Fixed operator * ( Fixed x, int y )
{
    return x * Fixed ( y, 0 );
}


Fixed operator * ( Fixed x, Fixed y )
{
    x.value = x.value * y.value;
    x.precision = x.precision + y.precision;
    return x;
}


Fixed Fixed::operator [] ( int p ) const
{
    _depura ( "Fixed::operator[]" );
    Fixed x ( 0, p );
    _depura ( "END Fixed::operator[]" );
    return x;
}


Fixed operator - ( Fixed x, Fixed y )
{
    x.equalize_precision ( y );
    x.value = x.value - y.value;
    return x;
}


Fixed operator - ( Fixed x, int y )
{
    return x - Fixed ( y, 0 );
}


Fixed operator - ( int x, Fixed y )
{
    return Fixed ( x, 0 ) - y;
}

