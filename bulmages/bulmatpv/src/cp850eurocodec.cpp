/****************************************************************************
**
** Copyright (C) 1992-2008 Trolltech ASA. All rights reserved.
**
** This file is part of the QtCore module of the Qt Toolkit.
**
** This file may be used under the terms of the GNU General Public
** License versions 2.0 or 3.0 as published by the Free Software
** Foundation and appearing in the files LICENSE.GPL2 and LICENSE.GPL3
** included in the packaging of this file.  Alternatively you may (at
** your option) use any later version of the GNU General Public
** License if such license has been publicly approved by Trolltech ASA
** (or its successors, if any) and the KDE Free Qt Foundation. In
** addition, as a special exception, Trolltech gives you certain
** additional rights. These rights are described in the Trolltech GPL
** Exception version 1.2, which can be found at
** http://www.trolltech.com/products/qt/gplexception/ and in the file
** GPL_EXCEPTION.txt in this package.
**
** Please review the following information to ensure GNU General
** Public Licensing requirements will be met:
** http://trolltech.com/products/qt/licenses/licensing/opensource/. If
** you are unsure which license is appropriate for your use, please
** review the following information:
** http://trolltech.com/products/qt/licenses/licensing/licensingoverview
** or contact the sales department at sales@trolltech.com.
**
** In addition, as a special exception, Trolltech, as the sole
** copyright holder for Qt Designer, grants users of the Qt/Eclipse
** Integration plug-in the right for the Qt/Eclipse Integration to
** link to functionality provided by Qt Designer and its related
** libraries.
**
** This file is provided "AS IS" with NO WARRANTY OF ANY KIND,
** INCLUDING THE WARRANTIES OF DESIGN, MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE. Trolltech reserves all rights not expressly
** granted herein.
**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
**
****************************************************************************/

#include "cp850eurocodec.h"
#include <qlist.h>

#ifndef QT_NO_TEXTCODEC

QT_BEGIN_NAMESPACE

#define LAST_MIB 2004

static const struct {
    const char *mime;
    const char *aliases[7];
    int mib;
    quint16 values[128];
} unicodevalues[QCP850EuroTextCodec::numSimpleCodecs] = {
    // from RFC 1489, ftp://ftp.isi.edu/in-notes/rfc1489.txt
    

    // next bits generated again from tables on the Unicode 3.0 CD.

    // $ for a in CP* ; do (awk '/^0x[89ABCDEF]/{ print $1, $2 }' < $a) | sort | sed -e 's/#UNDEF.*$/0xFFFD/' | cut -c6- | paste '-d ' - - - - - - - - | sed -e 's/ /, /g' -e 's/$/,/' -e '$ s/,$/} },/' -e '1 s/^/{ /' > ~/tmp/$a ; done

    { "IBM850EURO", { "CP850EURO", "csPC850MultilingualEuro", 0 }, 6969,
      { 0x00C7, 0x00FC, 0x00E9, 0x00E2, 0x00E4, 0x00E0, 0x00E5, 0x00E7,
        0x00EA, 0x00EB, 0x00E8, 0x00EF, 0x00EE, 0x00EC, 0x00C4, 0x00C5,
        0x00C9, 0x00E6, 0x00C6, 0x00F4, 0x00F6, 0x00F2, 0x00FB, 0x00F9,
        0x00FF, 0x00D6, 0x00DC, 0x00F8, 0x00A3, 0x00D8, 0x00D7, 0x0192,
        0x00E1, 0x00ED, 0x00F3, 0x00FA, 0x00F1, 0x00D1, 0x00AA, 0x00BA,
        0x00BF, 0x00AE, 0x00AC, 0x00BD, 0x00BC, 0x00A1, 0x00AB, 0x00BB,
        0x2591, 0x2592, 0x2593, 0x2502, 0x2524, 0x00C1, 0x00C2, 0x00C0,
        0x00A9, 0x2563, 0x2551, 0x2557, 0x255D, 0x00A2, 0x00A5, 0x2510,
        0x2514, 0x2534, 0x252C, 0x251C, 0x2500, 0x253C, 0x00E3, 0x00C3,
        0x255A, 0x2554, 0x2569, 0x2566, 0x2560, 0x2550, 0x256C, 0x00A4,
        0x00F0, 0x00D0, 0x00CA, 0x00CB, 0x00C8, 0x0131, 0x00CD, 0x00CE,
        0x00CF, 0x2518, 0x250C, 0x2588, 0x2584, 0x00A6, 0x00CC, 0x2580,
        0x00D3, 0x00DF, 0x00D4, 0x00D2, 0x00F5, 0x00D5, 0x00B5, 0x00FE,
        0x00DE, 0x00DA, 0x00DB, 0x00D9, 0x00FD, 0x00DD, 0x00AF, 0x00B4,
        0x00AD, 0x00B1, 0x2017, 0x00BE, 0x00B6, 0x00A7, 0x00F7, 0x00B8,
        0x00B0, 0x00A8, 0x00B7, 0x00B9, 0x00B3, 0x00B2, 0x25A0, 0x00A0} }
    // if you add more chacater sets at the end, change LAST_MIB above
};

QCP850EuroTextCodec::QCP850EuroTextCodec(int i) : forwardIndex(i), reverseMap(0)
{
}


QCP850EuroTextCodec::~QCP850EuroTextCodec()
{
    delete reverseMap;
}

static QByteArray *buildReverseMap(int forwardIndex)
{
    QByteArray *map = new QByteArray();
    int m = 0;
    int i = 0;
    while(i < 128) {
        if (unicodevalues[forwardIndex].values[i] > m &&
             unicodevalues[forwardIndex].values[i] < 0xfffd)
            m = unicodevalues[forwardIndex].values[i];
        i++;
    }
    m++;
    map->resize(m);
    for(i = 0; i < 128 && i < m; i++)
        (*map)[i] = (char)i;
    for(;i < m; i++)
        (*map)[i] = 0;
    for(i=128; i<256; i++) {
        int u = unicodevalues[forwardIndex].values[i-128];
        if (u < m)
            (*map)[u] = (char)(unsigned char)(i);
    }
    return map;
}

QString QCP850EuroTextCodec::convertToUnicode(const char* chars, int len, ConverterState *) const
{
    if (len <= 0 || chars == 0)
        return QString();

    const unsigned char * c = (const unsigned char *)chars;

    QString r;
    r.resize(len);
    QChar* uc = r.data();

    for (int i = 0; i < len; i++) {
        if (c[i] > 127)
            uc[i] = unicodevalues[forwardIndex].values[c[i]-128];
        else
            uc[i] = QLatin1Char(c[i]);
    }
    return r;
}

QByteArray QCP850EuroTextCodec::convertFromUnicode(const QChar *in, int length, ConverterState *state) const
{
    const char replacement = (state && state->flags & ConvertInvalidToNull) ? 0 : '?';
    int invalid = 0;

    if (!reverseMap){
        QByteArray *tmp = buildReverseMap(this->forwardIndex);
        if (!reverseMap.testAndSetOrdered(0, tmp))
            delete tmp;
    }

    QByteArray r;
    r.resize(length);
    int i = length;
    int u;
    const QChar* ucp = in;
    unsigned char* rp = (unsigned char *)r.data();
    const unsigned char* rmp = (const unsigned char *)reverseMap->data();
    int rmsize = (int) reverseMap->size();
    while(i--)
    {
        u = ucp->unicode();
        if (u < 128) {
            *rp = (char)u;
        } else {
		
            *rp = ((u < rmsize) ? (*(rmp+u)) : 0xD5);
	    if(u == 0x20ac)
		*rp = 0xD5;
            if (*rp == 0) {
                *rp = replacement;
                ++invalid;
            }
        }
        rp++;
        ucp++;
    }

    if (state) {
        state->invalidChars += invalid;
    }
    return r;
}

QByteArray QCP850EuroTextCodec::name() const
{
    return unicodevalues[forwardIndex].mime;
}

QList<QByteArray> QCP850EuroTextCodec::aliases() const
{
    QList<QByteArray> list;
    const char * const*a = unicodevalues[forwardIndex].aliases;
    while (*a) {
        list << *a;
        ++a;
    }
    return list;
}

int QCP850EuroTextCodec::mibEnum() const
{
    return unicodevalues[forwardIndex].mib;
}

QT_END_NAMESPACE

#endif // QT_NO_TEXTCODEC
