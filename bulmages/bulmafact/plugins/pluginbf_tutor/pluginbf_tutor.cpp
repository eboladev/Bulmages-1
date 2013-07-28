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
BfBulmaFact *g_pluginbf_tutor = NULL;


///
/**
\param bges
\return
**/
int entryPoint ( BfBulmaFact *bges )
{
    BL_FUNC_DEBUG

    /// Inicializa el sistema de traducciones 'gettext'.
    setlocale ( LC_ALL, "" );
    blBindTextDomain ( "pluginbf_tutor", g_confpr->value( CONF_DIR_TRADUCCION ).toLatin1().constData() );
    g_pluginbf_tutor = bges;

    if ( bges->company()->hasTablePrivilege ( "cliente", "SELECT" ) ) {

        /// Miramos si existe un menu Ventas
        QMenu *pPluginMenu = bges->newMenu ( _("&Associats"), "menuAssociats", "menuMaestro" );

        /// Agrego un Separador
        pPluginMenu->addSeparator();

        BlAction *accionA = new BlAction ( _ ( "&Padres/Socios" ), 0 );
        accionA->setIcon ( QIcon ( QString::fromUtf8 ( ":/ImgGestionAula/icons/tutor-list.png" ) ) );
        accionA->setStatusTip ( _ ( "Padres/Socios" ) );
        accionA->setWhatsThis ( _ ( "Padres/Socios" ) );
        accionA->setObjectName("mui_actionSocio");
        pPluginMenu->addAction ( accionA );
        bges->Listados->addAction ( accionA );

        BlAction *accionB = new BlAction ( _ ( "&Nuevo padre/socio" ), 0 );
        accionB->setIcon ( QIcon ( QString::fromUtf8 ( ":/ImgGestionAula/icons/tutor-new.png" ) ) );
        accionB->setStatusTip ( _ ( "Nuevo padre/socio" ) );
        accionB->setWhatsThis ( _ ( "Nuevo padre/socio" ) );
        accionB->setObjectName("mui_actionSocioNuevo");
        pPluginMenu->addAction ( accionB );
        bges->Fichas->addAction ( accionB );
    } // end if

    
    return 0;
}


int BlAction_actionTriggered(BlAction *accion) {
    BL_FUNC_DEBUG
    BlDebug::blDebug ( Q_FUNC_INFO, 0, "PluginBf_Tutor" );
    if (accion->objectName() == "mui_actionSocio") {
        if ( g_tutoresList ) {
            g_tutoresList->hide();
            g_tutoresList->show();
        } // end if
    } // end if

    if (accion->objectName() == "mui_actionSocioNuevo") {
        TutorView * bud = new TutorView ( g_pluginbf_tutor->company(), NULL );
        g_pluginbf_tutor->company()->m_pWorkspace->addSubWindow ( bud );
        bud->show();
    } // end if

    return 0;
}


int BfCompany_createMainWindows_Post ( BfCompany *comp )
{
    BL_FUNC_DEBUG
    if ( comp->hasTablePrivilege ( "cliente", "SELECT" ) ) {
        g_tutoresList = new TutoresList ( comp, NULL );
        comp->m_pWorkspace->addSubWindow ( g_tutoresList );
        g_tutoresList->hide();
    } // end if
    
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

    if ( g_fieldName == "nomcliente" || g_fieldName == "nombretutor1") {
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
	QString query = "SELECT idalumno FROM alumno WHERE upper (COALESCE(apellido1alumno,'') || ' ' || COALESCE(apellido2alumno,'') || ' ' || COALESCE(nombrealumno,'') ) LIKE upper('" + sub->m_campoactual->text() + "%')";
        BlDbRecordSet *cur = sub->mainCompany() ->loadQuery ( query );
        if ( !cur->eof() ) {
            sub->m_registrolinea->setDbValue ( "idalumno", cur->value( "idalumno" ) );
        } // end if
        delete cur;
    } // end if
    if ( sub->m_campoactual->fieldName() == "nombretutor1" || sub->m_campoactual->fieldName() == "nomcliente") {
        BlDbRecordSet *cur = sub->mainCompany() ->loadQuery ( "SELECT idcliente, apellido1cliente, apellido2cliente, nomcliente FROM cliente WHERE upper( COALESCE(apellido1cliente,'') || ' ' || COALESCE(apellido2cliente,'') || ' ' || COALESCE(nomcliente,'') ) LIKE upper('" + sub->m_campoactual->text() + "%')");
        if ( !cur->eof() ) {
            sub->m_registrolinea->setDbValue ( "idcliente", cur->value( "idcliente" ) );
	    if (sub->existsHeader("apellido1cliente"))
	      sub->m_registrolinea->setDbValue ( "apellido1cliente", cur->value( "apellido1cliente" ) );
	    if (sub->existsHeader("apellido2cliente"))
	      sub->m_registrolinea->setDbValue ( "apellido2cliente", cur->value( "apellido2cliente" ) );
	    if (sub->existsHeader("nomcliente"))
	      sub->m_registrolinea->setDbValue ( "nomcliente", cur->value( "nomcliente" ) );
        } // end if
        delete cur;
    } // end if
    
    return 0;
}


int BlDbCompleterComboBox_textChanged (BlDbCompleterComboBox *bl) {
  BL_FUNC_DEBUG

        if ( bl->m_entrada.size() >= g_confpr->value(CONF_NUMCHAR_RELOAD_FILTRO).toInt() && (bl->m_tabla == "alumno" || bl->m_tabla=="cliente")) {
                QString cadwhere = "";
                /// Inicializamos los valores de vuelta a ""
                QMapIterator<QString, QString> i ( bl->m_valores );
                QString cador = "";
                while ( i.hasNext() ) {
                    i.next();
                    cadwhere = cadwhere + cador + " upper( COALESCE(" + i.key() + ", '') )";
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
    BL_FUNC_DEBUG
    
}

///
/**
**/
SubForm_Tutor::~SubForm_Tutor()
{
    BL_FUNC_DEBUG
    
}


///
/**
\param menu
**/
void SubForm_Tutor::s_pintaMenu ( QMenu *menu )
{
    BL_FUNC_DEBUG
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
    
}


///
/**
\param action
**/
void SubForm_Tutor::s_trataMenu ( QAction *action )
{
    BL_FUNC_DEBUG
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

    
}


///
/**
**/
void SubForm_Tutor::editarTutor ( QString idtutor )
{
    BL_FUNC_DEBUG
    BlSubForm * subf = ( BlSubForm * ) parent();
    TutorView * art = new TutorView ( ( BfCompany * ) subf->mainCompany(), 0 );
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
void SubForm_Tutor::nuevoTutor( )
{
    BL_FUNC_DEBUG
    
    BlSubForm * subf = ( BlSubForm * ) parent();
    TutorView * art = new TutorView ( ( BfCompany * ) subf->mainCompany(), 0 );
    art->setAttribute ( Qt::WA_DeleteOnClose, false );
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
	if (subf->existsHeader("cifcliente"))
	  subf->lineaact()->setDbValue ( "cifcliente", art->dbValue ( "cifcliente" ) );
	if (subf->existsHeader("nomcliente"))
	  subf->lineaact()->setDbValue ( "nomcliente", art->dbValue ( "nomcliente" ) );      
    } // end if
    delete art;  
    
    
}


///
/**
**/
void SubForm_Tutor::seleccionarTutor ( BfSubForm *sub )
{
    BL_FUNC_DEBUG
    
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
        
        return;
    } // end if

    BlDbRecordSet *cur = sub->mainCompany() ->loadQuery ( "SELECT * FROM cliente WHERE idcliente = " + idTutor );
    if ( !cur->eof() ) {
        sub->lineaact()->setDbValue ( "idcliente", idTutor );
	if (sub->existsHeader("cifcliente"))
	    sub->lineaact()->setDbValue ( "cifcliente", cur->value( "cifcliente" ) );
	if (sub->existsHeader("nomcliente"))
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
    SubForm_Tutor *subformods = new SubForm_Tutor ( sub );
    sub->QObject::connect ( sub, SIGNAL ( pintaMenu ( QMenu * ) ), subformods, SLOT ( s_pintaMenu ( QMenu * ) ) );
    sub->QObject::connect ( sub, SIGNAL ( trataMenu ( QAction * ) ), subformods, SLOT ( s_trataMenu ( QAction * ) ) );
    
    return 0;
}


/// Miramos de poner los iconos del menu de subformularios
///
/**
\param sub
\return
**/
int BlSubForm_preparaMenu ( BlSubForm *sub ) {
    BL_FUNC_DEBUG

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
    

    
    return 0;
}

