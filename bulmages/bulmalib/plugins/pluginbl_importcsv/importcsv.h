/***************************************************************************
 *   Copyright (C) 2008 by Tomeu Borras Riera                              *
 *   tborras@conetxia.com                                                  *
 *   http://www.iglues.org                                                 *
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

#ifndef IMPORTCSV_H
#define IMPORTCSV_H


#include <QtWidgets/QLineEdit>
#include <QtWidgets/QLabel>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QWidget>
#include <QtCore/QMap>

#include "blfunctions.h"
#include "ui_importcsvbase.h"
#include "blpostgresqlclient.h"
#include "blwidget.h"
#include "pdefs_pluginbl_importcsv.h"


class BlMainCompany;


/** Ventana de ficha de cobro.
    Se encarga de la presentacion de la ficha de cobro y del tratamiento de eventos producidos
    en dicha ventana.
    Deriva de Ficha para metodos comunes a todas las ventanas.
    Deriva de Cobro para el manejo de la Base de datos. */
class PLUGINBL_IMPORTCSV_EXPORT ImportCSV : public BlWidget, public Ui_ImportCSVBase
{
    Q_OBJECT

private:
    QList<QString> m_claves;

public:
    ImportCSV ( BlMainCompany *, QWidget * );
    ~ImportCSV();
    void rellenarTablas();
    void procesarLinea ( const QString &linea );
    QString dbFieldTypeByName(QString fieldName, QString dbTable);
    QStringList csvLineParser(QString data, QString fieldDelimiter = ",", QString textDelimiter = "\"");

public slots:
    void on_mui_aceptar_clicked();
    void on_mui_combotablas_activated ( const QString & text );
    void on_mui_buscarArchivo_clicked();
    void on_mui_cabeceras_stateChanged ( int state );
};

#endif

