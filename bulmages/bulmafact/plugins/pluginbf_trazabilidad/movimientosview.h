/***************************************************************************
 *   Copyright (C) 2005 by Tomeu Borras Riera                              *
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

#ifndef MOVIMIENTOSVIEW_H
#define MOVIMIENTOSVIEW_H

#include "busquedaarticulo.h"
#include "bfcompany.h"
#include "blfunctions.h"
#include "bfsubform.h"
#include "bfform.h"


/// Administra el detalle del listado de facturas a clientes.
/** */
class MovimientosSubform : public BfSubForm
{
    Q_OBJECT

public:
    MovimientosSubform ( QWidget *parent = 0, const char *name = 0 );
    ~MovimientosSubform() {}

public slots:
    virtual void cargar() {
        _depura ( "MovimientosSubform::cargar\n", 0 );
        QString SQLQuery = "SELECT * FROM factura";
        BlSubForm::cargar ( SQLQuery );
    }
    virtual void cargar ( QString query ) {
        BlSubForm::cargar ( query );
    }
};


#include "ui_movimientosbase.h"


/// Administra el listado de facturas a clientes.
/** */
class MovimientosView : public BfForm, public Ui_MovimientosBase
{
    Q_OBJECT

public:
    enum edmode {
        EditMode = 0,
        SelectMode = 1
    };

private:
    edmode m_modo;
    QString mdb_idfactura;

public:
//    MovimientosView(QWidget *parent = 0, edmode editmodo = EditMode);
    MovimientosView ( BfCompany *, QWidget *parent = 0, edmode editmodo = EditMode );
    virtual ~MovimientosView();
    void iniciaForm();
    void setMainCompany ( BfCompany *comp ) {
        BlMainCompanyPointer::setMainCompany ( comp );
        m_cliente->setMainCompany ( comp );
        m_articulo->setMainCompany ( comp );
        mui_list->setMainCompany ( comp );
    };

    int modo() {
        return m_modo;
    };
    void modoseleccion() {
        m_modo = SelectMode;
    };
    void modoedicion() {
        m_modo = EditMode;
    };
    QString idfactura() {
        return mdb_idfactura;
    };
    void hideBusqueda() {
        m_busqueda->hide();
    };
    void showBusqueda() {
        m_busqueda->show();
    };
    void setidcliente ( QString val ) {
        m_cliente->setId ( val );
    };
    void setidarticulo ( QString val ) {
        m_articulo->setidarticulo ( val );
    };
    QString generaFiltro();

    void presenta();

public slots:
    virtual void on_m_filtro_textChanged ( const QString &text ) {
        if ( text.size() >= 3 )
            on_mui_actualizar_clicked();
    };
    virtual void on_mui_actualizar_clicked() {
        presenta();
    }
    virtual void on_mui_crear_clicked() {
//        mainCompany() ->s_newFacturaCli();
    };
    virtual void on_mui_imprimir_clicked();
    virtual void on_mui_configurar_toggled ( bool checked ) {
        if ( checked ) {
            mui_list->showConfig();
        } else {
            mui_list->hideConfig();
        } // end if
    };

signals:
    void selected ( QString );
};

#endif

