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

#include <QToolButton>

#include "pluginbf_tipotrabajo.h"
#include "listtipostrabajoview.h"
#include "busquedatipotrabajo.h"
#include "bfbuscararticulo.h"

BfBulmaFact *g_pluginbf_tipotrabajo = NULL;

///
/**
\param bges
\return
**/
int entryPoint ( BfBulmaFact *bges )
{
    BL_FUNC_DEBUG

    /// El plugin necesita un parche en la base de datos para funcionar.
    bges->company()->dbPatchVersionCheck("PluginBf_TipoTrabajo", "0.10.1-0001");
    g_pluginbf_tipotrabajo = bges;
    /// Inicializa el sistema de traducciones 'gettext'.
    setlocale ( LC_ALL, "" );
    blBindTextDomain ( "pluginbf_tipotrabajo", g_confpr->value( CONF_DIR_TRADUCCION ).toAscii().constData() );

    BlAction *accion = new BlAction (  _("&Tipos de trabajo"), 0 );
    accion->setStatusTip ( _("Tipos de trabajo") );
    accion->setWhatsThis ( _("Tipos de trabajo") );
    accion->setObjectName("mui_actionTipoTrabajo");
    /// A&ntilde;adimos la nueva opci&oacute;n al men&uacute; principal del programa.
    bges->menuMaestro->addAction ( accion );
                    
    return 0;
    
}


int BlAction_actionTriggered(BlAction *accion) {
    BL_FUNC_DEBUG
    BlDebug::blDebug ( Q_FUNC_INFO, 0, "PluginBf_TipoTrabajo" );

    if (accion->objectName() == "mui_actionTipoTrabajo") {
        ListTiposTrabajoView *l = new ListTiposTrabajoView ( g_pluginbf_tipotrabajo->company(), 0 );
        g_pluginbf_tipotrabajo->company()->m_pWorkspace->addSubWindow ( l );
        l->show();
    } // end if

    return 0;
}


/// Al crear la ventana de trabajadores tambien creamos un combo box para el tipo de trabajador.
/**
\param trab
\return
**/
int TrabajadorView_TrabajadorView_Post ( TrabajadorView *trab )
{
    BL_FUNC_DEBUG

    QHBoxLayout *hboxLayout160 = new QHBoxLayout();
    hboxLayout160->setSpacing ( 2 );
    hboxLayout160->setMargin ( 0 );
    hboxLayout160->setObjectName ( QString::fromUtf8 ( "hboxLayout16" ) );

    QLabel *textLabel2_9_26 = new QLabel ( trab->m_frameplugin );
    textLabel2_9_26->setObjectName ( QString::fromUtf8 ( "textLabel2_9_2" ) );
    hboxLayout160->addWidget ( textLabel2_9_26 );
    textLabel2_9_26->setText ( _("Tipo de trabajo") );

    BusquedaTipoTrabajo *tipotraba = new BusquedaTipoTrabajo ( trab->m_frameplugin );
    tipotraba->setMainCompany ( trab->mainCompany() );
    tipotraba->setId ( "" );
    tipotraba->setObjectName ( QString::fromUtf8 ( "tipotraba" ) );
    hboxLayout160->addWidget ( tipotraba );


    /// Comprobamos que exista el layout.
    QVBoxLayout *m_hboxLayout1 = trab->m_frameplugin->findChild<QVBoxLayout *> ( "hboxLayout1" );
    if ( !m_hboxLayout1 ) {
        m_hboxLayout1 = new QVBoxLayout ( trab->m_frameplugin );
        m_hboxLayout1->setSpacing ( 0 );
        m_hboxLayout1->setMargin ( 0 );
        m_hboxLayout1->setObjectName ( QString::fromUtf8 ( "hboxLayout1" ) );
    } // end if

    m_hboxLayout1->addLayout ( hboxLayout160 );

//   trab->m_frameplugin->setLayout(hboxLayout160);
    return 0;
}


///
/**
\param trab
\return
**/
int TrabajadorView_on_mui_guardar_clicked ( TrabajadorView *trab )
{

    BusquedaTipoTrabajo * l = trab->findChild<BusquedaTipoTrabajo *> ( "tipotraba" );
    QString query = "UPDATE trabajador SET ";
    query += " id = " + l->id();
    query += " WHERE idtrabajador=" + trab->mainCompany() ->sanearCadena ( trab->mdb_idtrabajador );
    trab->mainCompany() ->begin();
    trab->mainCompany() ->runQuery ( query );
    trab->mainCompany() ->commit();
    return 0;
}


///
/**
\param trab
\return
**/
int TrabajadorView_on_mui_lista_currentItemChanged_Post ( TrabajadorView *trab )
{
    BL_FUNC_DEBUG
    if (trab) {
	BusquedaTipoTrabajo * l = trab->findChild<BusquedaTipoTrabajo *> ( "tipotraba" );
	if (l) {
	    BlDbRecordSet *cur = trab->mainCompany() ->loadQuery ( "SELECT idtipotrabajo FROM trabajador WHERE idtrabajador = " + trab->mdb_idtrabajador );
	    if ( !cur->eof() ) {
		l->setId( cur->value( "idtipotrabajo" ) );
	    } // end if
	    delete cur;
	} // end if
    } // end if
    return 0;
}


///
/**
\param alm
\return
**/
int AlmacenView_AlmacenView ( AlmacenView *alm )
{
    BL_FUNC_DEBUG

    BfSubForm *form = new BfSubForm ( alm );
    delete form->m_delegate;
    form->m_delegate = new QSubForm3BfDelegate ( form );
    form->mui_list->setItemDelegate ( form->m_delegate );
    form->setObjectName ( "mui_tipostrabajo" );
    form->setMainCompany ( alm->mainCompany() );
    form->setDbTableName ( "almacentipotrabajo" );
    form->setDbFieldId ( "idalmacen" );
    form->addSubFormHeader ( "nomtipotrabajo", BlDbField::DbVarChar, BlDbField::DbNoSave , BlSubFormHeader::DbNone, _ ( "ID nombre del tipo de trabajo" ) );
    form->addSubFormHeader ( "numpers", BlDbField::DbInt, BlDbField::DbNotNull, BlSubFormHeader::DbNone, _ ( "Numero de cargos necesarios" ) );
    form->addSubFormHeader ( "idalmacen", BlDbField::DbInt, BlDbField::DbPrimaryKey, BlSubFormHeader::DbHideView | BlSubFormHeader::DbNoWrite, _ ( "ID almacen" ) );
    form->addSubFormHeader ( "idtipotrabajo", BlDbField::DbInt, BlDbField::DbNotNull, BlSubFormHeader::DbHideView | BlSubFormHeader::DbNoWrite, _ ( "ID tipo de trabajo" ) );
    form->addSubFormHeader ( "origidtipotrabajo", BlDbField::DbInt, BlDbField::DbDupPrimaryKey | BlDbField::DbNoSave, BlSubFormHeader::DbHideView, _ ( "Id tipo de trabajo" ) );

    form->setInsert ( TRUE );
    form->setDelete ( TRUE );
    form->setSortingEnabled ( FALSE );

    alm->mui_tab->addTab ( form, "Trabajadores Requeridos" );
    return 0;
}


///
/**
\param fich
\return
**/
int BlForm_load ( BlForm *fich )
{
    BlSubForm * form = fich->findChild<BlSubForm *> ( "mui_tipostrabajo" );
    if ( form )
        form->load ( "SELECT *, tipotrabajo.idtipotrabajo AS origidtipotrabajo FROM almacentipotrabajo LEFT JOIN tipotrabajo ON almacentipotrabajo.idtipotrabajo = tipotrabajo.idtipotrabajo WHERE idalmacen = " + fich->dbValue ( "idalmacen" ) );
    return 0;
}


///
/**
\param fich
\return
**/
int BlForm_guardar_Post ( BlForm *fich )
{
    BL_FUNC_DEBUG
    BlSubForm * form = fich->findChild<BlSubForm *> ( "mui_tipostrabajo" );
    if ( form ) {
        form->setColumnValue ( "idalmacen", fich->dbValue ( "idalmacen" ) );
        form->save();
    }
    
    return 0;
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

    if ( linea->fieldName() == "nomtipotrabajo" ) {
        BusquedaTipoTrabajoDelegate * editor = new BusquedaTipoTrabajoDelegate ( parent );
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
    if ( linea->fieldName() == "nomtipotrabajo" ) {
        BusquedaTipoTrabajoDelegate * comboBox = static_cast<BusquedaTipoTrabajoDelegate*> ( editor );
        QString value = comboBox->currentText();
        model->setData ( index, value );
        m_subform->lineaat ( index.row() ) ->setDbValue ( "idtipotrabajo", comboBox->id() );
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
    if ( linea->fieldName() == "nomtipotrabajo" ) {
        QString value = index.model() ->data ( index, Qt::DisplayRole ).toString();
        BusquedaTipoTrabajoDelegate *comboBox = static_cast<BusquedaTipoTrabajoDelegate*> ( editor );
        comboBox->setId ( value );
    } else {
        BfSubFormDelegate::setEditorData ( editor, index );
    } // end if
    
}


