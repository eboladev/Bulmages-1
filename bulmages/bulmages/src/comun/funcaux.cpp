/***************************************************************************
                          funcaux.cpp  -  description
                             -------------------
    begin                : Sun Jan 26 2003
    copyright            : (C) 2003 by Josep Burcion and Tomeu Borrás  
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
#include "funcaux.h"

string extiendecodigo (string cad, unsigned int num1) {
   string cod=cad;
   unsigned int num=num1;
   if (cod.length() < num) {
     string str7 (num-cod.length()+1,'0');
     int pos = cod.find(".",0);
     fprintf(stderr,"Encontrado en la posicion: %d\n",pos);
     if (pos > 0) {
       cod.replace(pos,1,str7);
     }// end if
   }// end if
   fprintf(stderr,"Cadena resultante:%s\n",cod.c_str());
   return(cod);
}

QString extiendecodigo (QString cad, unsigned int num1) {
   QString cod=cad;
   unsigned int num=num1;
   if (cod.length() < num) {
     string str7 (num-cod.length()+1,'0');
     int pos = cod.find(".",0);
     fprintf(stderr,"Encontrado en la posicion: %d\n",pos);
     if (pos > 0) {
       cod.replace(pos,1,str7.c_str());
     }// end if
   }// end if
   fprintf(stderr,"Cadena resultante:%s\n",cod.latin1());
   return(cod);
}


// Redondeo de numeros en punto flotante.
//---------------------------------------
float fround(float n, unsigned d) {
  return floor(n*pow(10., d) + .5) / pow (10., d);
}// end fround

/********************************************************************************/
/* Esta función convierte un numero con decimales a un entero. Usando la regla  */
/* que si el el primer decimal es mayor a 5 se devuelve el entero superior.     */
/********************************************************************************/
int roundI(double valor) {
    int retorno;
    double mayor=floor(valor);
    if ((mayor-valor) > 0.5)  retorno=(int)mayor-1;
    else retorno= (int)mayor;
    return retorno;
}

/********************************************************
 * Esta funcion extiende la fecha pasada como parametro *
 * QString y devuelve la fecha en formato QDate.        *
 ********************************************************/
QDate normalizafecha(QString fechaintro) {
  QDate fecharesult;
  int d, M, y;
  switch(fechaintro.length()) {
    case 4: // fecha tipo ddMM, sin // y sin año.
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


/********************************************************
 * Esta funcion extiende la fecha pasada como parametro *
 * Y devuelve la fecha en formato extendido.            *
 ********************************************************/
/*
string modificafecha(string fechaintro) {
  string fecharesult;
  string sano;
  char cadena[10];
  int longitud = fechaintro.size();
  int ano = QDate::currentDate().year();
  sprintf (cadena,"%d",ano);
  sano = cadena;
  switch(longitud) {
     case 4: // fecha tipo ddMM, sin // y sin año.
        fecharesult = fechaintro.substr(0,2)+"/"+fechaintro.substr(2,2)+"/"+sano;
     break;
     case 5:// fecha tipo dd/MM
        fecharesult = fechaintro+"/"+sano;
     break;
     case 6: // fecha tipo yyMMyy
        fecharesult = fechaintro.substr(0,2)+"/"+fechaintro.substr(2,2)+"/20"+fechaintro.substr(4,2);
     break;
     case 8:
        if(fechaintro.rfind("/",0)==string::npos) {
            // fecha tipo  ddMMyyyy
            fecharesult = fechaintro.substr(0,2)+"/"+fechaintro.substr(2,2)+"/"+fechaintro.substr(4,4);
        } else {
            // o bien tipo dd/MM/yy
            fecharesult = fechaintro.substr(0,5)+"/20"+fechaintro.substr(6,2);
        }// end if
     break;
     case 10: // fecha tipo dd/MM/yyyy
     default:
        fecharesult = fechaintro;
  }// end switch
  return(fecharesult);
}// end modificafecha
*/


/********************************************************
 * Esta funcion extiende la fecha pasada como parametro *
 * Y devuelve la fecha en formato extendido.            *
 ********************************************************/
/*
QString modificafecha(QString fechaintro) {
  QString fecharesult;
  QString sano;
  QString cadena;
  int longitud = fechaintro.length();
  int ano = QDate::currentDate().year();
  cadena.sprintf ("%d",ano);
  sano = cadena;
  switch(longitud) {
     case 4: // fecha tipo ddMM, sin // y sin año.
        fecharesult = fechaintro.mid(0,2)+"/"+fechaintro.mid(2,2)+"/"+sano;
     break;
     case 5:// fecha tipo dd/MM
        fecharesult = fechaintro+"/"+sano;
     break;
     case 6: // fecha tipo yyMMyy
        fecharesult = fechaintro.mid(0,2)+"/"+fechaintro.mid(2,2)+"/20"+fechaintro.mid(4,2);
     break;
     case 8:
        if(fechaintro.find("/",0)==-1) {
            // fecha tipo  ddMMyyyy
            fecharesult = fechaintro.mid(0,2)+"/"+fechaintro.mid(2,2)+"/"+fechaintro.mid(4,4);
        } else {
            // o bien tipo dd/MM/yy
            fecharesult = fechaintro.mid(0,5)+"/20"+fechaintro.mid(6,2);
        }// end if
     break;
     case 10: // fecha tipo dd/MM/yyyy
     default:
        fecharesult = fechaintro;
  }// end switch
  return(fecharesult);
}// end modificafecha

*/
// Dada una fecha en formato de string, y normalizada a la forma dd/mm/yyyy se devuelve un QDate con el valor de la fecha.
//------------------------------------------------------------------------------------------------------------------------
/*
QDate devuelvefecha(string fechaintro) {
	int dia, mes, ano;
	dia = atoi(fechaintro.substr(0,2).c_str());
	mes = atoi(fechaintro.substr(3,2).c_str());
	ano = atoi(fechaintro.substr(6,4).c_str());
// 	fprintf(stderr,"devuelvefecha dia %d, mes %d, ano %d\n", dia, mes, ano);
	return QDate(ano,mes,dia);
}// end devuelvefecha
*/
