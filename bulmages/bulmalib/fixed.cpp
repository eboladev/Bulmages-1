/***************************************************************************
 *   Author: Philip Erdelsky's <pje@efgh.com>                              *
 *   http://efgh.com/software/fixed.htm                                    *
 *   Licence: Public Domain                                                *
 *   December 2, 1998                                                      *
 *   Modified by: Tomeu Borras Riera.                                      *
 ***************************************************************************/

#include "fixed.h"

#include "stdio.h"
#include <qstring.h>

#include "funcaux.h"


Fixed operator + (Fixed x, Fixed y) {
    x.equalize_precision(y);
    x.value = x.value + y.value;
    return x;
}


Fixed operator + (Fixed x, int y) {
    return x + Fixed(y, 0);
}


Fixed Fixed::operator = (Fixed x) {
    value = x.value;
    precision = x.precision;
    return *this;
}


Fixed Fixed::operator = (int x) {
    value = x;
    precision = 0;
    return *this;
}


Fixed operator / (Fixed x, Fixed y) {
    x.value = x.value / y.value;
    x.precision += y.precision;
    return x;
}


Fixed operator / (Fixed x, int y) {
    return x / Fixed(y, 0);
}


Fixed operator / (int x, Fixed y) {
    return Fixed(x, 0) / y;
}


QString Fixed::toQString() {
    setprecision(2);
    int options = COMMAS;
    Fixed_numerator x = value;
    bool negative;
    if (x < 0) {
        x = -x;
        /// prevent buffer overflow if result is still negative
        if (x < 0)
            x = x - 1;
        negative = true;
    } else
        negative = false;
    Fixed_numerator n = 0;
    Fixed_numerator units = 0;
    unsigned char buffer[MAX_FIXED_LENGTH + MAX_FIXED_PRECISION];
    for (unsigned int i = 0; i <= sizeof(buffer); i++)
        buffer[i] = 0;
    do {
        if (n == precision) {
            if (n > 0 || options & DECIMAL)
                buffer[sizeof(buffer) - ++n] = '.';
            units = n;
        }
        Fixed_numerator y;
        y = (Fixed_numerator) x / 10;
        buffer[sizeof(buffer) - ++n] = integer(x - y * 10) + '0';
        x = y;
    } while (n <= precision || x != 0);
    if (negative)
        buffer[sizeof(buffer) - ++n] = '-';
    if (options & ALIGN) {
        while (n - units < MAX_FIXED_LENGTH - 2)
            buffer[sizeof(buffer) - ++n] = ' ';
    }
    QString a((const char *) buffer + sizeof(buffer) - n);
    return a;
}


bool operator == (Fixed x, Fixed y) {
    x.equalize_precision(y);
    return x.value == y.value;
}


bool operator == (Fixed x, int y) {
    return x == Fixed(y, 0);
}


bool operator == (int x, Fixed y) {
    return Fixed(x, 0) == y;
}


void Fixed::equalize_precision(Fixed &x) {
    _depura("equalize_precision");
    while (precision < x.precision)   {
        value *= 10;
        precision ++;
    } // end while
    while (x.precision < precision)  {
        x.value *= 10 ;
        x.precision ++;
    } // end while
    _depura("Fin equalize_precision");
}


void Fixed::setprecision(int prec) {
    while (precision < prec) {
        value *= 10;
        precision ++;
    } // end while
    while (prec < precision) {
        Fixed_numerator aux;
        aux = value;
        value = (Fixed_numerator) (value / 10);
        if ((aux % 10) >=5) {
            value++;
        }// end if
        precision--;
    } // end while
}


Fixed::Fixed(const char *s) {
    value = 0;
    precision = 0;
    int c;
    while ((c = *s++) == ' ' || c == '\t')
        ;
    bool negative;
    if (c == '-') {
        negative = true;
        c = *s++;
    } else {
        negative = false;
    } // end if
    bool decimal = false;
    while (precision < MAX_FIXED_PRECISION) {
        if ('0' <= c && c <= '9') {
            value = value * 10 + (c - '0');
            if (decimal)
                precision++;
        } else if (c == '.') {
            if (decimal)
                break;
            decimal = true;
        } else if (c != ',')
            break;
        c = *s++;
    } // end while
    if (negative)
        value = - value;
    if (value == 0)
        precision = 1;
}


bool operator < (Fixed x, Fixed y) {
    x.equalize_precision(y);
    return x.value < y.value;
}


bool operator < (Fixed x, int y) {
    return x < Fixed(y, 0);
}


bool operator < (int x, Fixed y) {
    return Fixed(x, 0) < y;
}


Fixed operator - (Fixed x) {
    x.value = -x.value;
    return x;
}


Fixed operator * (Fixed x, int y) {
    return x * Fixed(y, 0);
}


Fixed operator * (Fixed x, Fixed y) {
    x.value = x.value * y.value;
    x.precision = x.precision + y.precision;
    return x;
}


Fixed Fixed::operator [] (int p) const {
    _depura("Fixed::operator[]");
    Fixed x(0, p);
    _depura("END Fixed::operator[]");
    return x;
}


Fixed operator - (Fixed x, Fixed y) {
    x.equalize_precision(y);
    x.value = x.value - y.value;
    return x;
}


Fixed operator - (Fixed x, int y) {
    return x - Fixed(y, 0);
}


Fixed operator - (int x, Fixed y) {
    return Fixed(x, 0) - y;
}

