/***************************************************************************
 *   Copyright (C) 2004 by Tomeu Borras Riera                              *
 *   tborras@conetxia.com                                                  *
 *   http://www.iglues.org Asociacion Iglues -- Contabilidad Linux         *
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

#ifndef BCBALANCEIMPORTARXML_H
#define BCBALANCEIMPORTARXML_H

#include <QtCore/QMap>
#include <QtXml/qxml.h>

#include "bccompany.h"
#include "blwidget.h"


class QString;


/** Es una implementación de un parser XML hecho en SAX de Qt que utiliza del sistema de diversas pasadas
  * para asegurar hacer la importación de datos.
  * En una primera pasada introduce en el sistema las masas patrimoniales y en una segunda pasada introduce los componentes del balance
  * Es preciso mantener en memoria algunos identificadores y esto se hace mediante el uso de un mapa
  */
class BC_EXPORT BcBalanceImportarXML: public QXmlDefaultHandler, public BlMainCompanyPointer
{
private:

    /// Esta variable global indica siempre el tag que está procesando el parser
    QString m_tag;
    /// Esta variable global indica la información que se acaba de recolectar en el parser.
    /// De modo que al llegar a un tag de cierre siempre sea m_data quien tenga el valor
    /// introducido para poder utilizarlo.
    QString m_data;
    /// En este mapa se almacenan los valores que se van rescatando del XML, de tal forma
    /// que al llegar a un tag de cierre importante se recuperen de aqu&iacute; los datos a ser
    /// introducidos en la base de datos.
    QMap<QString, QString> m_tvalores;
    /// En el caso de las masas patrimoniales hay un problema para los identificadores.

    /// Este hash sirve para almacenar las claves provisionales de las masas patrimoniales.
    QMap<QString, QString> m_identmasasp;
    /// Indica que paso estamos realizando. Recordemos que en esta ocasi&oacute;n se hacen
    /// dos pasadas de analizador (parser) para hacer la importaci&oacute;n completamente.
    int m_paso;

public:
    /// Constructor de la clase que inicializa determinadas variables de clase.
    BcBalanceImportarXML ( BcCompany * );
    /// Implementaci&oacute;n del analizador (parser) que procesa un tag de inicio.
    bool startElement ( const QString&, const QString&, const QString&, const QXmlAttributes& );
    /// Implementaci&oacute;n del analizador (parser) que procesa un tag de cierre
    bool endElement ( const QString&, const QString&, const QString& );
    /// Implementaci&oacute;n del analizador (parser) que procesa un tag de inicio en la
    /// primera pasada.
    bool startElement1 ( const QString&, const QString&, const QString&, const QXmlAttributes& );
    /// Implementaci&oacute;n del analizador (parser) que procesa un tag de cierre en la
    /// primera pasada.
    bool endElement1 ( const QString&, const QString&, const QString& );
    /// Implementaci&oacute;n del analizador (parser) que procesa un tag de inicio en la
    /// segunda pasada.
    bool startElement2 ( const QString&, const QString&, const QString&, const QXmlAttributes& );
    /// Implementaci&oacute;n del analizador (parser) que procesa un tag de cierre en la
    /// primera pasada.
    bool endElement2 ( const QString&, const QString&, const QString& );
    /// Recoge valores asignados a un determinado tag y los almacena en la variable m_data
    bool characters ( const QString & );
    /// Cambia de numero de paso. Recordar que en este caso la importación se hace en dos
    /// pasos.
    void cambiapaso();
};

#endif

