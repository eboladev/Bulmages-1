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

#include "plugintipostrabajo.h"
#include "listtipostrabajoview.h"
#include "tiptrab.h"
#include "busquedatipotrabajo.h"
#include "busquedaarticulo.h"


///
/**
**/
mytiptrab::mytiptrab()
{
    _depura ( "mytiptrab::mytiptrab", 0 );
    _depura ( "END mytiptrab::mytiptrab", 0 );
}


///
/**
**/
mytiptrab::~mytiptrab()
{
    _depura ( "mytiptrab::~mytiptrab", 0 );
    _depura ( "END mytiptrab::~mytiptrab", 0 );
}


///
/**
**/
void mytiptrab::elslot()
{
    _depura ( "mytiptrab::elslot", 0 );
    ListTiposTrabajoView *l = new ListTiposTrabajoView ( ( BfCompany * ) m_bulmafact->getcompany(), 0 );
    m_bulmafact->workspace() ->addWindow ( l );
    l->show();
    _depura ( "END mytiptrab::elslot", 0 );
}


///
/**
\param bges
**/
void mytiptrab::inicializa ( Bulmafact *bges )
{
    /// Creamos el men&uacute;.
    m_bulmafact = bges;
    QAction *accion = new QAction ( "&Tipos de Trabajo", 0 );
    accion->setStatusTip ( "Tipos de Trabajo" );
    accion->setWhatsThis ( "Tipos de Trabajo" );
    connect ( accion, SIGNAL ( activated() ), this, SLOT ( elslot() ) );
    /// A&ntilde;adimos la nueva opci&oacute;n al men&uacute; principal del programa.
    bges->menuMaestro->addAction ( accion );
}


///
/**
\param bges
\return
**/
int entryPoint ( Bulmafact *bges )
{
    _depura ( "Estoy dentro del plugin de tipos de trabajo", 0 );

    /// Inicializa el sistema de traducciones 'gettext'.
    setlocale(LC_ALL, "");
    bindtextdomain ("plugintipostrabajo", confpr->valor(CONF_DIR_TRADUCCION).toAscii().constData());

    mytiptrab *plug = new mytiptrab();
    plug->inicializa ( bges );
    return 0;
}


/// Al crear la ventana de trabajadores tambien creamos un combo box para el tipo de trabajador.
/**
\param trab
\return
**/
int TrabajadorView_TrabajadorView_Post ( TrabajadorView *trab )
{
    _depura ( "TrabajadorView_TrabajadorView_Post", 0 );

    QHBoxLayout *hboxLayout160 = new QHBoxLayout();
    hboxLayout160->setSpacing ( 2 );
    hboxLayout160->setMargin ( 0 );
    hboxLayout160->setObjectName ( QString::fromUtf8 ( "hboxLayout16" ) );

    QLabel *textLabel2_9_26 = new QLabel ( trab->m_frameplugin );
    textLabel2_9_26->setObjectName ( QString::fromUtf8 ( "textLabel2_9_2" ) );
    hboxLayout160->addWidget ( textLabel2_9_26 );
    textLabel2_9_26->setText ( "Tipo Trabajo" );

    BusquedaTipoTrabajo *tipotraba = new BusquedaTipoTrabajo ( trab->m_frameplugin );
    tipotraba->setMainCompany ( trab->mainCompany() );
    tipotraba->setidtipotrabajo ( "" );
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
    query += " idtipotrabajo = " + l->idtipotrabajo();
    query += " WHERE idtrabajador=" + trab->mainCompany() ->sanearCadena ( trab->idtrabajador() );
    trab->mainCompany() ->begin();
    trab->mainCompany() ->ejecuta ( query );
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
    BusquedaTipoTrabajo * l = trab->findChild<BusquedaTipoTrabajo *> ( "tipotraba" );

    BlDbRecordSet *cur = trab->mainCompany() ->loadQuery ( "SELECT idtipotrabajo FROM trabajador WHERE idtrabajador = " + trab->idtrabajador() );
    if ( !cur->eof() ) {
        l->setidtipotrabajo ( cur->valor ( "idtipotrabajo" ) );
    }
    delete cur;
    return 0;
}


///
/**
\param alm
\return
**/
int AlmacenView_AlmacenView ( AlmacenView *alm )
{
    _depura ( "esxtoy en la clase almacen", 0 );

    BfSubForm *form = new BfSubForm ( alm );
    delete form->m_delegate;
    form->m_delegate = new QSubForm3BfDelegate ( form );
    form->mui_list->setItemDelegate ( form->m_delegate );
    form->setObjectName ( "mui_tipostrabajo" );
    form->setMainCompany ( alm->mainCompany() );
    form->setDbTableName ( "almacentipotrabajo" );
    form->setDbFieldId ( "idalmacen" );
    form->addSubFormHeader ( "nomtipotrabajo", BlDbField::DBvarchar, BlDbField::DBNoSave , BlSubFormHeader::DBNone, _( "ID nombre del tipo de trabajo" ) );
    form->addSubFormHeader ( "numpers", BlDbField::DBint, BlDbField::DBNotNull, BlSubFormHeader::DBNone, _( "Numero de cargos necesarios" ) );
    form->addSubFormHeader ( "idalmacen", BlDbField::DBint, BlDbField::DBPrimaryKey, BlSubFormHeader::DBNoView | BlSubFormHeader::DBNoWrite, _( "ID almacen" ) );
    form->addSubFormHeader ( "idtipotrabajo", BlDbField::DBint, BlDbField::DBNotNull, BlSubFormHeader::DBNoView | BlSubFormHeader::DBNoWrite, _( "ID tipo de trabajo" ) );
    form->addSubFormHeader ( "origidtipotrabajo", BlDbField::DBint, BlDbField::DBDupPrimaryKey | BlDbField::DBNoSave, BlSubFormHeader::DBNoView, _("Id tipo de trabajo") );

    form->setinsercion ( TRUE );
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
int BlForm_cargar ( BlForm *fich )
{
    BlSubForm * form = fich->findChild<BlSubForm *> ( "mui_tipostrabajo" );
    if ( form )
        form->cargar ( "SELECT *, tipotrabajo.idtipotrabajo AS origidtipotrabajo FROM almacentipotrabajo LEFT JOIN tipotrabajo ON almacentipotrabajo.idtipotrabajo = tipotrabajo.idtipotrabajo WHERE idalmacen = " + fich->DBvalue ( "idalmacen" ) );
    return 0;
}


///
/**
\param fich
\return
**/
int BlForm_guardar_Post ( BlForm *fich )
{
    _depura ( "BlForm_guardar_Post", 0 );
    BlSubForm * form = fich->findChild<BlSubForm *> ( "mui_tipostrabajo" );
    if ( form ) {
        form->setColumnValue ( "idalmacen", fich->DBvalue ( "idalmacen" ) );
        form->guardar();
    }
    _depura ( "END BlForm_guardar_Post", 0 );
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
    _depura ( "QSubForm3BfDelegate::QSubForm3BfDelegate", 0 );
    _depura ( "END QSubForm3BfDelegate::QSubForm3BfDelegate", 0 );
}


///
/**
**/
QSubForm3BfDelegate::~QSubForm3BfDelegate()
{
    _depura ( "QSubForm3BfDelegate::~QSubForm3BfDelegate", 0 );
    _depura ( "END QSubForm3BfDelegate::~QSubForm3BfDelegate", 0 );
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
    _depura ( "QSubForm3BfDelegate::createEditor", 0 );
    BlSubFormHeader *linea;
    linea = m_subform->cabecera() ->at ( index.column() );

    if ( linea->nomcampo() == "nomtipotrabajo" ) {
        BusquedaTipoTrabajoDelegate * editor = new BusquedaTipoTrabajoDelegate ( parent );
        editor->setMainCompany ( ( BfCompany * ) m_subform->mainCompany() );
        return editor;
    } else  {
        return BfSubFormDelegate::createEditor ( parent, option, index );
    } // end if
    _depura ( "END QSubForm3BfDelegate::createEditor", 0 );
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
    _depura ( "QSubForm3BfDelegate::setModelData", 0 );

    /// Si la fila o columna pasadas son invalidas salimos.
    if ( index.column() < 0 || index.row() < 0 )
        return;

    BlSubFormHeader *linea;
    linea = m_subform->cabecera() ->at ( index.column() );
    if ( linea->nomcampo() == "nomtipotrabajo" ) {
        BusquedaTipoTrabajoDelegate * comboBox = static_cast<BusquedaTipoTrabajoDelegate*> ( editor );
        QString value = comboBox->currentText();
        model->setData ( index, value );
        m_subform->lineaat ( index.row() ) ->setDbValue ( "idtipotrabajo", comboBox->id() );
    } else {
        BfSubFormDelegate::setModelData ( editor, model, index );
    } // end if
    _depura ( "END QSubForm3BfDelegate::setModelData", 0 );
}


///
/**
\param editor
\param index
**/
void QSubForm3BfDelegate::setEditorData ( QWidget* editor, const QModelIndex& index ) const
{
    _depura ( "QSubForm3BfDelegate::setEditorData", 0 );
    BlSubFormHeader *linea;
    linea = m_subform->cabecera() ->at ( index.column() );
    if ( linea->nomcampo() == "nomtipotrabajo" ) {
        QString value = index.model() ->data ( index, Qt::DisplayRole ).toString();
        BusquedaTipoTrabajoDelegate *comboBox = static_cast<BusquedaTipoTrabajoDelegate*> ( editor );
        comboBox->set ( value );
    } else {
        BfSubFormDelegate::setEditorData ( editor, index );
    } // end if
    _depura ( "END QSubForm3BfDelegate::setEditorData", 0 );
}


