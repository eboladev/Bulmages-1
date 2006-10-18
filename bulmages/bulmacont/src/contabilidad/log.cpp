/***************************************************************************
 *   Copyright (C) 2003 by Fco. Javier M. C.,                              *
 *                         Jean Rene Merou,                                *
 *                         Victor G. Marimon                               *
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

#include <QDateTime>

#include "log.h"
#include "configuracion.h"
#include "empresa.h"
#include "funcaux.h"


/// Esta variable global es el objeto del sistema de debug del programa.
bitacora *ctllog;


bitacora::bitacora() {
    _depura("Constructor de Bitacora");
    /// Abre el archivo de log.
    archivolog = new ofstream(confpr->valor(CONF_ARCHIVO_LOG).ascii(), ios::out | ios::app);
    empresaactual = NULL;
    _depura("Fin de Constructor de Bitacora");
}


bitacora::~bitacora() {
    /// Cierra el archivo de Log.
    archivolog->close();
}


void bitacora::add
    (int tipoLog, int nivellog, QString logdebug, QString qsTxt) {
    QDateTime hora = QDateTime::currentDateTime();
    QString horastr = hora.toString(Qt::LocalDate);
    QString tipoLogtxt;
    if (empresaactual != NULL) {
        /// El int tipo de log da idea de si es un log de seguridad, de acceso a base de
        /// datos, ... etc.
        /// El int nivel log da idea de la profundidad del log, ejemplos los m&aacute;s
        /// generales = 1, absolutamente todos 9.
        /// El qstring logdebug sirve para saber donde est&aacute; la orden de un log
        /// para ayudar a programar, tiene 3 partes:
        /// 1- nombre de la clase (abreviado),
        /// 2- nombre funcion abr.
        /// 3- nmero de log (general).
        *archivolog << tipoLog <<  ", "  << nivellog << ", " << logdebug.ascii() << ", " << horastr.ascii() << ", usuario:" << empresaactual->nomuserempresa().ascii() << ", BD:" << empresaactual->nombreDB.ascii() << ", "<< qsTxt.ascii() << endl;
    } else {
        *archivolog << tipoLog <<  ", " << nivellog << ", " << logdebug.ascii() << ", " << horastr.ascii() << ", "  << ", "  <<  ", " << qsTxt.ascii() << endl;
    } // end if
}

