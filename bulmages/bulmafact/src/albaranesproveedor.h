/***************************************************************************
 *   Copyright (C) 2004 by Tomeu Borras Riera                              *
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

#ifndef ALBARANESPROVEEDOR_H
#define ALBARANESPROVEEDOR_H

#include <QLineEdit>

#include "company.h"
#include "busquedaproveedor.h"
#include "busquedaarticulo.h"
#include "funcaux.h"
#include "subform2bf.h"
#include "ficha.h"


/// Clase que maneja el subformulario de los albaranes de proveedor.
/** Deriva de la clase SubForm2Bf.
    Reimplementa los metodos de carga para que trabajan con la tabla albaranp.
*/
class AlbaranesProveedorListSubform : public SubForm2Bf {
    Q_OBJECT

public:
    AlbaranesProveedorListSubform(QWidget *parent = 0);
    ~AlbaranesProveedorListSubform();

public slots:
    virtual void cargar();
    virtual void cargar(QString query);
};


#include "ui_albaranesproveedorlistbase.h"


/// Clase que controla la pantalla de listado de albaranes a proveedor.
/// \TODO: Deberia derivar de Ficha o Listado en lugar de QWidget.
class AlbaranesProveedor : public Ficha, public Ui_AlbaranesProveedorListBase {
    Q_OBJECT

private:
    /// El puntero m_companyact que se propaga por practicamente todas las clases de la aplicacion.
    company *m_companyact;
    /// Indicador de modo edicion o modo seleccion
    /// == 0 es modo edicion.
    /// == 1 es modo selector.
    int m_modo;
    /// En el caso de estar en modo seleccion esta variable de clase indicara el identificador seleccionado.
    QString mdb_idalbaranp;

public:
    AlbaranesProveedor(QWidget *parent = 0, Qt::WFlags flag = 0);
    AlbaranesProveedor(company *comp = NULL, QWidget *parent = 0, Qt::WFlags flag = 0);
    ~AlbaranesProveedor();
    void presenta();
    void modoseleccion();
    void modoedicion();
    void setcompany(company *comp);
    QString idalbaranp();
    void hideBotonera();
    void showBotonera();
    void hideBusqueda();
    void showBusqueda();
    void imprimir();
    void meteWindow(QString nom, QObject *obj);
    void setidproveedor(QString val);
    void setidarticulo(QString val);
    QString generaFiltro();
    void editar(int);
    virtual void on_mui_borrar_clicked();

public slots:
    virtual void on_m_filtro_textChanged(const QString &text);
    void on_mui_list_itemDoubleClicked(QTableWidgetItem *);
    virtual void on_mui_editar_clicked();
    virtual void on_mui_crear_clicked();
    virtual void on_mui_imprimir_clicked();
    virtual void on_mui_actualizar_clicked();
    virtual void on_mui_configurar_toggled(bool checked);

signals:
    void selected(QString);
};

#endif

