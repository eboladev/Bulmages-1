//=============================== ADD.CPP      ===============================

#include "fixed.h"

#include "stdio.h"
#include <qstring.h>


Fixed operator + (Fixed x, Fixed y) {
    x.equalize_precision(y);
    x.value = x.value + y.value;
    return x;
}

//=============================== ADDFI.CPP    ===============================


Fixed operator + (Fixed x, int y) {
    return x + Fixed(y,0);
}

//=============================== ASSIGN.CPP   ===============================


Fixed Fixed::operator = (Fixed x) {
    /*
     value =
       precision > x.precision ?
         x.value * SCALE.x[precision - x.precision] :
       precision < x.precision ?
         x.value / SCALE.x[x.precision - precision] :
       x.value;
    */
    value = x.value;
    precision = x.precision;
    return *this;
}

//=============================== ASSIGNFI.CPP ===============================


Fixed Fixed::operator = (int x) {
    value = x;
    precision = 0;
    return *this;
}

//=============================== DIVIDE.CPP   ===============================


Fixed operator / (Fixed x, Fixed y) {
    //  x.value = (x.value * Fixed::SCALE.x[y.precision]) / y.value;
    x.value = x.value / y.value;
    x.precision += y.precision;
    return x;
}

//=============================== DIVIDEFI.CPP ===============================


Fixed operator / (Fixed x, int y) {
    return x / Fixed(y,0);
}


//=============================== DIVIDEIF.CPP ===============================


Fixed operator / (int x, Fixed y) {
    return Fixed(x,0) / y;
}

/*
QString Fixed::toQString() {
	QString a;
	a=QString(edit());
//	a=a.left(a.length()-precision)+"."+a.right(precision);
	return a;
}
*/

QString Fixed::toQString() {
    setprecision(2);
    int options=COMMAS;
    fprintf(stderr,"Fixed::edit(%d, %d)\n", value, precision);
    Fixed_numerator x = value;
    bool negative;
    if (x < 0)  {
        x = -x;
        // prevent buffer overflow if result is still negative
        if (x < 0)
            x = x - 1;
        negative = true;
    }  else
        negative = false;
    Fixed_numerator n = 0;
    Fixed_numerator units = 0;
    unsigned char buffer[MAX_FIXED_LENGTH + MAX_FIXED_PRECISION];
    for (unsigned int i =0; i<=sizeof(buffer); i++)
        buffer[i]=0;
    do  {
        if (n == precision) {
            if (n > 0 || options & DECIMAL)
                buffer[sizeof(buffer) - ++n] = '.';
            units = n;
        }  // else if (options & COMMAS && n > precision && (n - units) % 4 == 3)
        // buffer[sizeof(buffer) - ++n] = ',';
        Fixed_numerator y;
        y = (Fixed_numerator)x / 10;
        buffer[sizeof(buffer) - ++n] = integer(x - y*10) + '0';
        x = y;
        fprintf(stderr," x: %d, precision: %d\n",x, precision);
    } while (n <= precision || x != 0);
    if (negative)
        buffer[sizeof(buffer) - ++n] = '-';
    if (options & ALIGN) {
        while (n - units < MAX_FIXED_LENGTH-2)
            buffer[sizeof(buffer) - ++n] = ' ';
    }
    fprintf(stderr,"fin Fixed::edit %s\n", (const char *) buffer + sizeof(buffer) - n );
    QString a((const char *) buffer + sizeof(buffer) - n);
    return a;
}
//=============================== EDIT.CPP     ===============================

//=============================== EQL.CPP      ===============================


bool operator == (Fixed x, Fixed y) {
    x.equalize_precision(y);
    return x.value == y.value;
}

//=============================== EQLFI.CPP    ===============================


bool operator == (Fixed x, int y) {
    return x == Fixed(y,0);
}


//=============================== EQLIF.CPP    ===============================


bool operator == (int x, Fixed y) {
    return Fixed(x,0) == y;
}


//=============================== EQUALIZE.CPP ===============================


void Fixed::equalize_precision(Fixed &x) {
    fprintf(stderr,"Ecualizar precisión\n");
    while (precision < x.precision)   {
        value *= 10; //value * SCALE.x[x.precision - precision];
        precision ++;
    }

    while (x.precision < precision)  {
        x.value *= 10 ; //x.value * SCALE.x[precision - x.precision];
        x.precision ++;
    }// end if
    fprintf(stderr,"Fin de ecualizar precisión\n");
}


void Fixed::setprecision(int prec) {
    while (precision < prec) {
        value *= 10;
        precision ++;
    }
    while (prec < precision) {
        Fixed_numerator aux;
        aux = value;
        // No incorpora redondeos
        //		value /= 10;
        // Vamos a incorporar redondeos
        value = (Fixed_numerator) (value / 10);
        if ((aux % 10) >=5) {
            value++;
	}// end if
        precision--;
    }
}// end setprecision


//=============================== FIXEDS.CPP   ===============================


Fixed::Fixed(const char *s) {
    value = 0;
    precision = 0;
    int c;
    while ((c = *s++) == ' ' || c == '\t')
        ;
    bool negative;
    if (c == '-')    {
        negative = true;
        c = *s++;
    }  else
        negative = false;
    bool decimal = false;
    while (precision < MAX_FIXED_PRECISION)   {
        if ('0' <= c && c <= '9')   {
            value = value * 10 + (c - '0');
            if (decimal)
                precision++;
        } else if (c == '.')  {
            if (decimal)
                break;
            decimal = true;
        }  else if (c != ',')
            break;
        c = *s++;
    }
    if (negative)
        value = - value;
    if (value==0)
        precision=1;

    fprintf(stderr,"Fixed: Valor: %d, Precision %d\n", value, precision);
}

//=============================== LSS.CPP      ===============================

bool operator < (Fixed x, Fixed y) {
    x.equalize_precision(y);
    return x.value < y.value;
}

//=============================== LSSFI.CPP    ===============================


bool operator < (Fixed x, int y) {
    return x < Fixed(y,0);
}


//=============================== LSSIF.CPP    ===============================


bool operator < (int x, Fixed y) {
    return Fixed(x,0) < y;
}


//=============================== MAKEFILE     ===============================


//=============================== MINUS.CPP    ===============================

Fixed operator - (Fixed x) {
    x.value = -x.value;
    return x;
}

//=============================== MULTIPFI.CPP ===============================

Fixed operator * (Fixed x, int y) {
    return x * Fixed(y,0);
}


//=============================== MULTIPLY.CPP ===============================

Fixed operator * (Fixed x, Fixed y) {
    x.value = x.value * y.value;
    x.precision = x.precision + y.precision;
    return x;
}

//=============================== OBJECTS      ===============================


//=============================== PRECISIO.CPP ===============================


Fixed Fixed::operator [] (int p) const {
    fprintf(stderr,"Fixed::operator[]\n");
    Fixed x(0, p);
    /*
    x.value =
      p < precision ?
        value / SCALE.x[precision - p] :
      p > precision ?
        value * SCALE.x[p - precision] :
      value;
    */
    fprintf(stderr,"END Fixed::operator[]\n");
    return x;
}



//=============================== SUBTRACT.CPP ===============================


Fixed operator - (Fixed x, Fixed y) {
    x.equalize_precision(y);
    x.value = x.value - y.value;
    return x;
}

//=============================== SUBTRAFI.CPP ===============================


Fixed operator - (Fixed x, int y) {
    return x - Fixed(y,0);
}


//=============================== SUBTRAIF.CPP ===============================

Fixed operator - (int x, Fixed y) {
    return Fixed(x,0) - y;
}


//=============================== TEST.CPP     ===============================
/*
#include <stdio.h>
#include "Fixed.h"
 
void main(int argc, char **argv)
{
  int i;
  Fixed a(argv[1]);
  Fixed b(argv[2]);
  printf("a = %s\n", a.edit());
  printf("b = %s\n", b.edit());
  printf("a+b = %s\n", (a+b).edit());
  printf("a-b = %s\n", (a-b).edit());
  printf("a*b = %s\n", (a*b).edit());
  printf("-a = %s\n", (-a).edit());
  printf("a[4] = %s\n", a[4].edit());
  if (b != 0)
    printf("a/b = %s\n", (a/b).edit());
  printf("a.whole() = %d\n", a.whole());
  printf("is a == b? %s\n", a == b ? "yes" : "no");
  printf("is a != b? %s\n", a != b ? "yes" : "no");
  printf("is a <  b? %s\n", a <  b ? "yes" : "no");
  printf("is a <= b? %s\n", a <= b ? "yes" : "no");
  printf("is a >  b? %s\n", a >  b ? "yes" : "no");
  printf("is a >= b? %s\n", a >= b ? "yes" : "no");
}
 
*/
//=============================== WHOLE.CPP    ===============================

