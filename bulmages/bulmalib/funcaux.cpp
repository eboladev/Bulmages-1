/***************************************************************************
                          funcaux.cpp  -  description
                             -------------------
    begin                : Sun Jan 26 2003
    copyright            : (C) 2003 by Josep Burcion and Tomeu Borr�  
    email                : tborras@conetxia.com
 ***************************************************************************/
/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/
/** \file funcaux.cpp
  * Fichero de implementaci� de funciones auxiliares que no es preciso encapsular<BR>
  * <P>En este fichero se implementan todas aquellas funciones que por su brevedad o aislamiento son
  * utilizadas de forma regular en el programa. Definidas en \ref funcaux.h </P>
  * <P>Dichas funciones normalmente son de uso general, por lo que es normal ver este archivo incluido
  * en la pr�tica totalidad de los dem� ficheros</P>
  */

#include "funcaux.h"
#include "configuracion.h"
#include <qstring.h>
#include <QTextEdit>


/// Definimos aqui la variable global g_main para que sea accesible desde esta libreria.
Q3MainWindow *g_main=NULL;

/// Esta funcion permite editar un texto en un QTextEdit y devuelve el texto editado.
QString editaTexto(QString texto) {
                QTextEdit *ed = new QTextEdit(0);
		ed->setFixedSize(450,250);
                ed->setPlainText(texto);
                g_main->setEnabled(FALSE);
                ed->show();
                while(!ed->isHidden())
                    theApp->processEvents();
                g_main->setEnabled(TRUE);
		QString vuelta = ed->toPlainText();
		return vuelta;
}

/** Proteje cadenas de texto pasandoles una sustituci� de codigos especiales de XML
  * \param string cadena que se va a proteger.
  * \return cadena en formato protegido.
  */
QString XMLProtect( const QString& string ) {
    QString s = string;
    s.replace( "&", "&amp;" );
    s.replace( ">", "&gt;" );
    s.replace( "<", "&lt;" );
    s.replace( "\"", "&quot;" );
    s.replace( "\'", "&apos;" );
    return s;
}

/** Extiende un string a un numero de cuenta sustituyendo los '.' por ceros.
  * \param cad Cadena inicial
  * \param num1 Numero de d�itos totales de la cuenta.
  * \return Devuelve un string con el c�igo de cuenta extendido al nmero de d�itos indicado.
  */
string extiendecodigo (string cad, unsigned int num1) {
   string cod=cad;
   unsigned int num=num1;
   if (cod.length() < num) {
     string str7 (num-cod.length()+1,'0');
     int pos = cod.find(".",0);
     if (pos > 0) {
       cod.replace(pos,1,str7);
     }// end if
   }// end if
   return(cod);
}

/** Extiende un string a un numero de cuenta sustituyendo los '.' por ceros.
  * \param cad Cadena inicial
  * \param num1 Numero de d�itos totales de la cuenta.
  * \return Devuelve un QString con la cuenta extendida al nmero de d�itos indicado.
  */
QString extiendecodigo (QString cad, unsigned int num1) {
   QString cod=cad;
   int num=num1;
   if (cod.length() < num) {
     string str7 (num-cod.length()+1,'0');
     int pos = cod.find(".",0);
     if (pos > 0) {
       cod.replace(pos,1,str7.c_str());
     }// end if
   }// end if
   return(cod);
}


/** Redondeo de numeros en punto flotante.
  * \param n Numero a redondear
  * \param d Numero de decimales
  * \return Numero redondeado
  */
float fround(float n, unsigned d) {
  return floor(n*pow(10., d) + .5) / pow (10., d);
}// end fround


/** Esta funci� convierte un numero con decimales a un entero. Usando la regla
  * que si el el primer decimal es mayor o igual  a 5 se devuelve el entero superior.
  * \param valor Numero a convertir
  */
int roundI(double valor) {
    int retorno;
    double mayor=floor(valor);
    if ((mayor-valor) >= 0.5)  retorno=(int)mayor-1;
    else retorno= (int)mayor;
    return retorno;
}

/** Procesa el string pasado como par�etro y devuelve una estructura del tipo QDate
 * Esta funcion extiende la fecha pasada como parametro 
 * QString y devuelve la fecha en formato QDate.
 * \param fechaintro string con la fecha a ser normalizada.
 */
QDate normalizafecha(QString fechaintro) {
  QDate fecharesult;
  int d, M, y;
  switch(fechaintro.length()) {
    case 4: // fecha tipo ddMM, sin // y sin a�.
      d = fechaintro.mid(0,2).toInt();
      M = fechaintro.mid(2,2).toInt();
      y = QDate::currentDate().year();
      break;
    case 5:// fecha tipo dd/MM
      d = fechaintro.mid(0,2).toInt();
      M = fechaintro.mid(3,2).toInt();
      y = QDate::currentDate().year();
    break;
    case 6: // fecha tipo ddMMyy
      d = fechaintro.mid(0,2).toInt();
      M = fechaintro.mid(2,2).toInt();
      y = 2000 + fechaintro.mid(4,2).toInt();
     break;
     case 8:
        if(fechaintro.contains("/",TRUE) || fechaintro.contains("-",TRUE)) {
            // fecha tipo  dd/MM/yy o dd-MM-yy
            d = fechaintro.mid(0,2).toInt();
            M = fechaintro.mid(3,2).toInt();
            y = 2000 + fechaintro.mid(6,2).toInt();
        } else {
            // o bien tipo ddMMyyyy
            d = fechaintro.mid(0,2).toInt();
            M = fechaintro.mid(2,2).toInt();
            y = fechaintro.mid(4,4).toInt();
        }// end if
     break;
     case 10: // fecha tipo dd/MM/yyyy
        d = fechaintro.mid(0,2).toInt();
        M = fechaintro.mid(3,2).toInt();
        y = fechaintro.mid(6,4).toInt();
    break;    
    default:
        d = QDate::currentDate().day();
        M = QDate::currentDate().month();
        y = QDate::currentDate().year();
  }// end switch
  if (!fecharesult.setYMD(y,M,d)) fecharesult=QDate::currentDate();
  return(fecharesult);
}// end normalizafecha


/** Esta funci� ajusta el c�igo pasado al nmero de digitos especificado.
    Para ello bsca los ceros intermedios y los amplia hasta que el numero de caracteres sea el deseado.
    Lo hace a partir del quinto d�ito por defecto. Aunque este parametro deber� ser configurable.
  */
QString ajustacodigo (QString cad, unsigned int num1) {
   QString cod=cad;
   unsigned int longcad = cad.length();
   if (longcad > 4) { 
	if (longcad < num1) {
	string str7 (num1 -longcad,'0');
	cod = cad.left(4);
	cod += QString(str7.c_str());
	cod += cad.right(longcad-4);
	}// end if
	if (longcad > num1) {
	cod = cad.left(4);
	cod += cad.right(num1-4);
	}// end if
   }// end if
   return(cod);
}// end ajustacodigo


void reemplazaarchivo (QString archivo, QString texto1, QString texto2, QString archivo2) {
	QString cadena = " sed -e \"s&"+texto1+"&"+texto2+"&g\"  "+archivo+" > "+archivo2+"";
	system (cadena.toAscii().data());
}



/// En la impresion de documentos con trml2pdf esta funcion hace casi todo el trabajo de la invocacion de trml2pdf
/// Para evitar trabajo duplicado.
void generaPDF(const QString arch) {
	QString cadsys;
#ifdef WINDOWS
	cadsys = "cd " + confpr->valor(CONF_DIR_TMP)+" & ";
	cadsys = cadsys + confpr->valor(CONF_PYTHON)+" "+confpr->valor(CONF_PROGDATA)+"trml2pdf\\trml2pdf.py "+confpr->valor(CONF_DIR_TMP)+arch+".rml > "+confpr->valor(CONF_DIR_TMP)+arch+".pdf";
    cadsys = cadsys + " & ";
	cadsys = cadsys + confpr->valor(CONF_FLIP)+" -u "+confpr->valor(CONF_DIR_TMP)+arch+".pdf";
	_depura(cadsys,0);
	system (cadsys.ascii());
	_depura(cadsys,0);
#else
    cadsys = "trml2pdf.py "+confpr->valor(CONF_DIR_TMP)+arch+".rml > "+confpr->valor(CONF_DIR_TMP)+arch+".pdf";
    system(cadsys.ascii());
#endif
}


void invocaPDF(const QString arch) {
    generaPDF(arch);
    QString cadsys = confpr->valor(CONF_PDF)+" "+confpr->valor(CONF_DIR_TMP)+arch+".pdf";
    system(cadsys.ascii());
}

/// De momento no se usa, pero sirve para enviar documentos por e-mail a un destinatario.
void mailsendPDF(const QString arch, const QString to, const QString subject, const QString message) {
//    QString cadsys = confpr->valor(CONF_PDF)+" "+confpr->valor(CONF_DIR_TMP)+arch+".pdf";
	QString cadsys = "mailsend -h mail.conetxia.com -d conetxia.com -f bulmages@conetxia.com -t tborras@conetxia.com -sub hola -m hola";
    system(cadsys.ascii());
}
