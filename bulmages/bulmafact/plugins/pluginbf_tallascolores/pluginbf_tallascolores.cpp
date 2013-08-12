/***************************************************************************
 *   Copyright (C) 2005 by Tomeu Borras Riera                              *
 *   tborras@conetxia.com                                                  *
 *   Copyright (C) 2006 by Fco. Javier M. C.                               *
 *   fcojavmc@todo-redes.com                                               *
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


#include <QtWidgets/QMessageBox>
#include <QtWidgets/QWidget>
#include <QtGui/QIcon>
#include <QtWidgets/QApplication>
#include <QtCore/QObject>

#include "pluginbf_tallascolores.h"
#include "bfcompany.h"
#include "blfunctions.h"
#include "busquedacolor.h"
#include "busquedatalla.h"
#include "listcoloresview.h"
#include "listtallasview.h"


BfBulmaFact *g_pluginbf_tallascolores = NULL;

///
/**
\param bges
**/
int entryPoint ( BfBulmaFact *bges )
{
    BL_FUNC_DEBUG

    /// El plugin necesita un parche en la base de datos para funcionar.
    bges->company()->dbPatchVersionCheck("PluginBf_TallasColores", "0.11.1-0002");
    g_pluginbf_tallascolores = bges;

    /// Inicializa el sistema de traducciones 'gettext'.
    setlocale ( LC_ALL, "" );
    blBindTextDomain ( "PluginBf_TallasColores", g_confpr->value( CONF_DIR_TRADUCCION ).toLatin1().constData() );

    QMenu *pPluginMenu = bges->newMenu(_("&Tallas y colores"), "menuTallasyColores", "menuAcerca_de");

    /// Creamos el men&uacute;.

    BlAction *accionA = new BlAction ( _ ( "&Tallas" ), 0 );
    accionA->setStatusTip ( _ ( "Tallas" ) );
    accionA->setWhatsThis ( _ ( "Tallas" ) );
    accionA->setObjectName("mui_actionTallas");    
    
    pPluginMenu->addAction ( accionA );

    BlAction *accionB = new BlAction ( _ ( "&Colores" ), 0 );
    accionB->setStatusTip ( _ ( "Colores" ) );
    accionB->setWhatsThis ( _ ( "Colores" ) );
    accionB->setObjectName("mui_actionColores");

    pPluginMenu->addAction ( accionB );

    return 0;
}


int BlAction_actionTriggered(BlAction *accion) {
    BL_FUNC_DEBUG
    if (accion->objectName() == "mui_actionTallas") {
        ListTallasView *tallas = new ListTallasView ( g_pluginbf_tallascolores->company(), 0 );
        g_pluginbf_tallascolores->company()->m_pWorkspace->addSubWindow ( tallas );
        tallas->show();
    } // end if

    if (accion->objectName() == "mui_actionColores") {
        ListColoresView *colores = new ListColoresView ( g_pluginbf_tallascolores->company(), 0 );
        g_pluginbf_tallascolores->company()->m_pWorkspace->addSubWindow ( colores );
        colores->show();
    } // end if

    return 0;
}



///
/**
\param art
\return
**/
int ArticuloView_ArticuloView ( ArticuloView *art )
{
    BL_FUNC_DEBUG

    /// Agregamos el subformulario de validaciones.
    BfSubForm *l = new BfSubForm ( art );

    /// Ponemos un delegate al subformulario para que coja los combos asignados.
    delete l->m_delegate;
    l->m_delegate = new QSubForm3BfDelegate ( l );
    l->mui_list->setItemDelegate ( l->m_delegate );

    l->setObjectName ( QString::fromUtf8 ( "laliastc" ) );
    l->setMainCompany ( art->mainCompany() );
    l->setDbTableName ( "tc_articulo_alias" );
    l->setDbFieldId ( "idarticulo" );
    l->addSubFormHeader ( "aliastc_articulo_tallacolor", BlDbField::DbVarChar, BlDbField::DbNotNull, BlSubFormHeader::DbNone , _ ( "Alias" ) );
    l->addSubFormHeader ( "idarticulo", BlDbField::DbInt, BlDbField::DbPrimaryKey, BlSubFormHeader::DbHideView | BlSubFormHeader::DbNoWrite , _ ( "Id Articulo" ) );
    l->addSubFormHeader ( "idtc_color", BlDbField::DbInt, BlDbField::DbNotNull, BlSubFormHeader::DbHideView | BlSubFormHeader::DbNoWrite, _ ( "color" ) );
    l->addSubFormHeader ( "idtc_talla", BlDbField::DbInt, BlDbField::DbNotNull, BlSubFormHeader::DbHideView | BlSubFormHeader::DbNoWrite, _ ( "Talla" ) );


    l->addSubFormHeader ( "nomtc_color", BlDbField::DbVarChar, BlDbField::DbNoSave, BlSubFormHeader::DbNone, _ ( "Nombre color" ) );
    l->addSubFormHeader ( "nomtc_talla", BlDbField::DbVarChar, BlDbField::DbNoSave, BlSubFormHeader::DbNone, _ ( "Nombre talla" ) );

    l->addSubFormHeader ( "idtc_colora", BlDbField::DbInt, BlDbField::DbDupPrimaryKey| BlDbField::DbNoSave, BlSubFormHeader::DbHideView | BlSubFormHeader::DbNoWrite,  "idtc_color"  );
    l->addSubFormHeader ( "idtc_tallaa", BlDbField::DbInt, BlDbField::DbDupPrimaryKey| BlDbField::DbNoSave, BlSubFormHeader::DbHideView | BlSubFormHeader::DbNoWrite,  "idtc_talla"  );

    
    l->setInsert ( true );
    l->setDelete ( true );
    l->setSortingEnabled ( false );
    art->dialogChanges_setExcludedObject ( l->mui_list );

    art->mui_tab->addTab ( l, _("Tallas y colores") );

    
    return 0;
}


///
/**
\param art
\return
**/
int ArticuloView_load ( ArticuloView *art )
{
    BL_FUNC_DEBUG
    BfSubForm *l = art->findChild<BfSubForm *> ( "laliastc" );
    if ( l ) {
        QString query = "SELECT *, tc_articulo_alias.idtc_talla AS idtc_tallaa, tc_articulo_alias.idtc_color AS idtc_colora FROM tc_articulo_alias LEFT JOIN tc_talla AS t1 ON tc_articulo_alias.idtc_talla = t1.idtc_talla LEFT JOIN tc_color AS t2 ON tc_articulo_alias.idtc_color = t2.idtc_color WHERE tc_articulo_alias.idarticulo = " + art->dbValue ( "idarticulo" );
        l->load ( query );
    } // end if
    
    return 0;
}



///
/**
\param art
\return
**/
int ArticuloView_guardar_post ( ArticuloView *art )
{
    BL_FUNC_DEBUG
    try {
        BfSubForm *l = art->findChild<BfSubForm *> ( "laliastc" );
        l->setColumnValue ( "idarticulo", art->dbValue ( "idarticulo" ) );
        l->save();
        return 0;
    } catch ( ... ) {
	BlDebug::blDebug ( Q_FUNC_INFO, 0, _("Error al guardar.") );
	blMsgError(_("Error al guardar."));
        return 0;
    } // end try
}




/// ============================= SUBFORM3BFDELEGATE =============================================
/// ===============================================================
///  Tratamientos del Item Delegate
/// ===============================================================


///
/**
\param parent
**/
QSubForm3BfDelegate::QSubForm3BfDelegate ( QObject *parent = 0 ) : BfSubFormDelegate ( parent )
{
    BL_FUNC_DEBUG
    
}


///
/**
**/
QSubForm3BfDelegate::~QSubForm3BfDelegate()
{
    BL_FUNC_DEBUG
    
}


///
/**
\param parent
\param option
\param index
\return
**/
QWidget *QSubForm3BfDelegate::createEditor ( QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index ) const
{
    BL_FUNC_DEBUG
    BlSubFormHeader *linea;
    linea = m_subform->headerList() ->at ( index.column() );

    if ( linea->fieldName() == "nomtc_color" ) {
        BusquedaColorDelegate * editor = new BusquedaColorDelegate ( parent );
        editor->setMainCompany ( ( BfCompany * ) m_subform->mainCompany() );
        return editor;
    } else if ( linea->fieldName() == "nomtc_talla" ) {
        BusquedaTallaDelegate * editor = new BusquedaTallaDelegate ( parent );
        editor->setMainCompany ( ( BfCompany * ) m_subform->mainCompany() );
        return editor;
    } else  {
        return BfSubFormDelegate::createEditor ( parent, option, index );
    } // end if
    
}


///
/**
\param editor
\param model
\param index
\return
**/
void QSubForm3BfDelegate::setModelData ( QWidget *editor, QAbstractItemModel *model, const QModelIndex &index ) const
{
    BL_FUNC_DEBUG

    /// Si la fila o columna pasadas son invalidas salimos.
    if ( index.column() < 0 || index.row() < 0 )
        return;

    BlSubFormHeader *linea;
    linea = m_subform->headerList() ->at ( index.column() );
    if ( linea->fieldName() == "nomtc_color" ) {
        BusquedaColorDelegate * comboBox = static_cast<BusquedaColorDelegate*> ( editor );
        QString value = comboBox->currentText();
	model->setData(index, value);
        m_subform->lineaat ( index.row() ) ->setDbValue ( "idtc_color", comboBox->id(value) );
    } else     if ( linea->fieldName() == "nomtc_talla" ) {
        BusquedaTallaDelegate * comboBox = static_cast<BusquedaTallaDelegate*> ( editor );
        QString value = comboBox->currentText();
	model->setData(index, value);
        m_subform->lineaat ( index.row() ) ->setDbValue ( "idtc_talla", comboBox->id(value) );
    } else {
        BfSubFormDelegate::setModelData ( editor, model, index );
    } // end if
    
}


///
/**
\param editor
\param index
**/
void QSubForm3BfDelegate::setEditorData ( QWidget* editor, const QModelIndex& index ) const
{
    BL_FUNC_DEBUG
    BlSubFormHeader *linea;
    linea = m_subform->headerList() ->at ( index.column() );
    if ( linea->fieldName() == "nomtc_color" ) {
        QString value = index.model() ->data ( index, Qt::DisplayRole ).toString();
        BusquedaColorDelegate *comboBox = static_cast<BusquedaColorDelegate*> ( editor );
        comboBox->setId ( value );
    } else if ( linea->fieldName() == "nomtc_talla" ) {
        QString value = index.model() ->data ( index, Qt::DisplayRole ).toString();
        BusquedaTallaDelegate *comboBox = static_cast<BusquedaTallaDelegate*> ( editor );
        comboBox->setId ( value );
    } else {
        BfSubFormDelegate::setEditorData ( editor, index );
    } // end if
    
}


int Busqueda_on_m_inputBusqueda_textChanged ( BlSearchWidget *busc )
{
    bool encontrado = false;

    if ( busc->tableName() == "articulo" ) {

        QString val = busc->m_inputBusqueda->text();

        QString SQLQuery = "SELECT * FROM tc_articulo_alias LEFT JOIN articulo ON tc_articulo_alias.idarticulo = articulo.idarticulo WHERE aliastc_articulo_tallacolor = '" + val + "'";
        BlDbRecordSet *cur = busc->mainCompany() ->loadQuery ( SQLQuery );
        if ( !cur->eof() ) {
            busc->setId ( cur->value( "idarticulo" ) );
            encontrado = true;
        }
        delete cur;

        if ( encontrado ) {
            return -1;
        } // end if
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

    subform->addSubFormHeader ( "idtc_color", BlDbField::DbInt, BlDbField::DbPrimaryKey, BlSubFormHeader::DbHideView | BlSubFormHeader::DbNoWrite, _ ( "Color" ) );
    subform->addSubFormHeader ( "idtc_talla", BlDbField::DbInt, BlDbField::DbPrimaryKey, BlSubFormHeader::DbHideView | BlSubFormHeader::DbNoWrite, _ ( "Talla" ) );
    subform->addSubFormHeader ( "nomtc_color", BlDbField::DbVarChar, BlDbField::DbNoSave, BlSubFormHeader::DbNone, _ ( "Nombre color" ) );
    subform->addSubFormHeader ( "nomtc_talla", BlDbField::DbVarChar, BlDbField::DbNoSave, BlSubFormHeader::DbNone, _ ( "Nombre talla" ) );

    delete subform->m_delegate;
    subform->m_delegate = new QSubForm3BfDelegate ( subform );
    subform->mui_list->setItemDelegate ( subform->m_delegate );

    
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
    subform->addSubFormHeader ( "idtc_color", BlDbField::DbInt, BlDbField::DbPrimaryKey, BlSubFormHeader::DbHideView | BlSubFormHeader::DbNoWrite, _ ( "Color" ) );
    subform->addSubFormHeader ( "idtc_talla", BlDbField::DbInt, BlDbField::DbPrimaryKey, BlSubFormHeader::DbHideView | BlSubFormHeader::DbNoWrite, _ ( "Talla" ) );
    subform->addSubFormHeader ( "nomtc_color", BlDbField::DbVarChar, BlDbField::DbNoSave, BlSubFormHeader::DbNone, _ ( "Nombre color" ) );
    subform->addSubFormHeader ( "nomtc_talla", BlDbField::DbVarChar, BlDbField::DbNoSave, BlSubFormHeader::DbNone, _ ( "Nombre talla" ) );

    delete subform->m_delegate;
    subform->m_delegate = new QSubForm3BfDelegate ( subform );
    subform->mui_list->setItemDelegate ( subform->m_delegate );
    
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
    subform->addSubFormHeader ( "idtc_color", BlDbField::DbInt, BlDbField::DbNothing, BlSubFormHeader::DbHideView | BlSubFormHeader::DbNoWrite, _ ( "Color" ) );
    subform->addSubFormHeader ( "idtc_talla", BlDbField::DbInt, BlDbField::DbNothing, BlSubFormHeader::DbHideView | BlSubFormHeader::DbNoWrite, _ ( "Talla" ) );
    subform->addSubFormHeader ( "nomtc_color", BlDbField::DbVarChar, BlDbField::DbNoSave, BlSubFormHeader::DbNone, _ ( "Nombre color" ) );
    subform->addSubFormHeader ( "nomtc_talla", BlDbField::DbVarChar, BlDbField::DbNoSave, BlSubFormHeader::DbNone, _ ( "Nombre talla" ) );

    delete subform->m_delegate;
    subform->m_delegate = new QSubForm3BfDelegate ( subform );
    subform->mui_list->setItemDelegate ( subform->m_delegate );
    
    return 0;
}


int BfClienteAlbaranSubForm_load ( BfClienteAlbaranSubForm *subform )
{
    QString query = "SELECT *, (pvplalbaran * cantlalbaran)::NUMERIC(12,2) AS totallalbaran FROM lalbaran LEFT JOIN articulo AS t1 ON lalbaran.idarticulo = t1.idarticulo LEFT JOIN tc_color AS t2 on t2.idtc_color = lalbaran.idtc_color LEFT JOIN tc_talla AS t3 ON t3.idtc_talla= lalbaran.idtc_talla WHERE idalbaran=" + subform->mdb_idalbaran + "   ORDER BY ordenlalbaran";
    subform->BlSubForm::load ( query );
    return -1;
}


///
/**
\param subform
\return
**/
int ListLinFacturaView_ListLinFacturaView ( ListLinFacturaView *subform )
{
    BL_FUNC_DEBUG
    subform->addSubFormHeader ( "idtc_color", BlDbField::DbInt, BlDbField::DbNothing, BlSubFormHeader::DbHideView | BlSubFormHeader::DbNoWrite, _ ( "Color" ) );
    subform->addSubFormHeader ( "idtc_talla", BlDbField::DbInt, BlDbField::DbNothing, BlSubFormHeader::DbHideView | BlSubFormHeader::DbNoWrite, _ ( "Talla" ) );
    subform->addSubFormHeader ( "nomtc_color", BlDbField::DbVarChar, BlDbField::DbNoSave, BlSubFormHeader::DbNone, _ ( "Nombre color" ) );
    subform->addSubFormHeader ( "nomtc_talla", BlDbField::DbVarChar, BlDbField::DbNoSave, BlSubFormHeader::DbNone, _ ( "Nombre talla" ) );

    delete subform->m_delegate;
    subform->m_delegate = new QSubForm3BfDelegate ( subform );
    subform->mui_list->setItemDelegate ( subform->m_delegate );
    
    return 0;
}


int ListLinFacturaView_load ( ListLinFacturaView *subform )
{
    QString query = "SELECT *, (pvplfactura * cantlfactura)::NUMERIC(12,2) AS totallfactura FROM lfactura LEFT JOIN articulo AS t1 ON lfactura.idarticulo = t1.idarticulo LEFT JOIN tc_color AS t2 on t2.idtc_color = lfactura.idtc_color LEFT JOIN tc_talla AS t3 ON t3.idtc_talla= lfactura.idtc_talla WHERE idfactura=" + subform->mdb_idfactura + "   ORDER BY ordenlfactura";
    subform->BlSubForm::load ( query );
    return -1;
}

