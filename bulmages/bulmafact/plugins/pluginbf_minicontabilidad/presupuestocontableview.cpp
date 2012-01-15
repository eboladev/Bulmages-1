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
    BL_FUNC_DEBUG
    setAttribute ( Qt::WA_DeleteOnClose );
    try {
        setupUi ( this );

        setTitleName ( _ ( "Presupuesto Contable" ) );
        setDbTableName ( "presupuestocontable" );
        setDbFieldId ( "idpresupuestocontable" );
        addDbField ( "idpresupuestocontable", BlDbField::DbInt, BlDbField::DbPrimaryKey, _ ( "ID presupuesto contable" ) );
        addDbField ( "fechapresupuestocontable", BlDbField::DbDate, BlDbField::DbNothing, _ ( "Fecha" ) );
        addDbField ( "conceptopresupuestocontable", BlDbField::DbVarChar, BlDbField::DbNothing, _ ( "Concepto" ) );


	mui_list->setDbTableName ( "lpresupuestocontable" );
	mui_list->setDbFieldId ( "idlpresupuestocontable" );
	mui_list->addSubFormHeader ( "idlpresupuestocontable", BlDbField::DbInt, BlDbField::DbPrimaryKey, BlSubFormHeader::DbHideView | BlSubFormHeader::DbNoWrite, _ ( "ID lpresupuestocontable" ) );
	mui_list->addSubFormHeader ( "idpresupuestocontable", BlDbField::DbInt, BlDbField::DbNotNull, BlSubFormHeader::DbHideView | BlSubFormHeader::DbNoWrite, _ ( "ID lpresupuestocontable" ) );
	mui_list->addSubFormHeader ( "idpartida", BlDbField::DbDate, BlDbField::DbNotNull, BlSubFormHeader::DbHideView | BlSubFormHeader::DbNoWrite, _ ( "ID partida" ) );
	mui_list->addSubFormHeader ( "codigocompletopartida", BlDbField::DbVarChar, BlDbField::DbNoSave, BlSubFormHeader::DbNone | BlSubFormHeader::DbNoWrite, _ ( "Codigo" ) );
	mui_list->addSubFormHeader ( "nombrepartida", BlDbField::DbVarChar, BlDbField::DbNoSave, BlSubFormHeader::DbNone | BlSubFormHeader::DbNoWrite, _ ( "Concepto" ) );
	mui_list->addSubFormHeader ( "conceptolpresupuestocontable", BlDbField::DbVarChar, BlDbField::DbNotNull, BlSubFormHeader::DbNone, _ ( "Nombre" ) );
	mui_list->addSubFormHeader ( "saldolpresupuestocontable", BlDbField::DbNumeric, BlDbField::DbNotNull, BlSubFormHeader::DbNone, _ ( "Saldo" ) );
	mui_list->setInsert ( FALSE );
	mui_list->setDelete ( FALSE );
	mui_list->setSortingEnabled ( TRUE );


        /// Disparamos los plugins.
        int res = g_plugins->run ( "PresupuestoContableView_PresupuestoContableView", this );
        if ( res != 0 ) {
            return;
        } // end if

        /// Usurpamos la identidad de mlist y ponemos nuestro propio widget con sus cosillas.
        mui_list->setMainCompany ( comp );

        setListaLineas ( mui_list );

        insertWindow ( windowTitle(), this, FALSE );
        /// Disparamos los plugins por flanco descendente.
        g_plugins->run ( "PresupuestoContableView_PresupuestoContableView_Post", this );
	blScript(this);
    } catch ( ... ) {
        blMsgInfo ( _ ( "Error al crear el presupuesto" ), this );
    } // end try

    
}


/// Este metodo es llamado cuando hacemos un nuevo registro, pero no hay carga desde la base de datos.
/**
**/
void PresupuestoContableView::inicializar()
{
    BL_FUNC_DEBUG
    mui_list->inicializar();
    dialogChanges_readValues();
    
}


///
/**
**/
PresupuestoContableView::~PresupuestoContableView()
{
    BL_FUNC_DEBUG
    /// Disparamos los plugins.
    g_plugins->run ( "PresupuestoContableView_DesPresupuestoContableView", this );
    
}




///
/**
\return
**/
int PresupuestoContableView::beforeDelete()
{
    BL_FUNC_DEBUG
    /// Disparamos los plugins con presupuesto_imprimirPresupuesto.
    g_plugins->run ( "PresupuestoContableView_beforeDelete", this );
    mui_list->remove();
    
    return 0;
}


/// Esta funcion carga un Presupuesto.
/**
\param idbudget
\return
**/
int PresupuestoContableView::cargarPost ( QString idbudget )
{
    BL_FUNC_DEBUG

    QString query = "SELECT * FROM ( SELECT * FROM partida WHERE idpartida NOT IN (SELECT DISTINCT COALESCE(padre,0) FROM partida) ) AS t1 LEFT JOIN (SELECT idlpresupuestocontable, coalesce(saldolpresupuestocontable, 0) AS saldolpresupuestocontable, idpartida, idpresupuestocontable, conceptolpresupuestocontable FROM lpresupuestocontable) AS t2 ON t2.idpartida = t1.idpartida AND t2.idpresupuestocontable = "+idbudget;
    mui_list->load ( query );

    
    return 0;
}


///
/**
\return
**/
int PresupuestoContableView::afterSave()
{
    BL_FUNC_DEBUG

    mui_list->setColumnValue ( "idpresupuestocontable", dbValue ( "idpresupuestocontable" ) );

    mui_list->save();
    /// Disparamos los plugins con presupuesto_imprimirPresupuesto.
    g_plugins->run ( "PresupuestoContableView_afterSave_Post", this );

    
    return 0;
}

