/***************************************************************************
 *   Copyright (C) 2005 by Alvaro de Miguel                                *
 *   alvaro.demiguel@gmail.com                                             *
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

#ifndef CLIENTDELIVNOTESLIST_H
#define CLIENTDELIVNOTESLIST_H


#include "busquedacliente.h"
#include "busquedaarticulo.h"
#include "company.h"
#include "subform2bf.h"
#include "ficha.h"

/// Subformulario de albaranes de clientes.
/** Esta clase se encarga de implementar el subformulario de albaranes a clientes.
    Es una clase derivada de SubForm2Bf, donde estan las funciones especificas de la facturacion.
    que a su vez deriva de SubForm3 que tiene las funciones de subformularios.
    
    La funcionalidad de esta clase es configurar el subformulario para que funcione
    con la tabla de albaranes.
    
    Reimplementa los metodos de cargar para producir el query adecuado.
*/
class AlbaranClienteListSubform : public SubForm2Bf {
    Q_OBJECT
public:
    AlbaranClienteListSubform(QWidget *parent = 0);
    ~AlbaranClienteListSubform() {}

public slots:
    virtual void cargar() {
        _depura("AlbaranClienteListSubform::cargar\n", 0);
        QString SQLQuery = "SELECT * FROM albaran";
        cursor2 * cur= companyact()->cargacursor(SQLQuery);
        SubForm3::cargar(cur);
        delete cur;
    }
    virtual void cargar(QString query) {
        SubForm3::cargar(query);
    }
};

#include "ui_albaranclientelistbase.h"


/** Albaranes a Clientes.
    Esta clase representa la pantalla de albaranes a clientes.
*/
/// \TODO: Deberia crearse una clase derivada de Ficha que fuese Listado y que
/// incorporase algunas funcionalidades comunes a todos los listados.
class AlbaranClienteList : public Ficha, public Ui_AlbaranClienteListBase {
    Q_OBJECT

public:
    enum edmode {
        EditMode = 0,
        SelectMode = 1
    };

private:
    /// El puntero a company que se propaga siempre en toda la aplicacion.
    company *m_companyact;
    
    /// El modo del listado, modo edicion o modo selector.
    /// m_modo == 0 es modo edicion.
    /// m_modo == 1 es modo selector.
    /// \TODO: Traspasar a la clase listado.
    int m_modo;
    
    /// En el modo selector esta variable indica el elemento seleccionado.
    QString mdb_idalbaran;

public:
    AlbaranClienteList(QWidget *parent = 0, Qt::WFlags flag = 0, edmode editmodo = EditMode);
    AlbaranClienteList(company *, QWidget *parent = 0, Qt::WFlags flag = 0, edmode editmodo = EditMode);
    ~AlbaranClienteList();
    void presenta();
    void modoseleccion() {
        m_modo = 1;
    }
    void modoedicion() {
        m_modo=0;
    }
    void imprimir();
    void setcompany(company *comp) {
        m_companyact = comp;
        m_cliente->setcompany(comp);
        mui_list->setcompany(comp);
    }
    void hideBotonera() {
        m_botonera->hide();
    }
    void showBotonera() {
        m_botonera->show();
    }
    void hideBusqueda() {
        m_busqueda->hide();
    }
    void showBusqueda() {
        m_busqueda->show();
    }
    void setidcliente(QString val) {
        m_cliente->setidcliente(val);
    }
    QString idCliDelivNote() {
        return mdb_idalbaran;
    }
    void meteWindow(QString nom, QObject *obj) {
        if (m_companyact != NULL) {
            m_companyact->meteWindow(nom, obj);
        }
    }
    QString generarFiltro();
    void editar(int);

public slots:
    virtual void on_m_filtro_textChanged(const QString &text) {
        if (text.size() >=3) {
            on_mui_actualizar_clicked();
        } // end if
    }
    virtual void on_mui_list_itemDoubleClicked(QTableWidgetItem *) {
        on_mui_editar_clicked();
    }
    virtual void on_mui_crear_clicked() {
        m_companyact->s_newAlbaranClienteView();
    }
    virtual void on_mui_borrar_clicked();
    virtual void on_mui_editar_clicked();
    virtual void on_mui_imprimir_clicked() {
        imprimir();
    }
    virtual void on_mui_actualizar_clicked() {
        presenta();
    }
    virtual void on_mui_configurar_toggled(bool checked) {
        if (checked) {
            mui_list->showConfig();
        } else {
            mui_list->hideConfig();
        } // end if
    }

signals:
    /// Estando en modo seleccion al seleccionar un elemento se emite esta se&ntilde;al.
    void selected(QString);
};

#endif

