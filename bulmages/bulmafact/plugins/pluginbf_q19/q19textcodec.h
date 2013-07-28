//
// C++ Interface: q19textcodec
//
// Description: 
//
//
// Author: Xavi Drudis Ferran <xdrudis@tinet.cat>, (C) 2009
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef Q19TEXTCODEC_H
#define Q19TEXTCODEC_H

#include <QtCore/QTextCodec>
#include <QtCore/QRegExp>

/**
	@author Xavi Drudis Ferran <xdrudis@tinet.cat>
*/
class Q19TextCodec : public QTextCodec
{
private:

   QTextCodec *latin1;
   QRegExp invalid; 

public:
    Q19TextCodec();

    ~Q19TextCodec();

virtual QByteArray name () const ;

virtual QList<QByteArray> aliases () const  ;

virtual int mibEnum () const;

virtual QString convertToUnicode ( const char * chars, int len, ConverterState * state ) const ;

virtual QByteArray convertFromUnicode ( const QChar * input, int number, ConverterState * state ) const  ;

};

#endif
