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

#include <QtWidgets/QAction>
#include <QtCore/QObject>
#include <QtWidgets/QMessageBox>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QTabWidget>

#include "bfcompany.h"
#include "pluginbf_trazabilidad.h"
#include "blfunctions.h"
#include "movimientosview.h"

BfBulmaFact *g_pluginbf_trazabilidad = NULL;


///
/**
\param bges
\return
**/
int entryPoint ( BfBulmaFact *bges )
{
    BL_FUNC_DEBUG

    /// El plugin necesita un parche en la base de datos para funcionar.
    bges->company()->dbPatchVersionCheck("PluginBf_Trazabilidad", "0.9.1");

    /// Inicializa el sistema de traducciones 'gettext'.
    setlocale ( LC_ALL, "" );
    blBindTextDomain ( "pluginbf_trazabilidad", g_confpr->value( CONF_DIR_TRADUCCION ).toLatin1().constData() );
    g_pluginbf_trazabilidad = bges;

    /// Creamos el men&uacute;.
    BlAction *accionA = new BlAction ( _("&Movimientos"), 0 );
    accionA->setStatusTip ( _("Ventana de Movimientos") );
    accionA->setWhatsThis ( _("Movimientos") );
    accionA->setObjectName("mui_actionMovimientos");
    accionA->setIcon ( QIcon ( QString::fromUtf8 ( ":/Images/product-family.png" ) ) );

    bges->menuMaestro->addSeparator();
    bges->menuMaestro->addAction ( accionA );


    
    return 0;
}


int BlAction_actionTriggered(BlAction *accion) {
    BL_FUNC_DEBUG
    BlDebug::blDebug ( Q_FUNC_INFO, 0, "PluginBf_Trazabilidad" );

    if (accion->objectName() == "mui_actionMovimientos") {
        MovimientosView *mov = new MovimientosView ( g_pluginbf_trazabilidad->company() );
        g_pluginbf_trazabilidad->company() ->pWorkspace() ->addSubWindow ( mov );
        mov->show();
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
    BL_FUNC_DEBUG
    subform->addSubFormHeader ( "lotelalbaranp", BlDbField::DbVarChar, BlDbField::DbNothing, BlSubFormHeader::DbNone, _("Lote") );
    
    return 0;
}

///
/**
\param subform
\return
**/
int BfClienteAlbaranSubForm_BfClienteAlbaranSubForm ( BfClienteAlbaranSubForm *subform )
{
    BL_FUNC_DEBUG
    subform->addSubFormHeader ( "lotelalbaran", BlDbField::DbVarChar, BlDbField::DbNothing, BlSubFormHeader::DbNone, _("Lote") );
    
    return 0;
}


///
/**
\param subform
\return
**/
int ListLinFacturaView_ListLinFacturaView ( ListLinFacturaView *subform )
{
    BL_FUNC_DEBUG
    subform->addSubFormHeader ( "lotelfactura", BlDbField::DbVarChar, BlDbField::DbNothing, BlSubFormHeader::DbNone, _("Lote") );
    
    return 0;
}


///
/**
\param subform
\return
**/
int ListLinFacturaProveedorView_ListLinFacturaProveedorView ( ListLinFacturaProveedorView *subform )
{
    BL_FUNC_DEBUG
    subform->addSubFormHeader ( "lotelfacturap", BlDbField::DbVarChar, BlDbField::DbNothing, BlSubFormHeader::DbNone, _("Lote") );
    
    return 0;
}


///
/**
\return
**/
int BfSubForm_on_mui_list_editFinished ( BfSubForm * )
{
    BL_FUNC_DEBUG

    /*

        if(subform->m_prevRow == -1 || subform->m_prevCol == -1) {
     return 0;
        } // end if

        BlDbSubFormRecord *rec = subform->lineaat(subform->m_prevRow);
        BlDbSubFormField *camp = (BlDbSubFormField *) subform->item(subform->m_prevRow, subform->m_prevCol);
        BlDbSubFormField *campact = (BlDbSubFormField *) subform->item(subform->currentRow(), subform->currentColumn());
        camp->refresh();



        if (camp->fieldName() == "lote"+subform->tableName()) {
     QString query = "SELECT * FROM movimiento LEFT JOIN articulo ON movimiento.idarticulo = articulo.idarticulo WHERE lotemovimiento = '"+camp->fieldValue()+"'";
     BlDbRecordSet *cur = subform->mainCompany()->loadQuery(query);
     if (!cur->eof()) {
      if (campact->fieldName() == "codigocompletoarticulo") {
       subform->situarse1(subform->m_prevRow, subform->m_prevCol);
       campact->setText(cur->value("codigocompletoarticulo"));
      } else {
       rec->setDbValue("idarticulo", cur->value("idarticulo"));
       rec->setDbValue("codigocompletoarticulo", cur->value("codigocompletoarticulo"));
       rec->setDbValue("nomarticulo", cur->value("nomarticulo"));
       rec->refresh();
      } // end if
     } // end if
     delete cur;
        } // end if

    */
    return 0;

    
}
