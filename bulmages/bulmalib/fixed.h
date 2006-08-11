/***************************************************************************
 *   Copyright (C) 2004 by Tomeu Borras Riera                              *
 *   tborras@conetxia.com                                                  *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/

#ifndef H__FIXED
#define H__FIXED

#include <QString>


const int MAX_FIXED_PRECISION = 12;
const int MAX_FIXED_LENGTH = 25;

typedef long long int Fixed_numerator;

inline long long int integer(Fixed_numerator x) {
    return (int) x;
}


class Fixed {

private:

public:
    Fixed_numerator value;
    int precision;
    void equalize_precision(Fixed &);
    void setprecision(int);

public:
    Fixed(int x, int p) {
        value = x;
        precision = p;
    }
    Fixed(const char *);
    Fixed() {
        value = 0;
        precision = 1;
    };
    Fixed operator [] (int) const;
    Fixed operator = (Fixed);
    Fixed operator = (int);
    QString toQString();
    friend Fixed operator + (Fixed, Fixed);
    friend Fixed operator - (Fixed, Fixed);
    friend Fixed operator * (Fixed, Fixed);
    friend Fixed operator / (Fixed, Fixed);
    friend Fixed operator - (Fixed);
    friend bool operator == (Fixed, Fixed);
    friend bool operator < (Fixed, Fixed);
    friend Fixed operator - (int, Fixed);
    friend Fixed operator / (int, Fixed);
    friend bool operator == (int, Fixed);
    friend bool operator < (int, Fixed);
    friend Fixed operator + (Fixed, int);
    friend Fixed operator - (Fixed, int);
    friend Fixed operator * (Fixed, int);
    friend Fixed operator / (Fixed, int);
    friend bool operator == (Fixed, int);
    friend bool operator < (Fixed, int);
    enum {ALIGN = 1, COMMAS = 2, DECIMAL = 4};
};


inline bool operator != (Fixed x, Fixed y) {
    return !(x == y);
}

inline bool operator != (Fixed x, int y) {
    return !(x == y);
}

inline bool operator != (int x, Fixed y) {
    return !(x == y);
}

inline bool operator >= (Fixed x, Fixed y) {
    return !(x < y);
}

inline bool operator >= (Fixed x, int y) {
    return !(x < y);
}

inline bool operator >= (int x, Fixed y) {
    return !(x < y);
}

inline bool operator > (Fixed x, Fixed y) {
    return y < x;
}

inline bool operator > (Fixed x, int y) {
    return y < x;
}

inline bool operator > (int x, Fixed y) {
    return y < x;
}

inline bool operator <= (Fixed x, Fixed y) {
    return !(y < x);
}

inline bool operator <= (Fixed x, int y) {
    return !(y < x);
}

inline bool operator <= (int x, Fixed y) {
    return !(y < x);
}

#endif

