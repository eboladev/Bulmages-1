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

#ifndef ABREEMPRESAVIEW_H
#define ABREEMPRESAVIEW_H

#include <QString>
#include <QDialog>

#include "ui_abreempresabase.h"


/// Esta clase muestra el dialogo de seleccionar una empresa y espera a que se haga doble click sobre uno de los
/// elementos que contiene, se haga click en aceptar o se de la accion de cancelar.
/// La ventan siempre se abre en modo modal y detiene la ejecucion del programa.
/// Se puede especificar, si es preciso, un tipo de empresas a mostrar (bulmacont o bulmafact)
/// para facilitar la seleccion de elementos.
class abreempresaview : public QDialog, public Ui_abreempresabase {
    Q_OBJECT

private:
    /// Se usa para almacenar el nombre de la base de datos seleccionada.
    QString m_empresabd;
    /// Se usa para almacenar el nombre de la empresa seleccionada.
    QString m_nombreempresa;
    /// Se usa para tener presente que tipo de elementos queremos que el selector muestre.
    QString m_tipo;
    /// Se usa para devolver el tipo de empresa que se ha seleccionado en el selector.
    QString m_tipoempresa;
    /// Almacena el nombre del usuario que desea acceder a la contabilidad.
    QString user;
    /// Almacena el password del usuario que desa acceder a la contabilidad.
    QString password;
    /// Indica  el modo en que opera la ventana. Cerrando la aplicacion al cancelar
    /// o cerrando solo la ventana. Por defecto es 0 -> cerrar aplicacion.
    int m_modo;

public:
    /// Establece el modo no destructivo
    void mododestructivo() {
        m_modo = 0;
    };
    void modonodestructivo() {
        m_modo = 1;
    };
    /// Inicia las variables m_tipo y m_tipoempresa y carga el archivo para hacer la presentacion.
    abreempresaview(QWidget *parent = 0, QString tipo = 0, const char *name = 0, bool modal = true);
    ~abreempresaview();
    /// Devuelve el nombre de la base de datos seleccionada.
    QString nomDB() {
        return m_empresabd;
    };
    /// Devuelve el nombre de la empresa seleccionada.
    QString nomEmpresa() {
        return m_nombreempresa;
    };
    /// Devuelve el tipo de empresa que se ha seleccionado.
    QString tipoEmpresa() {
        return m_tipoempresa;
    };
private:
    /// Guarda en el archivo especificado en LISTEMPRESAS la lista de empresas disponibles.
    void guardaArchivo();
    /// Carga del archivo especificado en LISTEMPRESAS la lista de empresas disponibles.
    void cargaArchivo();

private:
    /// Inserta una companya en el QList empresas definido en el dialogo.
    void insertCompany(QString, QString, QString, QString);
    void preparaempresas();

private slots:
    /// Al pulsar el boton de cancelar se aborta la ejecucion del programa.
    virtual void s_botonCancelar() {
        if(m_modo == 0)
            exit(1);
        else
            done(1);
    };
    bool close(bool) {
        s_botonCancelar();
        return true;
    }
    void abreempresaview::empresasdobleclick();

public slots:
    /// This SLOT is activated to select a company and to press enter.
    virtual void accept();
    /// This SLOT searches in the databases of postgres all the companyes needed.
    virtual void s_reloadButton();
};

#endif

