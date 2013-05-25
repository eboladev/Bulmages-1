/***************************************************************************
 *   Copyright (C) 2004 by Tomeu Borras Riera                              *
 *   tborras@conetxia.com                                                  *
 *   http://www.iglues.org Asociación Iglues -- Contabilidad Linux         *
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

#ifndef BCASIENTOINTELIGENTEIMPORTARXML_H
#define BCASIENTOINTELIGENTEIMPORTARXML_H

#include <QtCore/QMap>
#include <QtCore/QString>
#include <QtXml/qxml.h>

#include "blwidget.h"
#include "bccompany.h"
#include "pdefs_pluginbc_asiento.h"


/// Sirve para importar asientos inteligentes (plantillas) dentro de la empresa en uso.
/** Esta clase es un analizador (parser) XML cuya funci&oacute;n es importar a la base
    de datos abierta una plantilla de asiento inteligente. Para ello utiliza las clases
    SAX de Qt. */
class PLUGINBC_ASIENTO_EXPORT BcAsientoInteligenteImportarXML: public QXmlDefaultHandler, public BlMainCompanyPointer
{
private:
    /// Indica el tag que el parser está procesando en este preciso momento.
    QString tag;
    /// Indica los datos adicionales que estám contenidos en el tag.
    QString data;
    /// Este mapa de valores sirve para almacenar de forma sencilla los valores que se han
    /// ido recogiendo y para poder ser recogidos en el momento de escritura en la base
    /// de datos.
    QMap<QString, QString> tvalores;


public:
    /// Constructor de la clase que inicializa las variables company y dbConnection.
    BcAsientoInteligenteImportarXML ( BcCompany * );
    /// Elemento del analizador (parser) de SAX que se dispara al encontrar un tag de inicio.
    bool startElement ( const QString&, const QString&, const QString&, const QXmlAttributes& );
    /// Elemento del analizador (parser) de SAX que se dispara al encontrar un tag de fin
    bool endElement ( const QString&, const QString&, const QString& );
    /// Elemento del analizador (parser) de SAX que se dispara al encontrar contenidos.
    bool characters ( const QString & );
};

#endif

