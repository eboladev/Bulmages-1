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
#include "promocionarclaseview.h"
#include "listcuotasporactividadview.h"


AlumnosList *g_alumnosList = NULL;
BfBulmaFact *g_pluginbf_alumno = NULL;


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
    blBindTextDomain ( "pluginbf_alumno", g_confpr->value( CONF_DIR_TRADUCCION ).toLatin1().constData() );
    g_pluginbf_alumno = bges;


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

        BlAction *accionB = new BlAction ( _ ( "&Nuevo alumno" ), 0 );
        accionB->setIcon ( QIcon ( QString::fromUtf8 ( ":/ImgGestionAula/icons/alumno-new.png" ) ) );
        accionB->setStatusTip ( _ ( "Nuevo alumno" ) );
        accionB->setWhatsThis ( _ ( "Nuevo alumno" ) );
        accionB->setObjectName("mui_actionAlumnoNuevo");
        pPluginMenu->addAction ( accionB );
        bges->Fichas->addAction ( accionB );

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

        BlAction *accionH = new BlAction ( _ ( "&Descuentos por actividades contratadas" ), 0 );
        accionH->setIcon ( QIcon ( QString::fromUtf8 ( ":/ImgGestionAula/icons/mix_money_32.png" ) ) );
        accionH->setStatusTip ( _ ( "Descuentos por actividades contratadas" ) );
        accionH->setWhatsThis ( _ ( "Descuentos por actividades contratadas" ) );
        accionH->setObjectName("mui_actionDescuentosCuotas");
        pGesMenu->addAction ( accionH );
        bges->Fichas->addAction ( accionH );
	
        /// Agrego un separador
        pGesMenu->addSeparator();

        BlAction *accionD = new BlAction ( _ ( "&Clases" ), 0 );
        accionD->setStatusTip ( _ ( "Clases" ) );
        accionD->setWhatsThis ( _ ( "Clases" ) );
        accionD->setObjectName("mui_actionClases");
        accionD->setIcon ( QIcon ( QString::fromUtf8 ( ":/ImgGestionAula/icons/warehouse.png" ) ) );
        pGesMenu->addAction ( accionD );
        bges->Fichas->addAction ( accionD );

	BlAction *accionE = new BlAction ( _ ( "&Promocion de Clases" ), 0 );
        accionE->setStatusTip ( _ ( "Promocion de Clases" ) );
        accionE->setWhatsThis ( _ ( "Promocion de Clases" ) );
        accionE->setObjectName("mui_actionPromocionClases");
        accionE->setIcon ( QIcon ( QString::fromUtf8 ( ":/Images/i_promocion.png" ) ) );
        pGesMenu->addAction ( accionE );
        bges->Fichas->addAction ( accionE );

    } // end if

    
    
    return 0;
}


int BlAction_actionTriggered(BlAction *accion) {
    BL_FUNC_DEBUG
    if (accion->objectName() == "mui_actionAlumnos") {
        if ( g_alumnosList ) {
            g_alumnosList->hide();
            g_alumnosList->show();
        } // end if
    } // end if

    if (accion->objectName() == "mui_actionAlumnoNuevo") {
        AlumnoView * bud = new AlumnoView ( g_pluginbf_alumno->company(), NULL );
        g_pluginbf_alumno->company()->m_pWorkspace->addSubWindow ( bud );
        bud->show();
    } // end if

    if (accion->objectName() == "mui_actionAlumnoCuotas") {
        ListCuotasPorAlumnoView * bud = new ListCuotasPorAlumnoView ( g_pluginbf_alumno->company(), NULL );
        g_pluginbf_alumno->company()->m_pWorkspace->addSubWindow ( bud );
        bud->show();
    } // end if 
    
    if (accion->objectName() == "mui_actionClases") {
        ClaseView * bud = new ClaseView ( g_pluginbf_alumno->company(), NULL );
        g_pluginbf_alumno->company()->m_pWorkspace->addSubWindow ( bud );
        bud->show();
    } // end if
    
    if (accion->objectName() == "mui_actionPromocionClases") {
        PromocionarClaseView * bud = new PromocionarClaseView ( g_pluginbf_alumno->company(), NULL );
        g_pluginbf_alumno->company()->m_pWorkspace->addSubWindow ( bud );
        bud->show();
    } // end if

    if (accion->objectName() == "mui_actionDescuentosCuotas") {
        ListCuotasPorActividadView * bud = new ListCuotasPorActividadView ( g_pluginbf_alumno->company(), NULL );
        g_pluginbf_alumno->company()->m_pWorkspace->addSubWindow ( bud );
        bud->show();
    } // end if

    return 0;
}



int BfCompany_createMainWindows_Post ( BfCompany *comp )
{
    BL_FUNC_DEBUG

    if ( comp->hasTablePrivilege ( "alumno", "SELECT" ) ) {
        g_alumnosList = new AlumnosList ( comp, NULL );
        comp->m_pWorkspace->addSubWindow ( g_alumnosList );
        g_alumnosList->hide();
    } // end if
    
    

    return 0;
}

int BfSubForm_pressedAsterisk ( BfSubForm *sub )
{
    BL_FUNC_DEBUG

    if ( sub->m_campoactual->fieldName() != "nombrealumno1" ) {
        
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

    /// Si no tenemos un idalumno salimos ya que significa que no se ha seleccionado ninguno.
    if ( idAlumno == "" ) {
        
        return 0;
    } // end if

    BlDbRecordSet *cur = sub->mainCompany() ->loadQuery ( "SELECT * FROM alumno WHERE idalumno = " + idAlumno );
    if ( !cur->eof() ) {
        sub->m_registrolinea->setDbValue ( "idalumno", idAlumno );
        sub->m_registrolinea->setDbValue ( "nombrealumno1", cur->value( "apellido1alumno") + " " + cur->value("apellido2alumno") + ", " + cur->value( "nombrealumno" ));
    } // end if
    
    delete cur;
    
    

    return 0;
}



int Busqueda_on_mui_buscar_clicked ( BlSearchWidget *busq )
{
    BL_FUNC_DEBUG

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
    BL_FUNC_DEBUG
    
}

///
/**
**/
SubForm_Alumno::~SubForm_Alumno()
{
    BL_FUNC_DEBUG
    
}


///
/**
\param menu
**/
void SubForm_Alumno::s_pintaMenu ( QMenu *menu )
{
    BL_FUNC_DEBUG
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
    
}


///
/**
\param action
**/
void SubForm_Alumno::s_trataMenu ( QAction *action )
{
    BL_FUNC_DEBUG
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

    
}


///
/**
**/
void SubForm_Alumno::editarAlumno ( QString idalumno )
{
    BL_FUNC_DEBUG
    BlSubForm * subf = ( BlSubForm * ) parent();
    AlumnoView * art = new AlumnoView ( ( BfCompany * ) subf->mainCompany(), 0 );
    subf->mainCompany() ->m_pWorkspace->addSubWindow ( art );
    /// Si la carga no va bien entonces terminamos.
    if ( art->load ( idalumno ) ) {
        delete art;
	
        return;
    } // end if
    art->hide();
    art->show();
    
}



///
/**
**/
void SubForm_Alumno::nuevoAlumno( )
{
    BL_FUNC_DEBUG
    BlSubForm * subf = ( BlSubForm * ) parent();
    AlumnoView * art = new AlumnoView ( ( BfCompany * ) subf->mainCompany(), 0 );
    art->setAttribute ( Qt::WA_DeleteOnClose, false );
    
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
    
    
}


///
/**
**/
void SubForm_Alumno::seleccionarAlumno ( BfSubForm *sub )
{
    BL_FUNC_DEBUG
    
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
        
        return;
    } // end if

    BlDbRecordSet *cur = sub->mainCompany() ->loadQuery ( "SELECT * FROM alumno WHERE idalumno = " + idAlumno );
    if ( !cur->eof() ) {
        sub->lineaact()->setDbValue ( "idalumno", idAlumno );
        sub->lineaact()->setDbValue ( "nombrealumno1", cur->value( "nombrealumno" ) );
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
    SubForm_Alumno *subformods = new SubForm_Alumno ( sub );
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
          sel->setStatusTip ( _("Nuevo alumno") );
          sel->setToolTip ( _("Nuevo alumno") );
          sel->setMinimumSize ( QSize ( 18, 18 ) );
          sel->setIcon ( QIcon ( ":/ImgGestionAula/icons/alumno.png" ) );
          sel->setIconSize ( QSize ( 18, 18 ) );    
          m_hboxLayout1->addWidget ( sel );
          sel->connect (sel, SIGNAL(released()), subformods, SLOT(nuevoAlumno()));
        
          QToolButton *sel1 = new QToolButton ( sub->mui_menusubform );
          sel1->setStatusTip ( _("Seleccionar alumno") );
          sel1->setToolTip ( _("Seleccionar alumno") );
          sel1->setMinimumSize ( QSize ( 18, 18 ) );
          sel1->setIcon ( QIcon ( ":/ImgGestionAula/icons/alumno-list.png" ) );
          sel1->setIconSize ( QSize ( 18, 18 ) );
          m_hboxLayout1->addWidget ( sel1 );
          sel1->connect (sel1, SIGNAL(released()), subformods, SLOT(seleccionarAlumno()));
        } // end if
    } // end if

    return 0;
}


/// Apertura de un elemento controlado a partir del parametro g_plugParams tabla_identificador
int Plugin_open(BfCompany * comp) {
  BL_FUNC_DEBUG
  QString cad = *((QString*)g_plugParams);
  QStringList args = cad.split("_");
  if (args[0] == "actividad") {
	AlumnoView * bud = new AlumnoView ( comp, 0 );
        comp->m_pWorkspace->addSubWindow ( bud );
	QString id =  args[1];
	bud->load(id);
        bud->show();

  } // end if
  return 0;
}

