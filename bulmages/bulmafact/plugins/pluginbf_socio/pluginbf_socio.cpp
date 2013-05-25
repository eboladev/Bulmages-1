/***************************************************************************
 *   Copyright (C) 2009 by Xavier Rusiñol Escriu                           *
 *   russi7@gmail.com                                                      *
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

#include "pluginbf_socio.h"
#include "bfcompany.h"
#include "blfunctions.h"
#include "socioview.h"
#include "tiposocioview.h"
#include "sociolist.h"

#include "blsearchwidget.h"

SociosList *g_sociosList = NULL;

///
/**
**/
MyPlugProf::MyPlugProf()
{
    BL_FUNC_DEBUG
    
}


///
/**
**/
MyPlugProf::~MyPlugProf()
{
    BL_FUNC_DEBUG
    
}


///
/**
**/
void MyPlugProf::elslot()
{
    BL_FUNC_DEBUG
    if ( g_sociosList ) {
        g_sociosList->hide();
        g_sociosList->show();
    }// end if
    
}

///
/**
**/
void MyPlugProf::elslot1()
{
    BL_FUNC_DEBUG
    SocioView * bud = new SocioView ( ( BfCompany * ) mainCompany(), NULL );
    mainCompany() ->m_pWorkspace->addSubWindow ( bud );
    bud->show();
    
}

///
/**
**/
void MyPlugProf::elslot2()
{
    BL_FUNC_DEBUG
    TiposocioView * bud = new TiposocioView ( ( BfCompany * ) mainCompany(), NULL );
    mainCompany() ->m_pWorkspace->addSubWindow ( bud );
    bud->show();
    
}


///
/**
\param bges
**/
void MyPlugProf::inicializa ( BfBulmaFact *bges )
{
    BL_FUNC_DEBUG

    if ( bges->company()->hasTablePrivilege ( "cliente", "SELECT" ) ) {

        /// Miramos si existe un menu Docencia
        QMenu *pPluginMenu = bges->newMenu ( _("&Docencia"), "menuDocencia", "menuMaestro" );

	/// Añadimos un Separador
	pPluginMenu->addSeparator();

        /// El men&uacute; de Tarifas en la secci&oacute;n de art&iacute;culos.
        m_bges = bges;
        setMainCompany ( bges->company() );
        QAction *planCuentas = new QAction ( _ ( "&Socios" ), 0 );
        planCuentas->setIcon ( QIcon ( QString::fromUtf8 ( ":/ImgGestionAula/icons/tutor-list.png" ) ) );
        planCuentas->setStatusTip ( _ ( "Socios" ) );
        planCuentas->setWhatsThis ( _ ( "Socios" ) );
        pPluginMenu->addAction ( planCuentas );
        bges->Listados->addAction ( planCuentas );
        connect ( planCuentas, SIGNAL ( activated() ), this, SLOT ( elslot() ) );

        QAction *npago = new QAction ( _ ( "&Nuevo socio" ), 0 );
        npago->setIcon ( QIcon ( QString::fromUtf8 ( ":/ImgGestionAula/icons/tutor-new.png" ) ) );
        npago->setStatusTip ( _ ( "Nuevo socio" ) );
        npago->setWhatsThis ( _ ( "Nuevo socio" ) );
        pPluginMenu->addAction ( npago );
        bges->Fichas->addAction ( npago );
        connect ( npago, SIGNAL ( activated() ), this, SLOT ( elslot1() ) );

	QAction *tiposoc = new QAction ( _ ( "&Tipos de socio" ), 0 );
        tiposoc->setIcon ( QIcon ( QString::fromUtf8 ( ":/ImgGestionAula/icons/tutor-new.png" ) ) );
        tiposoc->setStatusTip ( _ ( "Tipos de socio" ) );
        tiposoc->setWhatsThis ( _ ( "Tipos de socio" ) );
        pPluginMenu->addAction ( tiposoc );
        bges->Fichas->addAction ( tiposoc );
        connect ( tiposoc, SIGNAL ( activated() ), this, SLOT ( elslot2() ) );
    }// end if
    
}


///
/**
\param bges
\return
**/
int entryPoint ( BfBulmaFact *bges )
{
    BL_FUNC_DEBUG

    /// El plugin necesita un parche en la base de datos para funcionar.
    bges->company()->dbPatchVersionCheck("PluginBf_Socio", "0.11.1-0001");
    
    /// Inicializa el sistema de traducciones 'gettext'.
    setlocale ( LC_ALL, "" );
    blBindTextDomain ( "pluginbf_socio", g_confpr->value( CONF_DIR_TRADUCCION ).toLatin1().constData() );

    MyPlugProf *plug = new MyPlugProf();
    plug->inicializa ( bges );
    return 0;
}


int BfCompany_createMainWindows_Post ( BfCompany *comp )
{
    BL_FUNC_DEBUG
    if ( comp->hasTablePrivilege ( "cliente", "SELECT" ) ) {
        g_sociosList = new SociosList ( comp, NULL );
        comp->m_pWorkspace->addSubWindow ( g_sociosList );
        g_sociosList->hide();
    }// end if
    
    return 0;
}


int BlSubFormDelegate_createEditor ( BlSubFormDelegate *bl )
{
    BL_FUNC_DEBUG
    int ret = 0;
    if ( g_fieldName == "nombrealumno" || g_fieldName == "nombrealumno1" ) {
        BlDbCompleterComboBox * editor = new BlDbCompleterComboBox ( g_editor );
        editor->setObjectName ( "EditNombreAlumno" );
        editor->setMainCompany ( ( BfCompany * ) bl->m_subform->mainCompany() );
	editor->m_valores["apellido1alumno"] = "";
	editor->m_valores["apellido2alumno"] = "";
        editor->m_valores["nombrealumno"] = "";
        editor->m_tabla = "alumno";
        g_plugParams =  editor;
        ret = -1;
    } // end if

    if ( g_fieldName == "nomcliente" ) {
        BlDbCompleterComboBox * editor = new BlDbCompleterComboBox ( g_editor );
        editor->setObjectName ( "EditNombreCliente" );
        editor->setMainCompany ( ( BfCompany * ) bl->m_subform->mainCompany() );
        editor->m_valores["nomcliente"] = "";
        editor->m_tabla = "cliente";
        g_plugParams =  editor;
        ret = -1;
    } // end if
    

    return ret;
}



/// Hay cosas que deberian estar en el plugin de alumno
int BlSubFormDelegate_setModelData ( BlSubFormDelegate *bl )
{
    BL_FUNC_DEBUG
    int ret = 0;
    if ( g_editor->objectName() == "EditNombreAlumno" || g_editor->objectName() == "EditNombreCliente" ) {
        BlDbCompleterComboBox * comboBox = ( BlDbCompleterComboBox * ) g_editor;
        QString value = comboBox->currentText();
        value = value.left ( value.indexOf ( ".-" ) );
        g_model->setData ( g_index, value );
        ret = -1;
    } // end if
    
    return ret;
}


int BlSubFormDelegate_setEditorData ( BlSubFormDelegate *bl )
{
    BL_FUNC_DEBUG
    int ret = 0;
    if ( g_editor->objectName() == "EditNombreAlumno" || g_editor->objectName() == "EditNombreCliente" ) {
        QString value = g_index.model() ->data ( g_index, Qt::DisplayRole ).toString();
        BlDbCompleterComboBox *comboBox = ( BlDbCompleterComboBox * ) g_editor ;
        comboBox->addItem ( value );
        ret = -1;
    } // end if
    
    return ret;
}

int BlSubForm_editFinished ( BlSubForm *sub )
{
    BL_FUNC_DEBUG
    if ( sub->m_campoactual->fieldName() == "nombrealumno1" ) {
	QString query = "SELECT idalumno FROM alumno WHERE upper (apellido1alumno || ' ' || apellido2alumno || ' ' || nombrealumno) LIKE upper('" + sub->m_campoactual->text() + "%')";
//	blMsgInfo(query);
        BlDbRecordSet *cur = sub->mainCompany() ->loadQuery ( query );
        if ( !cur->eof() ) {
            sub->m_registrolinea->setDbValue ( "idalumno", cur->value( "idalumno" ) );
        } // end if
        delete cur;
    } // end if
    if ( sub->m_campoactual->fieldName() == "nomcliente" ) {
        BlDbRecordSet *cur = sub->mainCompany() ->loadQuery ( "SELECT idcliente FROM cliente WHERE upper(nomcliente) = upper('" + sub->m_campoactual->text() + "')" );
        if ( !cur->eof() ) {
            sub->m_registrolinea->setDbValue ( "idcliente", cur->value( "idcliente" ) );
        } // end if
        delete cur;
    } // end if
    
    return 0;
}


int BlDbCompleterComboBox_textChanged (BlDbCompleterComboBox *bl) {
  BL_FUNC_DEBUG

        if ( bl->m_entrada.size() >= 3 && bl->m_tabla == "alumno") {
                QString cadwhere = "";
                /// Inicializamos los valores de vuelta a ""
                QMapIterator<QString, QString> i ( bl->m_valores );
                QString cador = "";
                while ( i.hasNext() ) {
                    i.next();
                    cadwhere = cadwhere + cador + " upper(" + i.key() + ")";
                    cador = " || ' ' ||";
                } // end while

                QString SQLQuery = "SELECT * FROM " + bl->m_tabla + " WHERE " + cadwhere + "LIKE  upper('%" + bl->m_entrada + "%')";
                bl->m_cursorcombo = bl->mainCompany() ->loadQuery ( SQLQuery );
                bl->clear();
                while ( !bl->m_cursorcombo->eof() ) {
                    QMapIterator<QString, QString> i ( bl->m_valores );
                    QString cad = "";
                    QString sep = "";
                    QString cad1 = "";
                    while ( i.hasNext() ) {
                        i.next();
                        cad = cad + sep + bl->m_cursorcombo->value( i.key() );
                        if ( sep == "" ) {
                            cad1 = i.key();
                            sep = " ";
                        } // end if
                    } // end while
                    bl->addItem ( cad , QVariant ( bl->m_cursorcombo->value( cad1 ) ) );
                    bl->m_cursorcombo->nextRecord();
                } // end while
                delete bl->m_cursorcombo;
	  return 1;
        } // end if
    return 0;
}



int BfSubForm_pressedAsterisk ( BfSubForm *sub )
{
    BL_FUNC_DEBUG

    if ( sub->m_campoactual->fieldName() != "nomcliente" ) {
        
        return 0;
    } // end if

    SociosList *tutoreslist = new SociosList ( ( BfCompany * ) sub->mainCompany(), NULL, 0, BL_SELECT_MODE );
    
    /// Esto es convertir un QWidget en un sistema modal de dialogo.
    sub->setEnabled ( false );
    tutoreslist->show();
    
    while ( !tutoreslist->isHidden() )
        g_theApp->processEvents();
        
    sub->setEnabled ( true );
    QString idCliente = tutoreslist->idcliente();
    
    delete tutoreslist;

    /// Si no tenemos un idtutor salimos ya que significa que no se ha seleccionado ninguno.
    if ( idCliente == "" ) {
        
        return 0;
    } // end if

    BlDbRecordSet *cur = sub->mainCompany() ->loadQuery ( "SELECT * FROM cliente WHERE idcliente = " + idCliente );
    if ( !cur->eof() ) {
        sub->m_registrolinea->setDbValue ( "idcliente", idCliente );
        sub->m_registrolinea->setDbValue ( "nomcliente", cur->value( "nomcliente" ));
    } // end if
    
    delete cur;
    
    

    return 0;
}








/// --------------------------------------------------------------
/// --------- Implemento la edicion de tutors -----------------
/// Con esta funcionalidad creamos menus contextuales en todos los subformularios donde
/// Aparezca el identificador de tutor como elemento y permite de forma sencilla
/// La creacion, la edicion, y la seleccion.
///


/**
\param parent
**/
MyPlugTutor1::MyPlugTutor1 ( BlSubForm *parent ) : QObject ( parent )
{
    BL_FUNC_DEBUG
    
}

///
/**
**/
MyPlugTutor1::~MyPlugTutor1()
{
    BL_FUNC_DEBUG
    
}


///
/**
\param menu
**/
void MyPlugTutor1::s_pintaMenu ( QMenu *menu )
{
    BL_FUNC_DEBUG
    BfSubForm *sub = ( BfSubForm * ) parent();
    BlSubFormHeader *header = sub->header ( "nomcliente" );
    if ( header ) {
        menu->addSeparator();
        menu->addAction ( _ ( "Nuevo socio" ) );
        QString idtutor = sub->dbValue ( "idcliente" );
        if ( idtutor != "" ) menu->addAction ( _ ( "Editar socio" ) );
        if ( ! ( header->options() & BlSubFormHeader::DbNoWrite ) )  {
            menu->addAction ( _ ( "Seleccionar socio" ) );
        } // end if
    } // end if
    
}


///
/**
\param action
**/
void MyPlugTutor1::s_trataMenu ( QAction *action )
{
    BL_FUNC_DEBUG
    BfSubForm *sub = ( BfSubForm * ) parent();
    if ( action->text() == _ ( "Editar socio" ) ) {
        QString idtutor = sub->dbValue ( "idcliente" );
        if ( idtutor != "" )
            editarTutor ( idtutor );
    } else if ( action->text() == _ ( "Seleccionar socio" ) ) {
        seleccionarTutor ( sub );
    } else if ( action->text() == _ ( "Nuevo socio" )  ) {
        nuevoTutor();
    } // end if

    
}


///
/**
**/
void MyPlugTutor1::editarTutor ( QString idtutor )
{
    BL_FUNC_DEBUG
    BlSubForm * subf = ( BlSubForm * ) parent();
    SocioView * art = new SocioView ( ( BfCompany * ) subf->mainCompany(), 0 );
    subf->mainCompany() ->m_pWorkspace->addSubWindow ( art );
    /// Si la carga no va bien entonces terminamos.
    if ( art->load ( idtutor ) ) {
        delete art;
	
        return;
    } // end if
    art->hide();
    art->show();
    
}



///
/**
**/
void MyPlugTutor1::nuevoTutor( )
{
    BL_FUNC_DEBUG
    BlSubForm * subf = ( BlSubForm * ) parent();
    SocioView * art = new SocioView ( ( BfCompany * ) subf->mainCompany(), 0 );
    subf->mainCompany() ->m_pWorkspace->addSubWindow ( art );
    art->hide();
    art->show();
    
}


///
/**
**/
void MyPlugTutor1::seleccionarTutor ( BfSubForm *sub )
{
    BL_FUNC_DEBUG

    SociosList *artlist = new SociosList ( ( BfCompany * ) sub->mainCompany(), NULL, 0, BL_SELECT_MODE );
    /// Esto es convertir un QWidget en un sistema modal de dialogo.
    sub->setEnabled ( false );
    artlist->show();
    while ( !artlist->isHidden() )
        g_theApp->processEvents();
    sub->setEnabled ( true );
    QString idTutor = artlist->idcliente();
    delete artlist;

    /// Si no tenemos un idtutor salimos ya que significa que no se ha seleccionado ninguno.
    if ( idTutor == "" ) {
        
        return;
    } // end if

    BlDbRecordSet *cur = sub->mainCompany() ->loadQuery ( "SELECT * FROM cliente WHERE idcliente = " + idTutor );
    if ( !cur->eof() ) {
        sub->lineaact()->setDbValue ( "idcliente", idTutor );
        sub->lineaact()->setDbValue ( "cifcliente", cur->value( "cifcliente" ) );
        sub->lineaact()->setDbValue ( "nomcliente", cur->value( "nomcliente" ) );
    } // end if
    delete cur;

    
}


///
/**
\param sub
\return
**/
int BlSubForm_BlSubForm_Post ( BlSubForm *sub )
{
    BL_FUNC_DEBUG
    MyPlugTutor1 *subformods = new MyPlugTutor1 ( sub );
    sub->QObject::connect ( sub, SIGNAL ( pintaMenu ( QMenu * ) ), subformods, SLOT ( s_pintaMenu ( QMenu * ) ) );
    sub->QObject::connect ( sub, SIGNAL ( trataMenu ( QAction * ) ), subformods, SLOT ( s_trataMenu ( QAction * ) ) );
    
    return 0;
}
