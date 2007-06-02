/***************************************************************************
 *   Copyright (C) 2004 by Tomeu Borras Riera                              *
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

#ifndef BULMAFACT_H
#define BULMAFACT_H

#include <QMainWindow>
#include <QWorkspace>

#include "qworkspace2.h"
#include "ui_bulmafactbase.h"
#include "company.h"
#include "listventanas.h"
#include "funcaux.h"


/** Esta es la clase principal del programa ya que es la que deriva de QMainWindow.
    Su funcionalidad es servir de base como aplicacion Qt y inicializar los componentes clave.
    Tambien sirve de soporte al dise&ntilde;o especificado en bulmafactbase.ui con sus menus
    y ToolBoxes.
    Hace todo el traspaso de mensajes de los menus a company y captura algunos que no son
    propiamente de la facturacion como pueda ser el FullScreen o el About.
*/
class bulmafact: public QMainWindow, public Ui_bulmafactbase {
    Q_OBJECT

private:
    /// El workSpace que se va a usar con la aplicacion.
    QWorkspace2 *pWorkspace;
    /// La clase corazon de la aplicacion y centralizadora de mensajes y componentes.
    company *m_company;
    /// El listado de ventanas abiertas que esta en forma de dock en la parte izquierda.
    listventanas *m_list;

public:
    bulmafact(QString bd);
    ~bulmafact();
    QWorkspace2 *workspace();
    company *getcompany();
    void createMainWindows();

public slots:
    virtual void closeEvent(QCloseEvent *);
    virtual void emitirfactura();
    virtual void recibirfactura();
    virtual void s_FPago();
    virtual void s_ventanaCompleta();
    virtual void s_About();
    virtual void on_actionDocumentacion_triggered();
    virtual void on_actionNuevo_Pedido_Proveedor_triggered();
    virtual void on_actionListado_Pedidos_Proveedor_triggered();
    virtual void on_actionNuevo_Albaran_Proveedor_triggered();
    virtual void on_actionListado_Albaranes_Proveedor_triggered();
    virtual void on_actionNueva_Factura_Proveedor_triggered();
    virtual void on_actionListado_Facturas_Proveedor_triggered();
    virtual void on_actionNuevo_Presupuesto_triggered();
    virtual void on_actionListado_Presupuestos_triggered();
    virtual void on_actionNuevo_Pedido_Cliente_triggered();
    virtual void on_actionListado_Pedidos_Cliente_triggered();
    virtual void on_actionNuevo_Albaran_Cliente_triggered();
    virtual void on_actionListado_Albaranes_Cliente_triggered();
    virtual void on_actionNueva_Factura_Cliente_triggered();
    virtual void on_actionListado_Facturas_Cliente_triggered();
    virtual void on_actionGestion_Familias_triggered();
    virtual void on_actionGestion_Tipos_Articulo_triggered();
    virtual void on_actionNuevo_Articulo_triggered();
    virtual void on_actionListado_de_Articulos_triggered();
    virtual void on_actionNuevo_Proveedor_triggered();
    virtual void on_actionListado_Proveedores_triggered();
    virtual void on_actionNuevo_Cliente_triggered();
    virtual void on_actionListado_Clientes_triggered();
    virtual void on_actionProvincias_triggered();
    virtual void on_actionSeries_de_Factura_triggered();
    virtual void on_actionTrabajadores_triggered();
    virtual void on_actionBancos_triggered();
    virtual void on_actionVentana_Completa_triggered();
    virtual void on_actionAcerta_de_triggered();
    virtual void on_actionAlmacenes_triggered();
    virtual void on_actionListado_de_Pagos_triggered();
    virtual void on_actionNuevo_Pago_triggered();
    virtual void on_actionNuevo_Cobro_triggered();
    virtual void on_actionListado_de_Cobros_triggered();
    virtual void on_actionParametros_triggered();
    virtual void on_actionFormas_de_Pago_triggered();
    virtual void on_actionIndexador_triggered();
    virtual void on_actionTipoIVA_triggered();
    virtual void on_actionTasaIVA_triggered();

private slots:
    /// Este SLOT sirve para actualizar el Indexador cuando se activa una ventana
    void informaindexador(QWidget *w);
    void setActionIndexador(bool);
    void documentacionError(const QString);
};

#endif

