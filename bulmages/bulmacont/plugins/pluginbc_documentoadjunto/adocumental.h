/***************************************************************************
 *   Copyright (C) 2002 by Tomeu Borras Riera                              *
 *   tborras@conetxia.com                                                  *
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

#ifndef ADOCUMENTAL_H
#define ADOCUMENTAL_H

#include <QtWidgets/QFileDialog>
#include <QtCore/QFileInfo>
#include <QtWidgets/QWidget>
#include <QtWidgets/QMessageBox>
#include <QtWidgets/QDialog>
#include <QtWidgets/QTableWidgetItem>

#include "ui_adocumentalbase.h"
#include "blpostgresqlclient.h"


class BcCompany ;


class myplugin1 : public QObject
{
    Q_OBJECT

public:
    BcCompany *company;
    BlPostgreSqlClient *dbConnection;

public:
    myplugin1 ( BcCompany * );
    ~myplugin1();

public slots:
    virtual void boton_adjuntar();
    virtual void boton_nuevoasientodocumental();
    virtual void archDoc();
};


class adocumental : public QDialog, public Ui_adocumentalbase
{
    Q_OBJECT

private:
    BcCompany *company;
    BlPostgreSqlClient *dbConnection;
    QString RutaADocumental;
    /// En el modo edicion la ventana responde como si fuese un listado sobre el que se
    /// pueden editar los elementos la hacer doble click sobre ellos.
    /// En el modo consulta la ventana responde como si fuese un desplegable y se puede
    /// seleccionar el elemento con el que se desea tratar.
    /// Si modo vale 0 entonces es modo edicion.
    /// Si modo vale 1 entonces es modo consulta.
    int modo;
    /// Almacena por un breve lapso de tiemo el idadocumental que se ha seleccionado.
    QString idadocumental;

public:
    adocumental ( BcCompany *, QWidget *parent = 0 );
    ~adocumental();
    void setEditMode() {
        modo = 0;
    };
    void setSelectMode() {
        modo = 1;
    };
    QString getidadocumental();
    void asociaasiento ( QString );
    void presentaprimervacio();
    /// A veces es necesario repintar las cosas. Esta funcion es la que lo hace.
    void inicializa();
    void newADocumental ( QString );

public slots:
    virtual void doubleclicked ( int, int, int, const QPoint & );
    virtual void boton_newadocumental();
    virtual void boton_desasociar();
    virtual void s_deleteADocumental();
    virtual void s_saveADocumental();
    virtual void s_agregarDirectorio();
};

#endif

