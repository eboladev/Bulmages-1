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

#include "importainteligente.h"
#include "empresa.h"
#include <qstring.h>

importainteligente::importainteligente( empresa *emp) : QXmlDefaultHandler() {
   fprintf(stderr,"Inicializador de importainteligente\n");
   empresaactual = emp;
   conexionbase = emp->bdempresa();
   tag="";
   data="";
}// end importainteligente


bool importainteligente::startElement( const QString& , const QString& , const QString& qName, const QXmlAttributes&) {
    tag=qName;
    QString SQLQuery;
    if (tag == "ainteligente") {      
       SQLQuery.sprintf("INSERT INTO ainteligente (descripcion) VALUES ('Elemento importado')\n");
       conexionbase->begin();
       conexionbase->ejecuta(SQLQuery);
       SQLQuery = "SELECT max(idainteligente) AS idainteligente FROM ainteligente";
       cursor2 *cur = conexionbase->cargacursor(SQLQuery,"unquerymas");
       conexionbase->commit();
       if (!cur->eof()) {
         tvalores["idainteligente"]=cur->valor("idainteligente");
       }// end if
       delete cur;
    }// end ainteligente
    if (tag == "binteligente") {
       SQLQuery.sprintf("INSERT INTO binteligente (idainteligente) VALUES (%s)\n", tvalores["idainteligente"].ascii());
       conexionbase->begin();
       conexionbase->ejecuta(SQLQuery);
       SQLQuery = "SELECT max(idbinteligente) AS idbinteligente FROM binteligente";
       cursor2 *cur = conexionbase->cargacursor(SQLQuery,"unquerymas");
       conexionbase->commit();
       if (!cur->eof()) {
         tvalores["idbinteligente"]=cur->valor("idbinteligente");
       }// end if
       delete cur; 
    }// end binteligente
    if (tag == "ivainteligente") {
       SQLQuery.sprintf("INSERT INTO ivainteligente (idainteligente, idbinteligente) VALUES (%s, %s)\n", tvalores["idainteligente"].ascii(), tvalores["idbinteligente"].ascii());
       conexionbase->begin();
       conexionbase->ejecuta(SQLQuery);
       SQLQuery = "SELECT max(idivainteligente) AS idivainteligente FROM ivainteligente";
       cursor2 *cur = conexionbase->cargacursor(SQLQuery,"unquerymas");
       conexionbase->commit();
       if (!cur->eof()) {
         tvalores["idivainteligente"]=cur->valor("idivainteligente");
       }// end if
       delete cur; 
    }// end ivainteligente
    return TRUE;
}// end startElement


bool importainteligente::endElement( const QString& , const QString& , const QString& qName ) {
   QString SQLQuery;   
   if (qName == "ainteligente") {
         SQLQuery.sprintf("UPDATE ainteligente SET descripcion='%s' WHERE idainteligente=%s\n", tvalores["descripcion"].ascii(), tvalores["idainteligente"].ascii());
         conexionbase->begin();
         conexionbase->ejecuta(SQLQuery);
         conexionbase->commit();
          SQLQuery.sprintf("UPDATE ainteligente SET comentariosasiento='%s' WHERE idainteligente=%s\n", tvalores["comentariosasiento"].ascii(), tvalores["idainteligente"].ascii());
         conexionbase->begin();
         conexionbase->ejecuta(SQLQuery);
         conexionbase->commit();
      }// end ainteligente
      if (qName == "binteligente") {
         SQLQuery.sprintf("UPDATE binteligente SET fecha='%s' WHERE idbinteligente=%s\n", tvalores["fecha"].ascii(), tvalores["idbinteligente"].ascii());
         conexionbase->begin();
         conexionbase->ejecuta(SQLQuery);
         conexionbase->commit();
         
         SQLQuery.sprintf("UPDATE binteligente SET conceptocontable='%s' WHERE idbinteligente=%s\n", tvalores["conceptocontable"].ascii(), tvalores["idbinteligente"].ascii());
         conexionbase->begin();
         conexionbase->ejecuta(SQLQuery);
         conexionbase->commit();
         
         SQLQuery.sprintf("UPDATE binteligente SET codcuenta='%s' WHERE idbinteligente=%s\n", tvalores["codcuenta"].ascii(), tvalores["idbinteligente"].ascii());
         conexionbase->begin();
         conexionbase->ejecuta(SQLQuery);
         conexionbase->commit();
         
         SQLQuery.sprintf("UPDATE binteligente SET descripcion='%s' WHERE idbinteligente=%s\n", tvalores["descripcionb"].ascii(), tvalores["idbinteligente"].ascii());
         conexionbase->begin();
         conexionbase->ejecuta(SQLQuery);
         conexionbase->commit();
         
         SQLQuery.sprintf("UPDATE binteligente SET debe='%s' WHERE idbinteligente=%s\n", tvalores["debe"].ascii(), tvalores["idbinteligente"].ascii());
         conexionbase->begin();
         conexionbase->ejecuta(SQLQuery);
         conexionbase->commit();
         
         SQLQuery.sprintf("UPDATE binteligente SET haber='%s' WHERE idbinteligente=%s\n", tvalores["haber"].ascii(), tvalores["idbinteligente"].ascii());
         conexionbase->begin();
         conexionbase->ejecuta(SQLQuery);
         conexionbase->commit();
         
         SQLQuery.sprintf("UPDATE binteligente SET contrapartida='%s' WHERE idbinteligente=%s\n", tvalores["contrapartida"].ascii(), tvalores["idbinteligente"].ascii());
         conexionbase->begin();
         conexionbase->ejecuta(SQLQuery);
         conexionbase->commit();
         
         SQLQuery.sprintf("UPDATE binteligente SET comentario='%s' WHERE idbinteligente=%s\n", tvalores["comentario"].ascii(), tvalores["idbinteligente"].ascii());
         conexionbase->begin();
         conexionbase->ejecuta(SQLQuery);
         conexionbase->commit();
         
         SQLQuery.sprintf("UPDATE binteligente SET canal='%s' WHERE idbinteligente=%s\n", tvalores["canal"].ascii(), tvalores["idbinteligente"].ascii());
         conexionbase->begin();
         conexionbase->ejecuta(SQLQuery);
         conexionbase->commit();
         
         SQLQuery.sprintf("UPDATE binteligente SET idc_coste='%s' WHERE idbinteligente=%s\n", tvalores["idc_coste"].ascii(), tvalores["idbinteligente"].ascii());
         conexionbase->begin();
         conexionbase->ejecuta(SQLQuery);
         conexionbase->commit();
         
         SQLQuery.sprintf("UPDATE binteligente SET marcaconciliacion='%s' WHERE idbinteligente=%s\n", tvalores["marcaconciliacion"].ascii(), tvalores["idbinteligente"].ascii());
         conexionbase->begin();
         conexionbase->ejecuta(SQLQuery);
         conexionbase->commit();
         
      }// end binteligente
      if (qName == "ivainteligente") {
         SQLQuery.sprintf("UPDATE ivainteligente SET contrapartida='%s' WHERE idivainteligente=%s\n", tvalores["contrapartida"].ascii(), tvalores["idivainteligente"].ascii());
         conexionbase->begin();
         conexionbase->ejecuta(SQLQuery);
         conexionbase->commit();
         
         SQLQuery.sprintf("UPDATE ivainteligente SET baseimp='%s' WHERE idivainteligente=%s\n", tvalores["baseimp"].ascii(), tvalores["idivainteligente"].ascii());
         conexionbase->begin();
         conexionbase->ejecuta(SQLQuery);
         conexionbase->commit();
         
         SQLQuery.sprintf("UPDATE ivainteligente SET iva='%s' WHERE idivainteligente=%s\n", tvalores["iva"].ascii(), tvalores["idivainteligente"].ascii()); 
         conexionbase->begin();
         conexionbase->ejecuta(SQLQuery);
         conexionbase->commit();
         
         SQLQuery.sprintf("UPDATE ivainteligente SET factura='%s' WHERE idivainteligente=%s\n", tvalores["factura"].ascii(), tvalores["idivainteligente"].ascii());
         conexionbase->begin();
         conexionbase->ejecuta(SQLQuery);
         conexionbase->commit();
         
         SQLQuery.sprintf("UPDATE ivainteligente SET idborrador='%s' WHERE idivainteligente=%s\n", tvalores["idborrador"].ascii(), tvalores["idivainteligente"].ascii());
         conexionbase->begin();
         conexionbase->ejecuta(SQLQuery);
         conexionbase->commit();
         
         SQLQuery.sprintf("UPDATE ivainteligente SET incregistro='%s' WHERE idivainteligente=%s\n", tvalores["incregistro"].ascii(), tvalores["idivainteligente"].ascii());
         conexionbase->begin();
         conexionbase->ejecuta(SQLQuery);
         conexionbase->commit();
         
         SQLQuery.sprintf("UPDATE ivainteligente SET regularizacion='%s' WHERE idivainteligente=%s\n", tvalores["regularizacion"].ascii(), tvalores["idivainteligente"].ascii());
         conexionbase->begin();
         conexionbase->ejecuta(SQLQuery);
         conexionbase->commit();
         
         SQLQuery.sprintf("UPDATE ivainteligente SET plan349='%s' WHERE idivainteligente=%s\n", tvalores["plan349"].ascii(), tvalores["idivainteligente"].ascii());  
         conexionbase->begin();
         conexionbase->ejecuta(SQLQuery);
         conexionbase->commit();
         
         SQLQuery.sprintf("UPDATE ivainteligente SET numorden='%s' WHERE idivainteligente=%s\n", tvalores["incregistro"].ascii(), tvalores["numorden"].ascii());  
         conexionbase->begin();
         conexionbase->ejecuta(SQLQuery);
         conexionbase->commit();
         
         SQLQuery.sprintf("UPDATE ivainteligente SET cif='%s' WHERE idivainteligente=%s\n", tvalores["cif"].ascii(), tvalores["idivainteligente"].ascii());
         conexionbase->begin();
         conexionbase->ejecuta(SQLQuery);
         conexionbase->commit();
         
      }// end ivainteligente
      tag="";
      data="";
      return TRUE;
}// endElement


bool importainteligente::characters( const QString& ch ) {
    data =ch;
    tvalores[tag]=data;
    return TRUE;
}// endElement

