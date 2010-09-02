//
// C++ Implementation: q19textcodec
//
// Description: 
//
//
// Author: Xavi Drudis Ferran <xdrudis@tinet.cat>, (C) 2009
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "q19textcodec.h"
#include "taula.h"

Q19TextCodec::Q19TextCodec()
 : QTextCodec()
{
  latin1 = QTextCodec::codecForName("ISO-8859-1");
  invalid = QRegExp("[^\x00-\x60\x7b-\x7f\xa5]");
}


Q19TextCodec::~Q19TextCodec()
{
}

QByteArray Q19TextCodec::name () const {
   return QByteArray("Q19") ;
}

QList<QByteArray> Q19TextCodec::aliases () const  {
   return QList<QByteArray>() << QByteArray("Q19") ;
}

int Q19TextCodec::mibEnum () const {
 return 1; // altres, no registrat a  IANA   
}

QString Q19TextCodec::convertToUnicode ( const char * chars, int len, ConverterState * state ) const {
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

QByteArray Q19TextCodec::convertFromUnicode ( const QChar * input, 
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
