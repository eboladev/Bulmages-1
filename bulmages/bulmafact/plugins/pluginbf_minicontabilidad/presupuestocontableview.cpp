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

#include <QMessageBox>
#include <QWidget>
#include <QObject>
#include <QComboBox>
#include <QToolButton>
#include <QFile>
#include <QTextStream>
#include <QLayout>
#include <QMessageBox>
#include <QCloseEvent>

#include "blfunctions.h"
#include "presupuestocontableview.h"
#include "bfcompany.h"
#include "blconfiguration.h"
#include "blplugins.h"


///
/**
\param comp
\param parent
\return
**/
PresupuestoContableView::PresupuestoContableView ( BfCompany *comp, QWidget *parent )
        : BfForm ( comp, parent )
{
    _depura ( "Inicializacion de PresupuestoContableView", 0 );
    setAttribute ( Qt::WA_DeleteOnClose );
    try {
        setupUi ( this );

        setTitleName ( _ ( "Presupuesto" ) );
        setDbTableName ( "presupuesto" );
        setDbFieldId ( "idpresupuesto" );
        addDbField ( "idpresupuesto", BlDbField::DbInt, BlDbField::DbPrimaryKey, _ ( "ID presupuesto" ) );
        addDbField ( "idcliente", BlDbField::DbInt, BlDbField::DbNotNull, _ ( "Cliente" ) );
        addDbField ( "idalmacen", BlDbField::DbInt, BlDbField::DbNotNull, _ ( "Almacen" ) );
        addDbField ( "numpresupuesto", BlDbField::DbInt, BlDbField::DbNothing, _ ( "Numero de presupuesto" ) );
        addDbField ( "fpresupuesto", BlDbField::DbDate, BlDbField::DbNothing, _ ( "Fecha de creacion" ) );
        addDbField ( "vencpresupuesto", BlDbField::DbDate, BlDbField::DbNothing, _ ( "Fecha de vencimiento" ) );
        addDbField ( "contactpresupuesto", BlDbField::DbVarChar, BlDbField::DbNothing, _ ( "Persona de contacto" ) );
        addDbField ( "telpresupuesto", BlDbField::DbVarChar, BlDbField::DbNothing, _ ( "Numero de telefono" ) );
        addDbField ( "comentpresupuesto", BlDbField::DbVarChar, BlDbField::DbNothing, _ ( "Comentarios" ) );
        addDbField ( "procesadopresupuesto", BlDbField::DbBoolean, BlDbField::DbNothing, _ ( "Procesado" ) );
        addDbField ( "descpresupuesto", BlDbField::DbVarChar, BlDbField::DbNothing, _ ( "% Descuento" ) );
        addDbField ( "refpresupuesto", BlDbField::DbVarChar, BlDbField::DbNothing, _ ( "Referencia" ) );
        addDbField ( "idforma_pago", BlDbField::DbInt, BlDbField::DbNotNull, _ ( "Forma de pago" ) );
        addDbField ( "idtrabajador", BlDbField::DbInt, BlDbField::DbNothing, _ ( "Trabajador" ) );

        /// Disparamos los plugins.
        int res = g_plugins->lanza ( "PresupuestoContableView_PresupuestoContableView", this );
        if ( res != 0 ) {
            return;
        } // end if

        /// Usurpamos la identidad de mlist y ponemos nuestro propio widget con sus cosillas.
        mui_list->setMainCompany ( comp );

        setListaLineas ( mui_list );

        meteWindow ( windowTitle(), this, FALSE );
        /// Disparamos los plugins por flanco descendente.
        g_plugins->lanza ( "PresupuestoContableView_PresupuestoContableView_Post", this );
    } catch ( ... ) {
        mensajeInfo ( _ ( "Error al crear el presupuesto" ), this );
    } // end try
    _depura ( "Fin de la inicializacion de PresupuestoContableView", 0 );
}


/// Este metodo es llamado cuando hacemos un nuevo registro, pero no hay carga desde la base de datos.
/**
**/
void PresupuestoContableView::inicializar()
{
    _depura ( "PresupuestoContableView::inicializar", 0 );
    mui_list->inicializar();
    dialogChanges_cargaInicial();
    _depura ( "END PresupuestoContableView::inicializar", 0 );
}


///
/**
**/
PresupuestoContableView::~PresupuestoContableView()
{
    _depura ( "PresupuestoContableView::~PresupuestoContableView", 0 );
    /// Disparamos los plugins.
    g_plugins->lanza ( "PresupuestoContableView_DesPresupuestoContableView", this );
    _depura ( "END PresupuestoContableView::~PresupuestoContableView", 0 );
}




///
/**
\return
**/
int PresupuestoContableView::borrarPre()
{
    _depura ( "PresupuestoContableView::borrar", 0 );
    /// Disparamos los plugins con presupuesto_imprimirPresupuesto.
    g_plugins->lanza ( "Presupuesto_borrarPre", this );
    m_listalineas->borrar();
    m_listadescuentos->borrar();

    _depura ( "END PresupuestoContableView::borrar", 0 );
    return 0;
}


/// Esta funcion carga un Presupuesto.
/**
\param idbudget
\return
**/
int PresupuestoContableView::cargarPost ( QString idbudget )
{
    _depura ( "PresupuestoContableView::cargarPost", 0 );

    m_listalineas->cargar ( idbudget );
    m_listadescuentos->cargar ( idbudget );

    calculaypintatotales();

    _depura ( "END PresupuestoContableView::cargar", 0 );
    return 0;
}


///
/**
\return
**/
int PresupuestoContableView::guardarPost()
{
    _depura ( "PresupuestoContableView::guardarPost", 0 );

    m_listalineas->setColumnValue ( "idpresupuesto", dbValue ( "idpresupuesto" ) );
    m_listadescuentos->setColumnValue ( "idpresupuesto", dbValue ( "idpresupuesto" ) );

    m_listalineas->guardar();
    m_listadescuentos->guardar();
    /// Disparamos los plugins con presupuesto_imprimirPresupuesto.
    g_plugins->lanza ( "Presupuesto_guardarPost_Post", this );

    _depura ( "END PresupuestoContableView::guardar", 0 );
    return 0;
}

