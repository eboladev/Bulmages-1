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
#ifndef Sepa20022TEXTCODEC_H
#define Sepa20022TEXTCODEC_H

#include <QtCore/QTextCodec>
#include <QtCore/QRegExp>

/**
	@author Xavi Drudis Ferran <xdrudis@tinet.cat>
*/
class Sepa20022TextCodec : public QTextCodec
{
private:

   QTextCodec *latin1;
   QRegExp invalid; 

public:
    Sepa20022TextCodec();

    ~Sepa20022TextCodec();

virtual QByteArray name () const ;

virtual QList<QByteArray> aliases () const  ;

virtual int mibEnum () const;

virtual QString convertToUnicode ( const char * chars, int len, ConverterState * state ) const ;

virtual QByteArray convertFromUnicode ( const QChar * input, int number, ConverterState * state ) const  ;

};

#endif
