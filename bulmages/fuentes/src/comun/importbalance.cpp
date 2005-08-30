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
 /** \file importbalance.cpp
   * Contiene la implementación de la clase \ref importbalance y sus métodos.
   * \author Tomeu Borrás Riera
   */
#include "importbalance.h"
#include "empresa.h"
#include <qstring.h>

/** Constructor de la clase que inicializa  la empresa actual
  * La base de datos
  * Establece que de momento no se ha visitado ningn tag 
  * Establece que no hay ningn dato recogido del XML
  * Inidca que el paso que vamos a procesar es el 0
  */
importbalance::importbalance( empresa *emp) : QXmlDefaultHandler() {
   empresaactual = emp;
   conexionbase = emp->bdempresa();
   m_tag="";
   m_data="";
   m_paso=0;
}// end importainteligente


/** Dependiendo del paso en que nos encontremos hace la llamada a una funcio u a otra
  */
bool importbalance::startElement( const QString &a , const QString &b , const QString &c, const QXmlAttributes &d) {
   if (m_paso ==0 ) {
      return startElement1(a, b, c, d);
   } else {
      return startElement2(a, b, c, d);
   }// end if
}// end startElement

/** Dependiendo del paso en que nos encontremos hace la llamada a una función u a otra
  */
bool importbalance::endElement( const QString &a , const QString &b , const QString &c ) {
   if (m_paso == 0) {
      return endElement1 (a, b, c);
   } else {
      return endElement2 (a, b, c);
   }// end if
}// end endElement


/** En el primer paso de la importación se hace la inserción de todos los tags balance y mpatrimonial
  * También se guarda en m_tvalores los identificadores obtenidos puesto que posteriormente seran utilizados
  * Además se asigna la variable global m_tag con el nombre del tag para poder hacer el almacenamiento de datos en 
  * el caso de que sea un tag secundario.
  * \param qName el nombre del tag que se ha encontrado
  * \return TRUE para no detener la ejecución del parser en caso de error.
  */
bool importbalance::startElement1( const QString& , const QString& , const QString& qName, const QXmlAttributes&) {
    m_tag=qName;
    QString SQLQuery;
    if (m_tag == "balance") {      
       SQLQuery.sprintf("INSERT INTO balance (nombrebalance) VALUES ('Elemento importado')\n");
       conexionbase->begin();
       conexionbase->ejecuta(SQLQuery);
       SQLQuery = "SELECT max(idbalance) AS idbalance FROM balance";
       cursor2 *cur = conexionbase->cargacursor(SQLQuery,"unquerymas");
       conexionbase->commit();
       if (!cur->eof()) {
         m_tvalores["idbalance"]=cur->valor("idbalance");
       }// end if
       delete cur;
    }// end if
    if (m_tag == "mpatrimonial") {
       SQLQuery.sprintf("INSERT INTO mpatrimonial (idbalance) VALUES (%s)\n", conexionbase->sanearCadena(m_tvalores["idbalance"]).ascii());
       conexionbase->begin();
       conexionbase->ejecuta(SQLQuery);
       SQLQuery = "SELECT max(idmpatrimonial) AS idmpatrimonial FROM mpatrimonial";
       cursor2 *cur = conexionbase->cargacursor(SQLQuery,"unquerymas");
       conexionbase->commit();
       if (!cur->eof()) {
         m_tvalores["idmpatrimonial_nueva"]=cur->valor("idmpatrimonial");
       }// end if
       delete cur; 
    }// end if
    return TRUE;
}// end startElement1

/** En el primer paso al encontrar un tag de cierre o bien es el de balance o bien es el de mpatrimonial
  * Lo que hacemos en ambos casos es hacer un update de la información puesto que toda la información que precisamos
  * ya ha sido extraida del archivo XML.
  * Al finalizar el método establecemos el campo m_tag a "" para que no se asignen posibles transfugas a este valor
  * el mapa m_identmasasp guarda la correlación entre identificadores de masa para que las que son valores argumentados en el paso 1
  * puedan coincidir. (Es el problema de los identificadores autonuméricos).
  * \param qName El nombre del tag cerrado
  * \return TRUE para no detener la ejecución del parser SAX
  */
bool importbalance::endElement1( const QString& , const QString& , const QString& qName ) {
   m_tag=qName;
   QString SQLQuery;   
   if (qName == "balance") {
         SQLQuery.sprintf("UPDATE balance SET nombrebalance='%s' WHERE idbalance=%s\n", m_tvalores["nombrebalance"].ascii(), m_tvalores["idbalance"].ascii());
         conexionbase->begin();
         conexionbase->ejecuta(SQLQuery);
         conexionbase->commit();
      }// end balance
      if (qName == "mpatrimonial") {
         /// En la inserción de masas patrimoniales hay que hacer una reconversión de la clave.
         /// Cuando todo haya terminado debemos actualizar el cambo idmpatrimonial de los compmasap para que la cosa funcione.
         m_identmasasp[m_tvalores["idmasa"]]=m_tvalores["idmpatrimonial_nueva"];
         SQLQuery.sprintf("UPDATE mpatrimonial SET descmpatrimonial='%s' WHERE idmpatrimonial=%s\n", m_tvalores["descmpatrimonial"].ascii(), m_tvalores["idmpatrimonial_nueva"].ascii());
         conexionbase->begin();
         conexionbase->ejecuta(SQLQuery);
         conexionbase->commit();      
         if (m_tvalores["orden"] != "") {
            SQLQuery.sprintf("UPDATE mpatrimonial SET orden=%s WHERE idmpatrimonial=%s\n", m_tvalores["orden"].ascii(), m_tvalores["idmpatrimonial_nueva"].ascii());
            conexionbase->begin();
            conexionbase->ejecuta(SQLQuery);
            conexionbase->commit();
            m_tvalores["orden"] = "";
         }// end if
         if (m_tvalores["tabulacion"] != "") {
            SQLQuery.sprintf("UPDATE mpatrimonial SET tabulacion=%s WHERE idmpatrimonial=%s\n", m_tvalores["tabulacion"].ascii(), m_tvalores["idmpatrimonial_nueva"].ascii());
            conexionbase->begin();
            conexionbase->ejecuta(SQLQuery);
            conexionbase->commit();
            m_tvalores["tabulacion"] = "";
         }// end if
         if (m_tvalores["opdesc"] != "") {
            SQLQuery.sprintf("UPDATE mpatrimonial SET opdesc=%s WHERE idmpatrimonial=%s\n", m_tvalores["opdesc"].ascii(), m_tvalores["idmpatrimonial_nueva"].ascii());
            conexionbase->begin();
            conexionbase->ejecuta(SQLQuery);
            conexionbase->commit();
            m_tvalores["opdesc"] = "";
         }// end if
      }// end mpatrimonial
      m_tag="";
      m_data="";
      return TRUE;
}// endElement1


/** En el segundo paso se registran compmasap y compbalance
  * Estos elementos no requieren que se haga primero una inserción y luego una actualización
  * \param qName el nombre del tag abierto
  * \return TRUE para no detener la ejecución del algoritmo
  */
bool importbalance::startElement2( const QString& , const QString& , const QString &qName , const QXmlAttributes&) {
   m_tag=qName;
   return TRUE;
}// end startElement2


/** En el segundo paso introducimos los compmasap y los compbalance
  * Siempre se insertan por flanco final porque no tienen dependencias de nada y son sencillos.
  * Se usa el mapa m_identmasasp para saber que identificadores de masa se han usado en el paso 0
  * \param qName contiene el valor del tag que se acaba de cerrar
  * \return TRUE para no detener nunca la ejecución del algoritmo
  */
bool importbalance::endElement2( const QString& , const QString& , const QString &qName  ) {
    m_tag=qName;
    QString SQLQuery;
    /// En el segundo paso se hacen las inserciones por el flanco de final en lugar de por el flanco de principio.
    /// Así nos aseguramos que ya existen los valores de idmpatrimonial y masaperteneciente.
    if (m_tag == "compmasap") {
       SQLQuery.sprintf("INSERT INTO compmasap (masaperteneciente) VALUES ( %s)\n", conexionbase->sanearCadena(m_identmasasp[m_tvalores["masaperteneciente"]]).ascii());
       conexionbase->begin();
       conexionbase->ejecuta(SQLQuery);
       SQLQuery = "SELECT max(idcompmasap) AS idcompmasap FROM compmasap";
       cursor2 *cur = conexionbase->cargacursor(SQLQuery,"unquerymas");
       conexionbase->commit();
       if (!cur->eof()) {
         m_tvalores["idcompmasap"]=cur->valor("idcompmasap");
       }// end if
       delete cur; 
    }// end if
    if (m_tag == "compbalance") {
       SQLQuery.sprintf("INSERT INTO compbalance (idbalance) VALUES ( %s)\n", conexionbase->sanearCadena(m_tvalores["idbalance"]).ascii());
       conexionbase->begin();
       conexionbase->ejecuta(SQLQuery);
       SQLQuery = "SELECT max(idcompbalance) AS idcompbalance FROM compbalance";
       cursor2 *cur = conexionbase->cargacursor(SQLQuery,"unquerymas");
       conexionbase->commit();
       if (!cur->eof()) {
         m_tvalores["idcompbalance"]=cur->valor("idcompbalance");
       }// end if
       delete cur; 
    }// end if    
    /// Como la inserción está hecha, podemos hacer los updates sin miedo a que no exista el registro.
    if (m_tag == "compmasap") {
         // Actualizamos el idmasapatrimonial del compmasap, que es el que más dolores de cabeza causa.
         if (m_tvalores["idmpatrimonial"] != "" && m_tvalores["codigo"] == "") {
            SQLQuery.sprintf("UPDATE compmasap SET idmpatrimonial=%s WHERE idcompmasap=%s\n", conexionbase->sanearCadena(m_identmasasp[m_tvalores["idmpatrimonial"]]).ascii(), conexionbase->sanearCadena(m_tvalores["idcompmasap"]).ascii());
            conexionbase->begin();
            conexionbase->ejecuta(SQLQuery);
            conexionbase->commit();         
         }// end if
         if (m_tvalores["codigo"] != "") {
            SQLQuery.sprintf("UPDATE compmasap SET idcuenta=id_cuenta('%s') WHERE idcompmasap=%s\n",
	    conexionbase->sanearCadena(m_tvalores["codigo"]).ascii(),
	    conexionbase->sanearCadena(m_tvalores["idcompmasap"]).ascii());
            conexionbase->begin();
            conexionbase->ejecuta(SQLQuery);
            conexionbase->commit();         
         }// end if
    
         SQLQuery.sprintf("UPDATE compmasap SET signo='%s' WHERE idcompmasap=%s\n",
	 conexionbase->sanearCadena(m_tvalores["signo"]).ascii(),
	 conexionbase->sanearCadena(m_tvalores["idcompmasap"]).ascii());
         conexionbase->begin();
         conexionbase->ejecuta(SQLQuery);
         conexionbase->commit();

         SQLQuery.sprintf("UPDATE compmasap SET nombre='%s' WHERE idcompmasap=%s\n",
	 conexionbase->sanearCadena(m_tvalores["nombre"]).ascii(),
	 conexionbase->sanearCadena(m_tvalores["idcompmasap"]).ascii());
         conexionbase->begin();
         conexionbase->ejecuta(SQLQuery);
         conexionbase->commit();         
                  
         m_tvalores["codigo"]="";
         m_tvalores["idmpatrimonial"]=""; 
      }// end compmasap      
    if (m_tag == "compbalance") {
         // Con los componentes del balance también intervienen las masas patrimoniales.
         SQLQuery.sprintf("UPDATE compbalance SET idmpatrimonial=%s WHERE idcompbalance=%s\n",
	 conexionbase->sanearCadena(m_identmasasp[m_tvalores["idmpatrimonial"]]).ascii(),
	 conexionbase->sanearCadena(m_tvalores["idcompbalance"]).ascii());
         conexionbase->begin();
         conexionbase->ejecuta(SQLQuery);
         conexionbase->commit();
         
         SQLQuery.sprintf("UPDATE compbalance SET orden=%s WHERE idcompbalance=%s\n",
	 conexionbase->sanearCadena(m_tvalores["orden"]).ascii(),
	 conexionbase->sanearCadena(m_tvalores["idcompbalance"]).ascii());
         conexionbase->begin();
         conexionbase->ejecuta(SQLQuery);
         conexionbase->commit();
         
         SQLQuery.sprintf("UPDATE compbalance SET concepto='%s' WHERE idcompbalance=%s\n",
	 conexionbase->sanearCadena(m_tvalores["concepto"]).ascii(),
	 conexionbase->sanearCadena(m_tvalores["idcompbalance"]).ascii());
         conexionbase->begin();
         conexionbase->ejecuta(SQLQuery);
         conexionbase->commit();

         SQLQuery.sprintf("UPDATE compbalance SET tabulacion=%s WHERE idcompbalance=%s\n",
	 conexionbase->sanearCadena(m_tvalores["tabulacion"]).ascii(),
	 conexionbase->sanearCadena(m_tvalores["idcompbalance"]).ascii());
         conexionbase->begin();
         conexionbase->ejecuta(SQLQuery);
         conexionbase->commit();         
         m_tvalores["codigo"]="";
         m_tvalores["idmpatrimonial"]=""; 
    }// end compbalance  
    m_tag="";
    m_data="";  
   return TRUE;
}// endElement2

/** Ha ocurrido una inserción de valor en un tag y almacenamos el valor de la tupla (tag, valor) en el mapa m_tvalores
  * Para que pueda ser utilizado posteriormente
  * \param ch El valor del tag abierto
  * \return TRUE porque no nos interesa abortar la ejecución del algoritmo.
  */
bool importbalance::characters( const QString& ch ) {
    if(m_tag != "") {
	m_data =ch;
	m_tvalores[m_tag]=m_data;
    }// end if
    return TRUE;
}// endElement

