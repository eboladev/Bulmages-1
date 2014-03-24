//
// C++ Implementation: sepa20022textcodec
//
// Description: 
//
//
// Author: Xavi Drudis Ferran <xdrudis@tinet.cat>, (C) 2009
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "sepa20022textcodec.h"
#include "taula.h"

Sepa20022TextCodec::Sepa20022TextCodec()
 : QTextCodec()
{
  latin1 = QTextCodec::codecForName("ISO-8859-1");
  invalid = QRegExp("[^\x00-\x60\x7b-\x7f\xa5]");
}


Sepa20022TextCodec::~Sepa20022TextCodec()
{
}

QByteArray Sepa20022TextCodec::name () const {
   return QByteArray("Sepa20022") ;
}

QList<QByteArray> Sepa20022TextCodec::aliases () const  {
   return QList<QByteArray>() << QByteArray("Sepa20022") ;
}

int Sepa20022TextCodec::mibEnum () const {
 return 1; // altres, no registrat a  IANA   
}

QString Sepa20022TextCodec::convertToUnicode ( const char * chars, int len, ConverterState * state ) const {
    QString resultat = latin1->toUnicode(chars,len,state);
    if ((!resultat.isNull()) && state )
     {
         state->invalidChars += resultat.count(invalid);
         if ( state->flags.testFlag(ConvertInvalidToNull) && state->invalidChars>0) {
           resultat = (QString)"";
         }
     }
    return resultat;
}

QByteArray Sepa20022TextCodec::convertFromUnicode ( const QChar * input, 
     int number, ConverterState * state ) const  {
   QByteArray resultat;
   resultat.resize(number);
   for(int i=0; i < number; i++) {
      char q19='?';
      bool error = true; 
      if (input[i].unicode() < 256 ) {
           q19=taula[input[i].unicode()];
           error = ((q19=='?') && (input[i]!='?'));
      }
      if (state && error )  {
         state->invalidChars++;
      } 
      resultat[i]=q19;
   }
   if (state) {
      if ( state->flags.testFlag(ConvertInvalidToNull) && state->invalidChars>0) {
         resultat = NULL;
      }
      state->remainingChars -= number;
   }
   return resultat;
}
