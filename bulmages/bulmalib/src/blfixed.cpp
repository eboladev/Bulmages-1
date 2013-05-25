/***************************************************************************
 *   Author: Philip Erdelsky's <pje@efgh.com>                              *
 *   http://efgh.com/software/fixed.htm                                    *
 *   Licence: Public Domain                                                *
 *   December 2, 1998                                                      *
 *   Modified by: Tomeu Borras Riera.                                      *
 *   Modified by: Fco. Javier M. C. (2008)                                 *
 ***************************************************************************/

#include <QtCore/QString>
#include <QtCore/QChar>
#include <QtCore/QLocale>

#include "stdio.h"

#include "blfixed.h"


///
/**
\param x
\param p
**/
BlFixed::BlFixed ( int x, int p )
{
    BL_FUNC_DEBUG
    value = x;
    m_precision = p;
    
}


///
/**
\param a
**/
BlFixed::BlFixed ( QString a )
{
    BL_FUNC_DEBUG
    fromBlFixed ( a.toLatin1() );
    
}


///
/**
\param a
**/
BlFixed::BlFixed ( const char *a )
{
    BL_FUNC_DEBUG
    fromBlFixed ( a );
    
}


///
/**
**/
BlFixed::BlFixed()
{
    BL_FUNC_DEBUG
    value = 0;
    m_precision = 1;
    

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
    m_precision = x.m_precision;
    return *this;
}


BlFixed BlFixed::operator = ( int x )
{
    value = x;
    m_precision = 0;
    return *this;
}


BlFixed operator / ( BlFixed x, BlFixed y )
{
    int pres = x.precision() > y.precision() ? x.precision() : y.precision();
    x.setPrecision(6);
    y.setPrecision(6);
    x.value = (x.value * BlFixed::SCALE.x[y.m_precision]) / y.value;
    x.setPrecision(pres);
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
\param separadorDecimal: '0' usa el decimalPoint de las locales
\param precision
\param separadorMillares: '0' usa el groupSeparator de las locales
\return
**/
QString BlFixed::toQString ( QChar separadorDecimal, int precision, QChar separadorMillares )
{
    BL_FUNC_DEBUG
    BlDebug::blDebug ( "BlFixed::toQString", 0, QString::number(precision) );

    /// Si no hay una precision establecida cogemos por defecto la precision del numero a mostrar
    if (precision < 0)  {
        precision = m_precision;
        BlDebug::blDebug("la precision ha cambiado a " + QString::number(m_precision));
    } // end if

    QLocale locale;

	/// Si no se pasa separador decimal cogemos el de las locales
	if  ( separadorDecimal == '0' ) {
		separadorDecimal = locale.decimalPoint ();
	} // end if

/*
	/// Si no se pasa separador de millares cogemos el de las locales
	if  ( separadorMillares == '0' ) { //ARON
		separadorMillares = locale.groupSeparator (); //ARON
	} // end if
*/

    setPrecision ( precision );
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
    QString buffer = "";

    do {
/*
        if ( ( n - precision  - 1 ) % 3 == 0 && n - 1 > precision ) //ARON
          buffer = separadorMillares + buffer; //ARON
*/
        if ( n == precision ) {
            if ( n > 0 || options & DECIMAL )
              n ++;
            if ( precision > 0 ) 
              buffer = separadorDecimal + buffer;
            units = n;
        } // end if
        BlFixed_numerator y;
        y = ( BlFixed_numerator ) x / 10;
        n++;
        buffer = QString::number(x-y * 10) + buffer;
        x = y;
    } while ( n <= precision || x != 0 );
    if ( negative )
	  buffer = "-" + buffer;
	 n++;
    if ( options & ALIGN ) {
        while ( n - units < MAX_FIXED_LENGTH - 2 ) {
	      buffer = " " + buffer;
		  n++;
        } // end while
    } // end if

    
    return buffer;
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
    BL_FUNC_DEBUG
    while ( m_precision < x.m_precision )   {
        value *= 10;
        m_precision ++;
    } // end while
    while ( x.m_precision < m_precision )  {
        x.value *= 10 ;
        x.m_precision ++;
    } // end while
    
}


///
/**
\param prec
**/
void BlFixed::setPrecision ( int prec )
{
    BL_FUNC_DEBUG
    while ( m_precision < prec ) {
        value *= 10;
        m_precision ++;
    } // end while
    while ( prec < m_precision ) {
        BlFixed_numerator aux;
        aux = value;
        value = ( BlFixed_numerator ) ( value / 10 );
        if ( ( aux % 10 ) >= 5 ) {
            value++;
        } // end if
        m_precision--;
    } // end while
    
}


///
/**
\param s
**/
void BlFixed::fromBlFixed ( const char *s )
{
    BL_FUNC_DEBUG
    QLocale locale;
    value = 0;
    m_precision = 0;
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
    while ( m_precision < MAX_FIXED_PRECISION ) {
        if ( '0' <= c && c <= '9' ) {
            value = value * 10 + ( c - '0' );
            if ( decimal )
                m_precision++;
        } else if ( c == '.' || c == ',' ) {
            if ( decimal )
                break; 
            decimal = true;
        } else if ( c != ','  )
            break; 
        c = *s++;
    } // end while
    if ( negative )
        value = - value;
    if ( value == 0 )
        m_precision = 1;

    
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
    int pres = x.precision() > y.precision() ? x.precision() : y.precision();
    x.value = x.value * y.value;
    x.m_precision = x.m_precision + y.m_precision;
    x.setPrecision(pres);
    return x;
}


BlFixed BlFixed::operator [] ( int p ) const
{
    BL_FUNC_DEBUG
    BlFixed x ( 0, p );
    
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

