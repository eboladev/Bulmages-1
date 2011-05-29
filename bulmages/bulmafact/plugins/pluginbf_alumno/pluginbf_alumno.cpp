/***************************************************************************
 *   Copyright (C) 2009 by Tomeu Borras Riera                              *
 *   tborras@conetxia.com                                                  *
 *                                                                         *
 *   Copyright (C) 2009 by Arturo Martin Llado                             *
 *   amartin@conetxia.com                                                  *
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

#include "pluginbf_alumno.h"
#include "bfcompany.h"
#include "blfunctions.h"
#include "alumnoview.h"
#include "alumnoslist.h"
#include "listcuotasporalumnoview.h"
#include "claseview.h"

AlumnosList *g_alumnosList = NULL;
BfBulmaFact *g_bges = NULL;


///
/**
\param bges
\return
**/
int entryPoint ( BfBulmaFact *bges )
{
    blDebug ( "Punto de entrada del plugin de alumnos\n", 0 );

    /// Inicializa el sistema de traducciones 'gettext'.
    setlocale ( LC_ALL, "" );
    blBindTextDomain ( "pluginbf_alumno", g_confpr->value( CONF_DIR_TRADUCCION ).toAscii().constData() );
    g_bges = bges;


    if ( bges->company()->hasTablePrivilege ( "alumno", "SELECT" ) ) {

        /// Miramos si existe un menu Docencia
        QMenu *pPluginMenu = bges->newMenu ( _("&Associats"), "menuAssociats", "menuMaestro" );

        /// Agrego un separador
        pPluginMenu->addSeparator();

        BlAction *accionA = new BlAction ( _ ( "&Alumnos" ), 0 );
        accionA->setIcon ( QIcon ( QString::fromUtf8 ( ":/ImgGestionAula/icons/alumno-list.png" ) ) );
        accionA->setStatusTip ( _ ( "Alumnos" ) );
        accionA->setWhatsThis ( _ ( "Alumnos" ) );
        accionA->setObjectName("mui_actionAlumnos");
        pPluginMenu->addAction ( accionA );
        bges->Listados->addAction ( accionA );
        //connect ( accionA, SIGNAL ( activated() ), this, SLOT ( elslot() ) );

        BlAction *accionB = new BlAction ( _ ( "&Nuevo alumno" ), 0 );
        accionB->setIcon ( QIcon ( QString::fromUtf8 ( ":/ImgGestionAula/icons/alumno-new.png" ) ) );
        accionB->setStatusTip ( _ ( "Nuevo alumno" ) );
        accionB->setWhatsThis ( _ ( "Nuevo alumno" ) );
        accionB->setObjectName("mui_actionAlumnoNuevo");
        pPluginMenu->addAction ( accionB );
        bges->Fichas->addAction ( accionB );
        //connect ( accionB, SIGNAL ( activated() ), this, SLOT ( elslot1() ) );

        /// Miramos si existe un menu Docencia
        QMenu *pGesMenu = bges->newMenu ( _("&Associats"), "menuAssociats", "menuMaestro" );

        /// Agrego un separador
        pGesMenu->addSeparator();

        BlAction *accionC = new BlAction ( _ ( "&Cuotas Socio" ), 0 );
        accionC->setIcon ( QIcon ( QString::fromUtf8 ( ":/ImgGestionAula/icons/mix_money_32.png" ) ) );
        accionC->setStatusTip ( _ ( "Cuotas por alumno" ) );
        accionC->setWhatsThis ( _ ( "Cuotas por alumno" ) );
        accionC->setObjectName("mui_actionAlumnoCuotas");
        pGesMenu->addAction ( accionC );
        bges->Fichas->addAction ( accionC );
        //connect ( accionC, SIGNAL ( activated() ), this, SLOT ( elslot2() ) );

        /// Agrego un separador
        pGesMenu->addSeparator();

        BlAction *accionD = new BlAction ( _ ( "&Clases" ), 0 );
        accionD->setStatusTip ( _ ( "Clases" ) );
        accionD->setWhatsThis ( _ ( "Clases" ) );
        accionD->setObjectName("mui_actionClases");
        accionD->setIcon ( QIcon ( QString::fromUtf8 ( ":/ImgGestionAula/icons/warehouse.png" ) ) );
        pGesMenu->addAction ( accionD );
        bges->Fichas->addAction ( accionD );
        //connect ( accionD, SIGNAL ( activated() ), this, SLOT ( elslot3() ) );

    } // end if

    blDebug ( "END Punto de entrada del plugin de alumnos\n", 0 );
    
    return 0;
}


int BlAction_triggered(BlAction *accion) {
    if (accion->objectName() == "mui_actionAlumnos") {
        blDebug ( "PluginBf_Alumno::BlAction_triggered::mui_actionAlumnos\n", 0 );
        if ( g_alumnosList ) {
            g_alumnosList->hide();
            g_alumnosList->show();
        } // end if
        blDebug ( "END PluginBf_Alumno::BlAction_triggered::mui_actionAlumnos\n", 0 );
    } // end if

    if (accion->objectName() == "mui_actionAlumnoNuevo") {
        blDebug ( "PluginBf_Alumno::BlAction_triggered::mui_actionAlumnoNuevo\n", 0 );
        AlumnoView * bud = new AlumnoView ( ( BfCompany * ) g_bges->company(), NULL );
        g_bges->company()->m_pWorkspace->addSubWindow ( bud );
        bud->show();
        blDebug ( "END PluginBf_Alumno::BlAction_triggered::mui_actionAlumnoNuevo\n", 0 );    
    } // end if

    if (accion->objectName() == "mui_actionAlumnoCuotas") {
        blDebug ( "PluginBf_Alumno::BlAction_triggered::mui_actionAlumnoCuotas\n", 0 );
        ListCuotasPorAlumnoView * bud = new ListCuotasPorAlumnoView ( ( BfCompany * ) g_bges->company(), NULL );
        g_bges->company()->m_pWorkspace->addSubWindow ( bud );
        bud->show();
        blDebug ( "END PluginBf_Alumno::BlAction_triggered::mui_actionAlumnoCuotas\n", 0 );
    } // end if 
    
    if (accion->objectName() == "mui_actionClases") {
        blDebug ( "PluginBf_Alumno::BlAction_triggered::mui_actionClases\n", 0 );
        ClaseView * bud = new ClaseView ( ( BfCompany * ) g_bges->company(), NULL );
        g_bges->company()->m_pWorkspace->addSubWindow ( bud );
        bud->show();
        blDebug ( "END PluginBf_Alumno::BlAction_triggered::mui_actionClases\n", 0 );
    } // end if

    return 0;
}



int BfCompany_createMainWindows_Post ( BfCompany *comp )
{
    blDebug ( "BfCompany_createMainWindows_Post", 0 );

    if ( comp->hasTablePrivilege ( "alumno", "SELECT" ) ) {
        g_alumnosList = new AlumnosList ( comp, NULL );
        comp->m_pWorkspace->addSubWindow ( g_alumnosList );
        g_alumnosList->hide();
    } // end if
    
    blDebug ( "END BfCompany_createMainWindows_Post", 0 );

    return 0;
}

int BfSubForm_pressedAsterisk ( BfSubForm *sub )
{
    blDebug ( "BfSubForm_pressedAsterisk" );

    if ( sub->m_campoactual->nomcampo() != "nombrealumno1" ) {
        blDebug ( "END BfSubForm::pressedAsterisk", 0 );
        return 0;
    } // end if

    AlumnosList *alumnoslist = new AlumnosList ( ( BfCompany * ) sub->mainCompany(), NULL, 0, BL_SELECT_MODE );
    
    /// Esto es convertir un QWidget en un sistema modal de dialogo.
    sub->setEnabled ( false );
    alumnoslist->show();
    
    while ( !alumnoslist->isHidden() )
        g_theApp->processEvents();
        
    sub->setEnabled ( true );
    QString idAlumno = alumnoslist->idalumno();
    
    delete alumnoslist;

    /// Si no tenemos un idarticulo salimos ya que significa que no se ha seleccionado ninguno.
    if ( idAlumno == "" ) {
        blDebug ( "END BfSubForm::pressedAsterisk", 0 );
        return 0;
    } // end if

    BlDbRecordSet *cur = sub->mainCompany() ->loadQuery ( "SELECT * FROM alumno WHERE idalumno = " + idAlumno );
    if ( !cur->eof() ) {
        sub->m_registrolinea->setDbValue ( "idalumno", idAlumno );
        sub->m_registrolinea->setDbValue ( "nombrealumno1", cur->value( "apellido1alumno") + " " + cur->value("apellido2alumno") + ", " + cur->value( "nombrealumno" ));
    } // end if
    
    delete cur;
    
    blDebug ( "END BfSubForm_pressedAsterisk" );

    return 0;
}



int Busqueda_on_mui_buscar_clicked ( BlSearchWidget *busq )
{
    blDebug ( "Busqueda_on_mui_buscar_clicked", 0 );

    if ( busq->tableName() == "alumno" ) {

        QDialog *diag = new QDialog ( 0 );
        diag->setModal ( true );
        diag->setGeometry ( QRect ( 0, 0, 750, 550 ) );
        blCenterOnScreen ( diag );

        AlumnosList *clients = new AlumnosList ( ( BfCompany * ) busq->mainCompany(), diag, 0, BL_SELECT_MODE );
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
        
        if ( clients->idalumno() != "" ) {
            busq->setId ( clients->idalumno() );
        } // end if
        
        delete diag;

        return 1;
        
    } // end if
    
    blDebug ( "END Busqueda_on_mui_buscar_clicked", 0 );
    
    return 0;

}




/// --------------------------------------------------------------
/// --------- Implemento la edicion de alumnos -----------------
/// Con esta funcionalidad creamos menus contextuales en todos los subformularios donde
/// Aparezca el identificador de alumno como elemento y permite de forma sencilla
/// La creacion, la edicion, y la seleccion.
///


/**
\param parent
**/
SubForm_Alumno::SubForm_Alumno ( BlSubForm *parent ) : QObject ( parent )
{
    blDebug ( "SubForm_Alumno::SubForm_Alumno", 0 );
    blDebug ( "END SubForm_Alumno::SubForm_Alumno", 0 );
}

///
/**
**/
SubForm_Alumno::~SubForm_Alumno()
{
    blDebug ( "SubForm_Alumno::~SubForm_Alumno", 0 );
    blDebug ( "END SubForm_Alumno::~SubForm_Alumno", 0 );
}


///
/**
\param menu
**/
void SubForm_Alumno::s_pintaMenu ( QMenu *menu )
{
    blDebug ( "SubForm_Alumno::s_pintaMenu", 0 );
    BfSubForm *sub = ( BfSubForm * ) parent();
    BlSubFormHeader *header = sub->header ( "nombrealumno1" );
    if (!header) 
	header = sub->header ( "nombrealumno" );
    if ( header ) {
        menu->addSeparator();
        menu->addAction ( QIcon ( ":/ImgGestionAula/icons/alumno.png" ), _ ( "Nuevo alumno" ) );
        QString idalumno = sub->dbValue ( "idalumno" );
        if ( idalumno != "" ) menu->addAction ( QIcon ( ":/ImgGestionAula/icons/alumno.png" ), _ ( "Editar alumno" ) );
        if ( ! ( header->options() & BlSubFormHeader::DbNoWrite ) )  {
            menu->addAction ( QIcon ( ":/ImgGestionAula/icons/alumno-list.png" ), _ ( "Seleccionar alumno" ) );
        } // end if
    } // end if
    blDebug ( "END SubForm_Alumno::s_pintaMenu", 0 );
}


///
/**
\param action
**/
void SubForm_Alumno::s_trataMenu ( QAction *action )
{
    blDebug ( "SubForm_Alumno::s_trataMenu", 0 );
    BfSubForm *sub = ( BfSubForm * ) parent();
    if ( action->text() == _ ( "Editar alumno" ) ) {
        QString idalumno = sub->dbValue ( "idalumno" );
        if ( idalumno != "" )
            editarAlumno ( idalumno );
    } else if ( action->text() == _ ( "Seleccionar alumno" ) ) {
        seleccionarAlumno ( sub );
    } else if ( action->text() == _ ( "Nuevo alumno" )  ) {
        nuevoAlumno();
    } // end if

    blDebug ( "END SubForm_Alumno::s_trataMenu", 0 );
}


///
/**
**/
void SubForm_Alumno::editarAlumno ( QString idalumno )
{
    blDebug ( "SubForm_Alumno::editarAlumno", 0 );
    BlSubForm * subf = ( BlSubForm * ) parent();
    AlumnoView * art = new AlumnoView ( ( BfCompany * ) subf->mainCompany(), 0 );
    subf->mainCompany() ->m_pWorkspace->addSubWindow ( art );
    /// Si la carga no va bien entonces terminamos.
    if ( art->cargar ( idalumno ) ) {
        delete art;
        blDebug ( "END AlumnosList::editar", 0, "Carga Erronea" );
        return;
    } // end if
    art->hide();
    art->show();
    blDebug ( "END SubForm_Alumno::editarAlumno", 0 );
}



///
/**
**/
void SubForm_Alumno::nuevoAlumno( )
{
    blDebug ( "SubForm_Alumno::editarAlumno", 0 );
    BlSubForm * subf = ( BlSubForm * ) parent();
    AlumnoView * art = new AlumnoView ( ( BfCompany * ) subf->mainCompany(), 0 );
    art->setAttribute ( Qt::WA_DeleteOnClose, FALSE );
    
    subf->mainCompany() ->m_pWorkspace->addSubWindow ( art );
    subf->setEnabled(false);
    art->hide();
    art->show();
    
    while ( !art->isHidden() )
        g_theApp->processEvents();
    subf->setEnabled ( true );
    QString idAlumno = art->dbValue("idalumno");
    if (idAlumno != "") {
        subf->lineaact()->setDbValue ( "idalumno", idAlumno );
        subf->lineaact()->setDbValue ( "nombrealumno1", art->dbValue ( "nombrealumno" ) );      
    } // end if
    delete art;    
    
    blDebug ( "END SubForm_Alumno::editarAlumno", 0 );
}


///
/**
**/
void SubForm_Alumno::seleccionarAlumno ( BfSubForm *sub )
{
    blDebug ( "SubForm_Alumno::editarAlumno", 0 );
    
    if (!sub) sub= (BfSubForm *) parent();
    
    AlumnosList *artlist = new AlumnosList ( ( BfCompany * ) sub->mainCompany(), NULL, 0, BL_SELECT_MODE );
    /// Esto es convertir un QWidget en un sistema modal de dialogo.
    sub->setEnabled ( false );
    artlist->show();
    while ( !artlist->isHidden() )
        g_theApp->processEvents();
    sub->setEnabled ( true );
    QString idAlumno = artlist->idalumno();
    delete artlist;

    /// Si no tenemos un idalumno salimos ya que significa que no se ha seleccionado ninguno.
    if ( idAlumno == "" ) {
        blDebug ( "END BfSubForm::pressedAsterisk", 0 );
        return;
    } // end if

    BlDbRecordSet *cur = sub->mainCompany() ->loadQuery ( "SELECT * FROM alumno WHERE idalumno = " + idAlumno );
    if ( !cur->eof() ) {
        sub->lineaact()->setDbValue ( "idalumno", idAlumno );
        sub->lineaact()->setDbValue ( "nombrealumno1", cur->value( "nombrealumno" ) );
    } // end if
    delete cur;

    blDebug ( "END SubForm_Alumno::editarAlumno", 0 );
}


///
/**
\param sub
\return
**/
int BlSubForm_BlSubForm_Post ( BlSubForm *sub )
{
    blDebug ( "BlSubForm_BlSubForm_Post", 0 );
    SubForm_Alumno *subformods = new SubForm_Alumno ( sub );
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

    BlSubFormHeader *header = sub->header ( "nombrealumno1" );
    if (!header) 
        header = sub->header ( "nombrealumno" );
    if ( header ) {
        SubForm_Alumno *subformods = new SubForm_Alumno ( sub );
        
        QHBoxLayout *m_hboxLayout1 = sub->mui_menusubform->findChild<QHBoxLayout *> ( "hboxLayout1" );
        if ( !m_hboxLayout1 ) {
            m_hboxLayout1 = new QHBoxLayout ( sub->mui_menusubform );
            m_hboxLayout1->setSpacing (0 );
            m_hboxLayout1->setMargin ( 0 );
            m_hboxLayout1->setObjectName ( QString::fromUtf8 ( "hboxLayout1" ) );
        } // end if
        
        if ( ! ( header->options() & BlSubFormHeader::DbNoWrite ) )  {
          QToolButton *sel = new QToolButton ( sub->mui_menusubform );
          sel->setStatusTip ( "Nuevo Alumno" );
          sel->setToolTip ( "Nuevo Alumno" );
          sel->setMinimumSize ( QSize ( 18, 18 ) );
          sel->setIcon ( QIcon ( ":/ImgGestionAula/icons/alumno.png" ) );
          sel->setIconSize ( QSize ( 18, 18 ) );    
          m_hboxLayout1->addWidget ( sel );
          sel->connect (sel, SIGNAL(released()), subformods, SLOT(nuevoAlumno()));
        
          QToolButton *sel1 = new QToolButton ( sub->mui_menusubform );
          sel1->setStatusTip ( "Seleccionar Alumno" );
          sel1->setToolTip ( "Seleccionar Alumno" );
          sel1->setMinimumSize ( QSize ( 18, 18 ) );
          sel1->setIcon ( QIcon ( ":/ImgGestionAula/icons/alumno-list.png" ) );
          sel1->setIconSize ( QSize ( 18, 18 ) );    
          m_hboxLayout1->addWidget ( sel1 );
          sel1->connect (sel1, SIGNAL(released()), subformods, SLOT(seleccionarAlumno()));
        } // end if
    } // end if
    

    blDebug ( "END BlSubForm_preparaMenu", 0 );
    return 0;
}




