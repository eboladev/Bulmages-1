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

#include <stdio.h>

#include <QAction>
#include <QObject>
#include <QMessageBox>
#include <QLineEdit>
#include <QTabWidget>

#include "bfcompany.h"
#include "pluginbf_trazabilidad.h"
#include "blfunctions.h"
#include "movimientosview.h"

BfBulmaFact *g_bges = NULL;


///
/**
\param bges
\return
**/
int entryPoint ( BfBulmaFact *bges )
{
    blDebug ( "entryPoint", 0, "Punto de Entrada de PluginBf_Trazabilidad" );

    /// El plugin necesita un parche en la base de datos para funcionar.
    bges->company()->dbPatchVersionCheck("PluginBf_Trazabilidad", "0.9.1");

    /// Inicializa el sistema de traducciones 'gettext'.
    setlocale ( LC_ALL, "" );
    blBindTextDomain ( "pluginbf_trazabilidad", g_confpr->valor ( CONF_DIR_TRADUCCION ).toAscii().constData() );
    g_bges = bges;

    /// Creamos el men&uacute;.
    BlAction *accionA = new BlAction ( _("&Movimientos"), 0 );
    accionA->setStatusTip ( _("Ventana de Movimientos") );
    accionA->setWhatsThis ( _("Movimientos") );
    accionA->setObjectName("mui_actionMovimientos");
    accionA->setIcon ( QIcon ( QString::fromUtf8 ( ":/Images/product-family.png" ) ) );

    bges->menuMaestro->addSeparator();
    bges->menuMaestro->addAction ( accionA );


    blDebug ( "END entryPoint", 0, "Punto de Entrada de PluginBf_Trazabilidad" );
    return 0;
}


int BlAction_triggered(BlAction *accion) {
    if (accion->objectName() == "mui_actionMovimientos") {
        blDebug ( "PluginBf_Trazabilidad::BlAction_triggered::mui_actionMovimientos", 0 );
        MovimientosView *mov = new MovimientosView ( ( BfCompany * ) g_bges->company() );
        g_bges->company() ->pWorkspace() ->addSubWindow ( mov );
        mov->show();
        blDebug ( "END PluginBf_Trazabilidad::BlAction_triggered::mui_actionMovimientos", 0 );        
    } // end if
    return 0;
}



///
/**
\param subform
\return
**/
int BfProveedorAlbaranSubForm_BfProveedorAlbaranSubForm ( BfProveedorAlbaranSubForm *subform )
{
    blDebug ( "BfProveedorAlbaranSubForm_BfProveedorAlbaranSubForm", 0 );
    subform->addSubFormHeader ( "lotelalbaranp", BlDbField::DbVarChar, BlDbField::DbNothing, BlSubFormHeader::DbNone, _("Lote") );
    blDebug ( "END BfProveedorAlbaranSubForm_BfProveedorAlbaranSubForm", 0 );
    return 0;
}

///
/**
\param subform
\return
**/
int BfClienteAlbaranSubForm_BfClienteAlbaranSubForm ( BfClienteAlbaranSubForm *subform )
{
    blDebug ( "BfClienteAlbaranSubForm_BfClienteAlbaranSubForm", 0 );
    subform->addSubFormHeader ( "lotelalbaran", BlDbField::DbVarChar, BlDbField::DbNothing, BlSubFormHeader::DbNone, _("Lote") );
    blDebug ( "END BfClienteAlbaranSubForm_BfClienteAlbaranSubForm", 0 );
    return 0;
}


///
/**
\param subform
\return
**/
int ListLinFacturaView_ListLinFacturaView ( ListLinFacturaView *subform )
{
    blDebug ( "ListLinFacturaView_ListLinFacturaView", 0 );
    subform->addSubFormHeader ( "lotelfactura", BlDbField::DbVarChar, BlDbField::DbNothing, BlSubFormHeader::DbNone, _("Lote") );
    blDebug ( "END ListLinFacturaView_ListLinFacturaView", 0 );
    return 0;
}


///
/**
\param subform
\return
**/
int ListLinFacturaProveedorView_ListLinFacturaProveedorView ( ListLinFacturaProveedorView *subform )
{
    blDebug ( "ListLinFacturaProveedorView_ListLinFacturaProveedorView", 0 );
    subform->addSubFormHeader ( "lotelfacturap", BlDbField::DbVarChar, BlDbField::DbNothing, BlSubFormHeader::DbNone, _("Lote") );
    blDebug ( "END ListLinFacturaProveedorView_ListLinFacturaProveedorView", 0 );
    return 0;
}


///
/**
\return
**/
int BfSubForm_on_mui_list_editFinished ( BfSubForm * )
{
    blDebug ( "BfSubForm_on_mui_list_editFinished", 0 );

    /*

        if(subform->m_prevRow == -1 || subform->m_prevCol == -1) {
     return 0;
        } // end if

        BlDbSubFormRecord *rec = subform->lineaat(subform->m_prevRow);
        BlDbSubFormField *camp = (BlDbSubFormField *) subform->item(subform->m_prevRow, subform->m_prevCol);
        BlDbSubFormField *campact = (BlDbSubFormField *) subform->item(subform->currentRow(), subform->currentColumn());
        camp->refresh();



        if (camp->nomcampo() == "lote"+subform->tableName()) {
     QString query = "SELECT * FROM movimiento LEFT JOIN articulo ON movimiento.idarticulo = articulo.idarticulo WHERE lotemovimiento = '"+camp->valorcampo()+"'";
     BlDbRecordSet *cur = subform->mainCompany()->loadQuery(query);
     if (!cur->eof()) {
      if (campact->nomcampo() == "codigocompletoarticulo") {
       subform->situarse1(subform->m_prevRow, subform->m_prevCol);
       campact->setText(cur->valor("codigocompletoarticulo"));
      } else {
       rec->setDbValue("idarticulo", cur->valor("idarticulo"));
       rec->setDbValue("codigocompletoarticulo", cur->valor("codigocompletoarticulo"));
       rec->setDbValue("nomarticulo", cur->valor("nomarticulo"));
       rec->refresh();
      } // end if
     } // end if
     delete cur;
        } // end if

    */
    return 0;

    blDebug ( "END BfSubForm_on_mui_list_editFinished", 0 );
}
