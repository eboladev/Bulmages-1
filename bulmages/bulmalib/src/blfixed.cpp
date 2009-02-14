/***************************************************************************
 *   Author: Philip Erdelsky's <pje@efgh.com>                              *
 *   http://efgh.com/software/fixed.htm                                    *
 *   Licence: Public Domain                                                *
 *   December 2, 1998                                                      *
 *   Modified by: Tomeu Borras Riera.                                      *
 *   Modified by: Fco. Javier M. C. (2008)                                 *
 ***************************************************************************/

#include <QString>
#include <QChar>
#include <QLocale>

#include "stdio.h"

#include "blfixed.h"


///
/**
\param x
\param p
**/
BlFixed::BlFixed ( int x, int p )
{
    _depura ( "BlFixed::BlFixed", 0 );
    value = x;
    precision = p;
    _depura ( "END BlFixed::BlFixed", 0 );
}


///
/**
\param a
**/
BlFixed::BlFixed ( QString a )
{
    _depura ( "BlFixed::BlFixed", 0 );
    fromBlFixed ( a.toAscii() );
    _depura ( "END BlFixed::BlFixed", 0 );
}


///
/**
\param a
**/
BlFixed::BlFixed ( const char *a )
{
    _depura ( "BlFixed::BlFixed", 0 );
    fromBlFixed ( a );
    _depura ( "END BlFixed::BlFixed", 0 );
}


///
/**
**/
BlFixed::BlFixed()
{
    _depura ( "BlFixed::BlFixed", 0 );
    value = 0;
    precision = 1;
    _depura ( "END BlFixed::BlFixed", 0 );

}


BlFixed::scale BlFixed::SCALE;


BlFixed::scale::scale(void)
{
  int i;
  BlFixed_numerator n;
  n = 1;
  for (i = 0; i <= MAX_FIXED_PRECISION; i++)
  {
    x[i] = n;
    n = 10 * n;
  }
}


BlFixed operator + ( BlFixed x, BlFixed y )
{
    x.equalize_precision ( y );
    x.value = x.value + y.value;
    return x;
}



BlFixed operator + ( BlFixed x, int y )
{
    return x + BlFixed ( y, 0 );
}



BlFixed BlFixed::operator = ( BlFixed x )
{
    value = x.value;
    precision = x.precision;
    return *this;
}


BlFixed BlFixed::operator = ( int x )
{
    value = x;
    precision = 0;
    return *this;
}


BlFixed operator / ( BlFixed x, BlFixed y )
{
    x.setprecision(6);
    y.setprecision(6);
    x.value = (x.value * BlFixed::SCALE.x[y.precision]) / y.value;
    return x;
}


BlFixed operator / ( BlFixed x, int y )
{
    return x / BlFixed ( y, 0 );
}


BlFixed operator / ( int x, BlFixed y )
{
    return BlFixed ( x, 0 ) / y;
}


///
/**
\param separadorDecimal
\return
**/
QString BlFixed::toQString ( QChar separadorDecimal, int precision )
{
    _depura ( "BlFixed::toQString", 0 );
	/// Si no se pasa separador decimal cogemos el de las locales
	if  ( separadorDecimal == '0' ) {
		QLocale locale;
		separadorDecimal = locale.decimalPoint ();
	} // end if


    setprecision ( precision );
    int options = COMMAS;
    BlFixed_numerator x = value;
    bool negative;
    if ( x < 0 ) {
        x = -x;
        /// prevent buffer overflow if result is still negative.
        if ( x < 0 )
            x = x - 1;
        negative = true;
    } else
        negative = false;
    BlFixed_numerator n = 0;
    BlFixed_numerator units = 0;
    unsigned char buffer[MAX_FIXED_LENGTH + MAX_FIXED_PRECISION];
    for ( unsigned int i = 0; i <= sizeof ( buffer ); i++ )
        buffer[i] = 0;
    do {
        if ( n == precision ) {
            if ( n > 0 || options & DECIMAL )
                buffer[sizeof ( buffer ) - ++n] = separadorDecimal.toAscii();
            units = n;
        }
        BlFixed_numerator y;
        y = ( BlFixed_numerator ) x / 10;
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
    _depura ( "END BlFixed::toQString", 0 );
    return a;
}


bool operator == ( BlFixed x, BlFixed y )
{
    x.equalize_precision ( y );
    return x.value == y.value;
}


bool operator == ( BlFixed x, int y )
{
    return x == BlFixed ( y, 0 );
}



bool operator == ( int x, BlFixed y )
{
    return BlFixed ( x, 0 ) == y;
}


///
/**
\param x
**/
void BlFixed::equalize_precision ( BlFixed &x )
{
    _depura ( "BlFixed::equalize_precision", 0 );
    while ( precision < x.precision )   {
        value *= 10;
        precision ++;
    } // end while
    while ( x.precision < precision )  {
        x.value *= 10 ;
        x.precision ++;
    } // end while
    _depura ( "END BlFixed::equalize_precision", 0 );
}


///
/**
\param prec
**/
void BlFixed::setprecision ( int prec )
{
    _depura ( "BlFixed::setprecision", 0 );
    while ( precision < prec ) {
        value *= 10;
        precision ++;
    } // end while
    while ( prec < precision ) {
        BlFixed_numerator aux;
        aux = value;
        value = ( BlFixed_numerator ) ( value / 10 );
        if ( ( aux % 10 ) >= 5 ) {
            value++;
        } // end if
        precision--;
    } // end while
    _depura ( "END BlFixed::setprecision", 0 );
}


///
/**
\param s
**/
void BlFixed::fromBlFixed ( const char *s )
{
    _depura ( "BlFixed::fromBlFixed", 0 );
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

    _depura ( "END BlFixed::fromBlFixed", 0 );
}


bool operator < ( BlFixed x, BlFixed y )
{
    x.equalize_precision ( y );
    return x.value < y.value;
}


bool operator < ( BlFixed x, int y )
{
    return x < BlFixed ( y, 0 );
}


bool operator < ( int x, BlFixed y )
{
    return BlFixed ( x, 0 ) < y;
}


BlFixed operator - ( BlFixed x )
{
    x.value = -x.value;
    return x;
}


BlFixed operator * ( BlFixed x, int y )
{
    return x * BlFixed ( y, 0 );
}


BlFixed operator * ( BlFixed x, BlFixed y )
{
    x.value = x.value * y.value;
    x.precision = x.precision + y.precision;
    return x;
}


BlFixed BlFixed::operator [] ( int p ) const
{
    _depura ( "BlFixed::operator[]" );
    BlFixed x ( 0, p );
    _depura ( "END BlFixed::operator[]" );
    return x;
}


BlFixed operator - ( BlFixed x, BlFixed y )
{
    x.equalize_precision ( y );
    x.value = x.value - y.value;
    return x;
}


BlFixed operator - ( BlFixed x, int y )
{
    return x - BlFixed ( y, 0 );
}


BlFixed operator - ( int x, BlFixed y )
{
    return BlFixed ( x, 0 ) - y;
}

