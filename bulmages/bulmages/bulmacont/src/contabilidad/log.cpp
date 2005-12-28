/***************************************************************************
 *Copyright (C) 2003 by Fco. Javier MC, Jean René Mérou, Víctor G. Marimón *
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
#include "log.h"
#include "configuracion.h"
#include "empresa.h"

#include <qdatetime.h>
#include "funcaux.h"

/// Esta variable global es el objeto del sistema de debug del programa
bitacora *ctllog;

bitacora::bitacora() {
   _depura("Constructor de Bitacora");
    // abre archivo Log.
    archivolog = new ofstream(confpr->valor(CONF_ARCHIVO_LOG).ascii(),ios::out | ios::app);      
    empresaactual = NULL;
    _depura("Fin de Constructor de Bitacora");
}


bitacora::~bitacora() {
    // Cierra archivo Log.
    archivolog->close();
}

void bitacora::add (int tipoLog, int nivellog, QString logdebug , QString qsTxt ) {
     QDateTime hora = QDateTime::currentDateTime();
     QString horastr = hora.toString(Qt::LocalDate);
     QString tipoLogtxt;
     
    /*switch(tipoLog) { //  Lo he comentado porque el tipoLogtxt podría ser variado si se considera que el numero puede ser tipo filtro
    	case LOG_SEG:
		tipoLogtxt = "SEG";
	break;
	case LOG_SIS:
		tipoLogtxt = "SIS";
	break;
    }// end tipoLog
    // Escribir en archivo log.
    *archivolog << tipoLog << ", " << tipoLogtxt.ascii() << ", " << horastr.ascii() << ", " empresaactual->nomuserempresa().ascii() << ", " << qsTxt.ascii() << endl;
    */
   // printf("empresa actual: %s  usuario actual: %s",empresaactual->nomuserempresa().ascii(),usuario );
   if (empresaactual != NULL) {
   // el int tipo de log da idea de si es un log de seguridad, de acceso a base de datos, ... etc
   // el int nivel log da idea de la profundidad del log, ejemplos los más generales=1, absolutamente todos 9
   // el qstring logdebug sirve para saber donde está la orden de un log para ayudar a programar, tiene 3 partes:
   //                                          1- nombre de la clase (abreviado), 2- nombre funcion abr. 3- nmero de log (general)
    
    *archivolog << tipoLog <<  ", "  << nivellog << ", " << logdebug.ascii() << ", " << horastr.ascii() << ", usuario:" << empresaactual->nomuserempresa().ascii() << ", BD:" << empresaactual->nombreDB.ascii() << ", "<< qsTxt.ascii() << endl;
    //", DB:" << empresaactual.nombreDB.ascii()
    
   } else {
    *archivolog << tipoLog <<  ", " << nivellog << ", " << logdebug.ascii() << ", " << horastr.ascii() << ", "  << ", "  <<  ", " << qsTxt.ascii() << endl;
   }// end if
}
