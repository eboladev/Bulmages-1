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

#include "pluginbf_tutor.h"
#include "bfcompany.h"
#include "blfunctions.h"
#include "tutorview.h"
#include "tutoreslist.h"
#include "blsearchwidget.h"

TutoresList *g_tutoresList = NULL;

///
/**
**/
PluginBf_Tutor::PluginBf_Tutor()
{
    blDebug ( "PluginBf_Tutor::PluginBf_Tutor", 0 );
    blDebug ( "END PluginBf_Tutor::PluginBf_Tutor", 0 );
}


///
/**
**/
PluginBf_Tutor::~PluginBf_Tutor()
{
    blDebug ( "PluginBf_Tutor::~PluginBf_Tutor", 0 );
    blDebug ( "END PluginBf_Tutor::~PluginBf_Tutor", 0 );
}


///
/**
**/
void PluginBf_Tutor::elslot()
{
    blDebug ( "PluginBf_Tutor::elslot", 0 );
    if ( g_tutoresList ) {
        g_tutoresList->hide();
        g_tutoresList->show();
    }// end if
    blDebug ( "END PluginBf_Tutor::elslot", 0 );
}

///
/**
**/
void PluginBf_Tutor::elslot1()
{
    blDebug ( "PluginBf_Tutor::elslot1", 0 );
    TutorView * bud = new TutorView ( ( BfCompany * ) mainCompany(), NULL );
    mainCompany() ->m_pWorkspace->addSubWindow ( bud );
    bud->show();
    blDebug ( "END PluginBf_Tutor::elslot1", 0 );
}



///
/**
\param bges
**/
void PluginBf_Tutor::inicializa ( BfBulmaFact *bges )
{
    blDebug ( "PluginBf_Tutor::inicializa", 0 );

    if ( bges->company()->hasTablePrivilege ( "cliente", "SELECT" ) ) {

        /// Miramos si existe un menu Ventas
        QMenu *pPluginMenu = bges->newMenu ( _("&Associats"), "menuAssociats", "menuMaestro" );

	/// Agrego un Separador
	pPluginMenu->addSeparator();

        /// El men&uacute; de Tarifas en la secci&oacute;n de art&iacute;culos.
        m_bges = bges;
        setMainCompany ( bges->company() );
        QAction *planCuentas = new QAction ( _ ( "&Padres/Socios" ), 0 );
        planCuentas->setIcon ( QIcon ( QString::fromUtf8 ( ":/ImgGestionAula/icons/tutor-list.png" ) ) );
        planCuentas->setStatusTip ( _ ( "Padres/Socios" ) );
        planCuentas->setWhatsThis ( _ ( "Padres/Socios" ) );
        pPluginMenu->addAction ( planCuentas );
        bges->Listados->addAction ( planCuentas );
        connect ( planCuentas, SIGNAL ( activated() ), this, SLOT ( elslot() ) );

        QAction *npago = new QAction ( _ ( "&Nuevo padre/socio" ), 0 );
        npago->setIcon ( QIcon ( QString::fromUtf8 ( ":/ImgGestionAula/icons/tutor-new.png" ) ) );
        npago->setStatusTip ( _ ( "Nuevo padre/socio" ) );
        npago->setWhatsThis ( _ ( "Nuevo padre/socio" ) );
        pPluginMenu->addAction ( npago );
        bges->Fichas->addAction ( npago );
        connect ( npago, SIGNAL ( activated() ), this, SLOT ( elslot1() ) );
    }// end if
    blDebug ( "END PluginBf_Tutor::inicializa", 0 );
}


///
/**
\param bges
\return
**/
int entryPoint ( BfBulmaFact *bges )
{
    blDebug ( "Punto de entrada del plugin de tutores\n", 0 );

    /// Inicializa el sistema de traducciones 'gettext'.
    setlocale ( LC_ALL, "" );
    blBindTextDomain ( "pluginbf_tutor", g_confpr->valor ( CONF_DIR_TRADUCCION ).toAscii().constData() );

    PluginBf_Tutor *plug = new PluginBf_Tutor();
    plug->inicializa ( bges );
    return 0;
}


int BfCompany_createMainWindows_Post ( BfCompany *comp )
{
    blDebug ( "pluginbf_tutor::BfCompany_createMainWindows_Post", 0 );
    if ( comp->hasTablePrivilege ( "cliente", "SELECT" ) ) {
        g_tutoresList = new TutoresList ( comp, NULL );
        comp->m_pWorkspace->addSubWindow ( g_tutoresList );
        g_tutoresList->hide();
    }// end if
    blDebug ( "END pluginbf_tutor::BfCompany_createMainWindows_Post", 0 );
    return 0;
}


int BlSubFormDelegate_createEditor ( BlSubFormDelegate *bl )
{
    blDebug ( "pluginbf_tutor::BlSubFormDelegate_createEditor", 0 );
    int ret = 0;
    if ( g_nomcampo == "nombrealumno" || g_nomcampo == "nombrealumno1" ) {
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

    if ( g_nomcampo == "nomcliente" ) {
        BlDbCompleterComboBox * editor = new BlDbCompleterComboBox ( g_editor );
        editor->setObjectName ( "EditNombreCliente" );
        editor->setMainCompany ( ( BfCompany * ) bl->m_subform->mainCompany() );
	editor->m_valores["apellido1cliente"] = "";
	editor->m_valores["apellido2cliente"] = "";
        editor->m_valores["nomcliente"] = "";
        editor->m_tabla = "cliente";
        g_plugParams =  editor;
        ret = -1;
    } // end if
    blDebug ( "END pluginbf_tutor::BlSubFormDelegate_createEditor", 0 );

    return ret;
}



/// Hay cosas que deberian estar en el plugin de alumno
int BlSubFormDelegate_setModelData ( BlSubFormDelegate *bl )
{
    blDebug ( "pluginbf_tutor::BlSubFormDelegate_setModelData", 0 );
    int ret = 0;
    if ( g_editor->objectName() == "EditNombreAlumno" || g_editor->objectName() == "EditNombreCliente" ) {
        BlDbCompleterComboBox * comboBox = ( BlDbCompleterComboBox * ) g_editor;
        QString value = comboBox->currentText();
        value = value.left ( value.indexOf ( ".-" ) );
        g_model->setData ( g_index, value );
        ret = -1;
    } // end if
    blDebug ( "END pluginbf_tutor::BlSubFormDelegate_setModelData", 0 );
    return ret;
}


int BlSubFormDelegate_setEditorData ( BlSubFormDelegate *bl )
{
    blDebug ( "pluginbf_tutor::BlSubFormDelegate_setEditorData", 0 );
    int ret = 0;
    if ( g_editor->objectName() == "EditNombreAlumno" || g_editor->objectName() == "EditNombreCliente" ) {
        QString value = g_index.model() ->data ( g_index, Qt::DisplayRole ).toString();
        BlDbCompleterComboBox *comboBox = ( BlDbCompleterComboBox * ) g_editor ;
        comboBox->addItem ( value );
        ret = -1;
    } // end if
    blDebug ( "END pluginbf_tutor::BlSubFormDelegate_setEditorData", 0 );
    return ret;
}

int BlSubForm_editFinished ( BlSubForm *sub )
{
    blDebug ( "pluginbf_tutor::BlSubForm_editFinished", 0 );
    if ( sub->m_campoactual->nomcampo() == "nombrealumno1" ) {
	QString query = "SELECT idalumno FROM alumno WHERE upper (apellido1alumno || ' ' || apellido2alumno || ' ' || nombrealumno) LIKE upper('" + sub->m_campoactual->text() + "%')";
//	blMsgInfo(query);
        BlDbRecordSet *cur = sub->mainCompany() ->loadQuery ( query );
        if ( !cur->eof() ) {
            sub->m_registrolinea->setDbValue ( "idalumno", cur->valor ( "idalumno" ) );
        } // end if
        delete cur;
    } // end if
    if ( sub->m_campoactual->nomcampo() == "nomcliente" ) {
        BlDbRecordSet *cur = sub->mainCompany() ->loadQuery ( "SELECT idcliente, apellido1cliente, apellido2cliente, nomcliente FROM cliente WHERE upper(apellido1cliente || ' ' || apellido2cliente || ' ' || nomcliente) LIKE upper('" + sub->m_campoactual->text() + "%')");
        if ( !cur->eof() ) {
            sub->m_registrolinea->setDbValue ( "idcliente", cur->valor ( "idcliente" ) );
	    if (sub->existsHeader("apellido1cliente"))
	      sub->m_registrolinea->setDbValue ( "apellido1cliente", cur->valor ( "apellido1cliente" ) );
	    if (sub->existsHeader("apellido2cliente"))
            sub->m_registrolinea->setDbValue ( "apellido2cliente", cur->valor ( "apellido2cliente" ) );
            sub->m_registrolinea->setDbValue ( "nomcliente", cur->valor ( "nomcliente" ) );
        } // end if
        delete cur;
    } // end if
    blDebug ( "END pluginbf_tutor::BlSubForm_editFinished", 0 );
    return 0;
}


int BlDbCompleterComboBox_textChanged (BlDbCompleterComboBox *bl) {
  blDebug("BlDbCompleterComboBox_textChanged", 0);

        if ( bl->m_entrada.size() >= 3 && (bl->m_tabla == "alumno" || bl->m_tabla=="cliente")) {
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
                        cad = cad + sep + bl->m_cursorcombo->valor ( i.key() );
                        if ( sep == "" ) {
                            cad1 = i.key();
                            sep = " ";
                        } // end if
                    } // end while
                    bl->addItem ( cad , QVariant ( bl->m_cursorcombo->valor ( cad1 ) ) );
                    bl->m_cursorcombo->nextRecord();
                } // end while
                delete bl->m_cursorcombo;

  blDebug("END BlDbCompleterComboBox_textChanged", 0);

	  return 1;
        } // end if
  blDebug("END BlDbCompleterComboBox_textChanged", 0);

    return 0;
}



int BfSubForm_pressedAsterisk ( BfSubForm *sub )
{
    blDebug ( "BfSubForm_pressedAsterisk" );

    if ( sub->m_campoactual->nomcampo() != "nomcliente" ) {
        blDebug ( "END BfSubForm::pressedAsterisk", 0 );
        return 0;
    } // end if

    TutoresList *tutoreslist = new TutoresList ( ( BfCompany * ) sub->mainCompany(), NULL, 0, BL_SELECT_MODE );
    
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
        blDebug ( "END BfSubForm::pressedAsterisk", 0 );
        return 0;
    } // end if

    BlDbRecordSet *cur = sub->mainCompany() ->loadQuery ( "SELECT * FROM cliente WHERE idcliente = " + idCliente );
    if ( !cur->eof() ) {
        sub->m_registrolinea->setDbValue ( "idcliente", idCliente );
        sub->m_registrolinea->setDbValue ( "nomcliente", cur->valor ( "nomcliente" ));
    } // end if
    
    delete cur;
    
    blDebug ( "END BfSubForm_pressedAsterisk" );

    return 0;
}


int Busqueda_on_mui_buscar_clicked ( BlSearchWidget *busq )
{
    if ( busq->tableName() == "cliente" ) {

        QDialog *diag = new QDialog ( 0 );
        diag->setModal ( true );
        diag->setGeometry ( QRect ( 0, 0, 750, 550 ) );
        blCenterOnScreen ( diag );

        TutoresList *clients = new TutoresList ( ( BfCompany * ) busq->mainCompany(), diag, 0, BL_SELECT_MODE );
        busq->connect ( clients, SIGNAL ( selected ( QString ) ), diag, SLOT ( accept() ) );

        /// Creamos un layout donde estara el contenido de la ventana y la ajustamos al QDialog
        /// para que sea redimensionable y aparezca el titulo de la ventana.
        QHBoxLayout *layout = new QHBoxLayout;
        layout->addWidget ( clients );
        layout->setMargin ( 0 );
        layout->setSpacing ( 0 );
        diag->setLayout ( layout );
        diag->setWindowTitle ( clients->windowTitle() );

        diag->exec();

        if ( clients->idcliente() != "" ) {
            busq->setId ( clients->idcliente() );
        } // end if

        delete diag;
        return 1;
    } // end if

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
SubForm_Tutor::SubForm_Tutor ( BlSubForm *parent ) : QObject ( parent )
{
    blDebug ( "SubForm_Tutor::SubForm_Tutor", 0 );
    blDebug ( "END SubForm_Tutor::SubForm_Tutor", 0 );
}

///
/**
**/
SubForm_Tutor::~SubForm_Tutor()
{
    blDebug ( "SubForm_Tutor::~SubForm_Tutor", 0 );
    blDebug ( "END SubForm_Tutor::~SubForm_Tutor", 0 );
}


///
/**
\param menu
**/
void SubForm_Tutor::s_pintaMenu ( QMenu *menu )
{
    blDebug ( "SubForm_Tutor::s_pintaMenu", 0 );
    BfSubForm *sub = ( BfSubForm * ) parent();
    BlSubFormHeader *header = sub->header ( "cifcliente" );
    if (!header)
      header = sub->header ( "nomcliente" );
    if ( header ) {
        menu->addSeparator();
        menu->addAction (QIcon ( QString::fromUtf8 ( ":/ImgGestionAula/icons/tutor.png" ) ),  _ ( "Nuevo tutor" ) );
        QString idtutor = sub->dbValue ( "idcliente" );
        if ( idtutor != "" ) menu->addAction (QIcon ( QString::fromUtf8 ( ":/ImgGestionAula/icons/tutor.png" ) ),  _ ( "Editar tutor" ) );
        if ( ! ( header->options() & BlSubFormHeader::DbNoWrite ) )  {
            menu->addAction ( QIcon ( QString::fromUtf8 ( ":/ImgGestionAula/icons/tutor-list.png" ) ), _ ( "Seleccionar tutor" ) );
        } // end if
    } // end if
    blDebug ( "END SubForm_Tutor::s_pintaMenu", 0 );
}


///
/**
\param action
**/
void SubForm_Tutor::s_trataMenu ( QAction *action )
{
    blDebug ( "SubForm_Tutor::s_trataMenu", 0 );
    BfSubForm *sub = ( BfSubForm * ) parent();
    if ( action->text() == _ ( "Editar tutor" ) ) {
        QString idtutor = sub->dbValue ( "idcliente" );
        if ( idtutor != "" )
            editarTutor ( idtutor );
    } else if ( action->text() == _ ( "Seleccionar tutor" ) ) {
        seleccionarTutor ( sub );
    } else if ( action->text() == _ ( "Nuevo tutor" )  ) {
        nuevoTutor();
    } // end if

    blDebug ( "END SubForm_Tutor::s_trataMenu", 0 );
}


///
/**
**/
void SubForm_Tutor::editarTutor ( QString idtutor )
{
    blDebug ( "SubForm_Tutor::editarTutor", 0 );
    BlSubForm * subf = ( BlSubForm * ) parent();
    TutorView * art = new TutorView ( ( BfCompany * ) subf->mainCompany(), 0 );
    subf->mainCompany() ->m_pWorkspace->addSubWindow ( art );
    /// Si la carga no va bien entonces terminamos.
    if ( art->cargar ( idtutor ) ) {
        delete art;
        blDebug ( "END SubForm_Tutor::editarTutor", 0, "Carga Erronea" );
        return;
    } // end if
    art->hide();
    art->show();
    blDebug ( "END SubForm_Tutor::editarTutor", 0 );
}



///
/**
**/
void SubForm_Tutor::nuevoTutor( )
{
    blDebug ( "SubForm_Tutor::nuevoTutor", 0 );
    
    BlSubForm * subf = ( BlSubForm * ) parent();
    TutorView * art = new TutorView ( ( BfCompany * ) subf->mainCompany(), 0 );
    subf->mainCompany() ->m_pWorkspace->addSubWindow ( art );
    subf->setEnabled(false);
    art->hide();
    art->show();
    
    while ( !art->isHidden() )
        g_theApp->processEvents();
    subf->setEnabled ( true );
    QString idCliente = art->dbValue("idcliente");
    if (idCliente != "") {
        subf->lineaact()->setDbValue ( "idcliente", idCliente );
	subf->lineaact()->setDbValue ( "cifcliente", art->dbValue ( "cifcliente" ) );
        subf->lineaact()->setDbValue ( "nomcliente", art->dbValue ( "nomcliente" ) );      
    } // end if
    delete art;  
    
    blDebug ( "END SubForm_Tutor::nuevoTutor", 0 );
}


///
/**
**/
void SubForm_Tutor::seleccionarTutor ( BfSubForm *sub )
{
    blDebug ( "SubForm_Tutor::editarTutor", 0 );
    
    if (!sub) sub= (BfSubForm *) parent();
    
    TutoresList *artlist = new TutoresList ( ( BfCompany * ) sub->mainCompany(), NULL, 0, BL_SELECT_MODE );
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
        blDebug ( "END BfSubForm::pressedAsterisk", 0 );
        return;
    } // end if

    BlDbRecordSet *cur = sub->mainCompany() ->loadQuery ( "SELECT * FROM cliente WHERE idcliente = " + idTutor );
    if ( !cur->eof() ) {
        sub->lineaact()->setDbValue ( "idcliente", idTutor );
        sub->lineaact()->setDbValue ( "cifcliente", cur->valor ( "cifcliente" ) );
        sub->lineaact()->setDbValue ( "nomcliente", cur->valor ( "nomcliente" ) );
    } // end if
    delete cur;

    blDebug ( "END SubForm_Tutor::editarTutor", 0 );
}


///
/**
\param sub
\return
**/
int BlSubForm_BlSubForm_Post ( BlSubForm *sub )
{
    blDebug ( "BlSubForm_BlSubForm_Post", 0 );
    SubForm_Tutor *subformods = new SubForm_Tutor ( sub );
    sub->QObject::connect ( sub, SIGNAL ( pintaMenu ( QMenu * ) ), subformods, SLOT ( s_pintaMenu ( QMenu * ) ) );
    sub->QObject::connect ( sub, SIGNAL ( trataMenu ( QAction * ) ), subformods, SLOT ( s_trataMenu ( QAction * ) ) );
    blDebug ( "END BlSubForm_BlSubForm_Post", 0 );
    return 0;
}


/// Miramos de poner los iconos del menu de subformularios
///
/**
\param sub
\return
**/
int BlSubForm_preparaMenu ( BlSubForm *sub ) {
    blDebug ( "BlSubForm_preparaMenu", 0 );

    BlSubFormHeader *header = sub->header ( "cifcliente" );
    if (!header)
      header = sub->header ( "nomcliente" );
    if ( header ) {
        SubForm_Tutor *subformods = new SubForm_Tutor ( sub );
        
        QHBoxLayout *m_hboxLayout1 = sub->mui_menusubform->findChild<QHBoxLayout *> ( "hboxLayout1" );
        if ( !m_hboxLayout1 ) {
            m_hboxLayout1 = new QHBoxLayout ( sub->mui_menusubform );
            m_hboxLayout1->setSpacing (0 );
            m_hboxLayout1->setMargin ( 0 );
            m_hboxLayout1->setObjectName ( QString::fromUtf8 ( "hboxLayout1" ) );
        } // end if
        
        if ( ! ( header->options() & BlSubFormHeader::DbNoWrite ) )  {
          QToolButton *sel = new QToolButton ( sub->mui_menusubform );
          sel->setStatusTip ( _("Nuevo Tutor") );
          sel->setToolTip ( _("Nuevo Tutor") );
          sel->setMinimumSize ( QSize ( 18, 18 ) );
          sel->setIcon ( QIcon ( QString::fromUtf8 ( ":/ImgGestionAula/icons/tutor-new.png" ) ));
          sel->setIconSize ( QSize ( 18, 18 ) );    
          m_hboxLayout1->addWidget ( sel );
          sel->connect (sel, SIGNAL(released()), subformods, SLOT(nuevoTutor()));
        
          QToolButton *sel1 = new QToolButton ( sub->mui_menusubform );
          sel1->setStatusTip ( _("Seleccionar Tutor") );
          sel1->setToolTip ( _("Seleccionar Tutor") );
          sel1->setMinimumSize ( QSize ( 18, 18 ) );
          sel1->setIcon ( QIcon ( QString::fromUtf8 ( ":/ImgGestionAula/icons/tutor-list.png" ) ) );
          sel1->setIconSize ( QSize ( 18, 18 ) );
          m_hboxLayout1->addWidget ( sel1 );
          sel1->connect (sel1, SIGNAL(released()), subformods, SLOT(seleccionarTutor()));
        } // end if
    } // end if
    

    blDebug ( "END BlSubForm_preparaMenu", 0 );
    return 0;
}

