/***************************************************************************
 *Copyright (C) 2004 by  Tomeu Borrás Riera                                *
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

#include "importbalance.h"
#include "empresa.h"
#include <qstring.h>

importbalance::importbalance( empresa *emp) : QXmlDefaultHandler() {
   fprintf(stderr,"Inicializador de importainteligente\n");
   empresaactual = emp;
   conexionbase = emp->bdempresa();
   tag="";
   data="";
   paso=0;
}// end importainteligente


bool importbalance::startElement( const QString &a , const QString &b , const QString &c, const QXmlAttributes &d) {
   if (paso ==0 ) {
      return startElement1(a, b, c, d);
   } else {
      return startElement2(a, b, c, d);
   }// end if
}// end startElement

bool importbalance::endElement( const QString &a , const QString &b , const QString &c ) {
   if (paso == 0) {
      return endElement1 (a, b, c);
   } else {
      return endElement2 (a, b, c);
   }// end if
}// end ene Element


bool importbalance::startElement1( const QString& , const QString& , const QString& qName, const QXmlAttributes&) {
    tag=qName;
    QString SQLQuery;
    if (tag == "balance") {      
       SQLQuery.sprintf("INSERT INTO balance (nombrebalance) VALUES ('Elemento importado')\n");
       conexionbase->begin();
       conexionbase->ejecuta(SQLQuery);
       SQLQuery = "SELECT max(idbalance) AS idbalance FROM balance";
       cursor2 *cur = conexionbase->cargacursor(SQLQuery,"unquerymas");
       conexionbase->commit();
       if (!cur->eof()) {
         tvalores["idbalance"]=cur->valor("idbalance");
       }// end if
       delete cur;
    }// end if
    if (tag == "mpatrimonial") {
       SQLQuery.sprintf("INSERT INTO mpatrimonial (idbalance) VALUES (%s)\n", tvalores["idbalance"].ascii());
       conexionbase->begin();
       conexionbase->ejecuta(SQLQuery);
       SQLQuery = "SELECT max(idmpatrimonial) AS idmpatrimonial FROM mpatrimonial";
       cursor2 *cur = conexionbase->cargacursor(SQLQuery,"unquerymas");
       conexionbase->commit();
       if (!cur->eof()) {
         tvalores["idmpatrimonial_nueva"]=cur->valor("idmpatrimonial");
       }// end if
       delete cur; 
    }// end if

    return TRUE;
}// end startElement1


bool importbalance::endElement1( const QString& , const QString& , const QString& qName ) {
   tag=qName;
   QString SQLQuery;   
   if (qName == "balance") {
         SQLQuery.sprintf("UPDATE balance SET nombrebalance='%s' WHERE idbalance=%s\n", tvalores["nombrebalance"].ascii(), tvalores["idbalance"].ascii());
         conexionbase->begin();
         conexionbase->ejecuta(SQLQuery);
         conexionbase->commit();
      }// end balance
      
      if (qName == "mpatrimonial") {
         // En la inserción de masas patrimoniales hay que hacer una reconversión de la clave.
         // Cuando todo haya terminado debemos actualizar el cambo idmpatrimonial de los compmasap para que la cosa funcione.
         identmasasp[tvalores["idmasa"]]=tvalores["idmpatrimonial_nueva"];
         SQLQuery.sprintf("UPDATE mpatrimonial SET descmpatrimonial='%s' WHERE idmpatrimonial=%s\n", tvalores["descmpatrimonial"].ascii(), tvalores["idmpatrimonial_nueva"].ascii());
         conexionbase->begin();
         conexionbase->ejecuta(SQLQuery);
         conexionbase->commit();      

         if (tvalores["orden"] != "") {
            SQLQuery.sprintf("UPDATE mpatrimonial SET orden=%s WHERE idmpatrimonial=%s\n", tvalores["orden"].ascii(), tvalores["idmpatrimonial_nueva"].ascii());
            conexionbase->begin();
            conexionbase->ejecuta(SQLQuery);
            conexionbase->commit();
            tvalores["orden"] = "";
         }// end if

         if (tvalores["tabulacion"] != "") {
            SQLQuery.sprintf("UPDATE mpatrimonial SET tabulacion=%s WHERE idmpatrimonial=%s\n", tvalores["tabulacion"].ascii(), tvalores["idmpatrimonial_nueva"].ascii());
            conexionbase->begin();
            conexionbase->ejecuta(SQLQuery);
            conexionbase->commit();
            tvalores["tabulacion"] = "";
         }// end if
                  
         if (tvalores["opdesc"] != "") {
            SQLQuery.sprintf("UPDATE mpatrimonial SET opdesc=%s WHERE idmpatrimonial=%s\n", tvalores["opdesc"].ascii(), tvalores["idmpatrimonial_nueva"].ascii());
            conexionbase->begin();
            conexionbase->ejecuta(SQLQuery);
            conexionbase->commit();
            tvalores["opdesc"] = "";
         }// end if
      }// end mpatrimonial

      tag="";
      data="";
      return TRUE;
}// endElement1


bool importbalance::startElement2( const QString& , const QString& , const QString &qName , const QXmlAttributes&) {
   tag=qName;
   return TRUE;
}// end startElement2


bool importbalance::endElement2( const QString& , const QString& , const QString &qName  ) {
    tag=qName;
    QString SQLQuery;
// En el segundo paso se hacen las inserciones por el flanco de final en lugar de por el flanco de principio.
// Así nos aseguramos que ya existen los valores de idmpatrimonial y masaperteneciente.
    if (tag == "compmasap") {
       SQLQuery.sprintf("INSERT INTO compmasap (masaperteneciente) VALUES ( %s)\n", identmasasp[tvalores["masaperteneciente"]].ascii());
       conexionbase->begin();
       conexionbase->ejecuta(SQLQuery);
       SQLQuery = "SELECT max(idcompmasap) AS idcompmasap FROM compmasap";
       cursor2 *cur = conexionbase->cargacursor(SQLQuery,"unquerymas");
       conexionbase->commit();
       if (!cur->eof()) {
         tvalores["idcompmasap"]=cur->valor("idcompmasap");
       }// end if
       delete cur; 
    }// end if
    if (tag == "compbalance") {
       SQLQuery.sprintf("INSERT INTO compbalance (idbalance) VALUES ( %s)\n", tvalores["idbalance"].ascii());
       conexionbase->begin();
       conexionbase->ejecuta(SQLQuery);
       SQLQuery = "SELECT max(idcompbalance) AS idcompbalance FROM compbalance";
       cursor2 *cur = conexionbase->cargacursor(SQLQuery,"unquerymas");
       conexionbase->commit();
       if (!cur->eof()) {
         tvalores["idcompbalance"]=cur->valor("idcompbalance");
       }// end if
       delete cur; 
    }// end if    

    
// Como la inserción está hecha, podemos hacer los updates sin miedo a que no exista el registro.
    if (qName == "compmasap") {
         // Actualizamos el idmasapatrimonial del compmasap, que es el que más dolores de cabeza causa.
         if (tvalores["idmpatrimonial"] != "" && tvalores["codigo"] == "") {
            SQLQuery.sprintf("UPDATE compmasap SET idmpatrimonial=%s WHERE idcompmasap=%s\n", identmasasp[tvalores["idmpatrimonial"]].ascii(), tvalores["idcompmasap"].ascii());
            conexionbase->begin();
            conexionbase->ejecuta(SQLQuery);
            conexionbase->commit();         
         }// end if
         if (tvalores["codigo"] != "") {
            SQLQuery.sprintf("UPDATE compmasap SET idcuenta=id_cuenta('%s') WHERE idcompmasap=%s\n", tvalores["codigo"].ascii(), tvalores["idcompmasap"].ascii());
            conexionbase->begin();
            conexionbase->ejecuta(SQLQuery);
            conexionbase->commit();         
         }// end if
    
         SQLQuery.sprintf("UPDATE compmasap SET signo='%s' WHERE idcompmasap=%s\n", tvalores["signo"].ascii(), tvalores["idcompmasap"].ascii());
         conexionbase->begin();
         conexionbase->ejecuta(SQLQuery);
         conexionbase->commit();

         SQLQuery.sprintf("UPDATE compmasap SET nombre='%s' WHERE idcompmasap=%s\n", tvalores["nombre"].ascii(), tvalores["idcompmasap"].ascii());
         conexionbase->begin();
         conexionbase->ejecuta(SQLQuery);
         conexionbase->commit();         
                  
         tvalores["codigo"]="";
         tvalores["idmpatrimonial"]=""; 
      }// end compmasap      
 
    if (qName == "compbalance") {
         // Con los componentes del balance también intervienen las masas patrimoniales.
         SQLQuery.sprintf("UPDATE compbalance SET idmpatrimonial=%s WHERE idcompbalance=%s\n", identmasasp[tvalores["idmpatrimonial"]].ascii(), tvalores["idcompbalance"].ascii());
         conexionbase->begin();
         conexionbase->ejecuta(SQLQuery);
         conexionbase->commit();
         
         SQLQuery.sprintf("UPDATE compbalance SET orden=%s WHERE idcompbalance=%s\n", tvalores["orden"].ascii(), tvalores["idcompbalance"].ascii());
         conexionbase->begin();
         conexionbase->ejecuta(SQLQuery);
         conexionbase->commit();
         
         SQLQuery.sprintf("UPDATE compbalance SET concepto='%s' WHERE idcompbalance=%s\n", tvalores["concepto"].ascii(), tvalores["idcompbalance"].ascii());
         conexionbase->begin();
         conexionbase->ejecuta(SQLQuery);
         conexionbase->commit();

         SQLQuery.sprintf("UPDATE compbalance SET tabulacion=%s WHERE idcompbalance=%s\n", tvalores["tabulacion"].ascii(), tvalores["idcompbalance"].ascii());
         conexionbase->begin();
         conexionbase->ejecuta(SQLQuery);
         conexionbase->commit();         
                  
         tvalores["codigo"]="";
         tvalores["idmpatrimonial"]=""; 
    }// end compbalance  
      
    
    tag="";
    data="";  
   return TRUE;
}// endElement2


bool importbalance::characters( const QString& ch ) {
    data =ch;
    tvalores[tag]=data;
    return TRUE;
}// endElement

