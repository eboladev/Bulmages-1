/***************************************************************************
 *   Copyright (C) 2004 by Tomeu Borras Riera                              *
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

#include <QMessageBox>
#include <QCheckBox>
#include <QFile>
#include <QCheckBox>
#include <QTextStream>
#include <QMenu>

#include "blconfiguration.h"
#include "alumnoslist.h"
#include "bfcompany.h"
#include "alumnoview.h"
#include "blfunctions.h"


/** Inicializa todos los componentes.
    Mete la ventana en el workSpace.
    Este constructor no es completo, debe inicializarse con setcompany para que la clase pueda operar.
*/
AlumnosList::AlumnosList ( QWidget *parent, Qt::WFlags flag, edmode editmodo )
        : BlFormList ( NULL, parent, flag, editmodo )
{
    BL_FUNC_DEBUG
    setupUi ( this );
    /// Disparamos los plugins.
    int res = g_plugins->run ( "AlumnosList_AlumnosList", this );
    if ( res != 0 )
        return;
    mdb_idalumno = "";
    
    setSubForm ( mui_list );
    hideBusqueda();
    /// Llamamos a los scripts
    blScript(this);
    
}


/** Inicializa todos los componentes.
    Hace una presentacion inicial.
    Mete la ventana en el workSpace.
*/
AlumnosList::AlumnosList ( BfCompany *comp, QWidget *parent, Qt::WFlags flag, edmode editmodo )
        : BlFormList ( comp, parent, flag, editmodo )
{
    BL_FUNC_DEBUG
    setupUi ( this );
    /// Disparamos los plugins.
    int res = g_plugins->run ( "AlumnosList_AlumnosList", this );
    if ( res != 0 )
        return;

    mui_list->setMainCompany ( comp );


    setSubForm ( mui_list );
    
    /// Cargamos los filtros guardados.
    cargaFiltrosXML();
    
    presentar();
    mdb_idalumno = "";

    if ( editMode() ) {
        mainCompany() ->insertWindow ( windowTitle(), this );
    } else {
        setWindowTitle ( _ ( "Selector de alumnos" ) );
        mui_editar->setHidden ( TRUE );
	/// Vamos a permitir la insercion de alumnos desde un listado en QDialog generando una insercion de alumnos tambien en QDialog
//        mui_crear->setHidden ( TRUE );
        mui_borrar->setHidden ( TRUE );
        mui_imprimir->setHidden ( TRUE );
    } // end if

    mui_idclase->setMainCompany ( mainCompany() );
    mui_idclase->setQuery ( "SELECT * FROM clase ORDER BY nomclase" );
    mui_idclase->setTableName ( "clase" );
    mui_idclase->setFieldId ( "idclase" );
    mui_idclase->m_valores["nomclase"] = "";
    mui_idclase->setAllowNull ( TRUE );
    mui_idclase->setId ( "" );

    hideBusqueda();
    /// Hacemos el tratamiento de los permisos que desabilita botones en caso de no haber suficientes permisos.
    trataPermisos ( "alumno" );
    /// Llamamos a los scripts
    blScript(this);
    
}


/** No requiere acciones especiales en el destructor.
*/
AlumnosList::~AlumnosList()
{
    BL_FUNC_DEBUG
    
}


/** Hace la carag del listado.
    PAra ello genera los SELECTS con ayuda de generaFiltro y los pasa al SubFormulario para que los presente.
    Tambien hace un select de calculo de totales y lo presenta en el textEdit correspondiente.
*/
void AlumnosList::presentar()
{
    BL_FUNC_DEBUG
    if ( mainCompany() != NULL ) {
        mui_list->load ( "SELECT * FROM alumno WHERE 1 = 1 " + generaFiltro() );
    } // end if
    
}


/** Metodo auxiliar que genera la clausula WHERE del listado con las opciones de filtrado especificadas.
*/
const QString AlumnosList::generaFiltro()
{
    BL_FUNC_DEBUG
    QString filtro = "";
    /// Hacemos el filtrado like del campo m_filtro
    filtro += mui_list->likeFilterSQL(m_filtro->text());
    

    if (mui_idclase->id() != "") {
      filtro += " AND idclase = " + mui_idclase->id();
    } // end if
    
    
    return ( filtro );
}



/** SLOT que responde a la pulsacion del boton de crear en el listado.
    Instancia la clase AlumnoView, y la presenta.
*/
void AlumnosList::crear()
{
    BL_FUNC_DEBUG
    
    if (selectMode()) {
	/// El modo consulta funciona algo diferente
        QDialog *diag = new QDialog ( 0 );
        diag->setModal ( true );
        diag->setGeometry ( QRect ( 0, 0, 750, 550 ) );
        blCenterOnScreen ( diag );

	AlumnoView *bud = new AlumnoView ( ( BfCompany * ) mainCompany(), 0 );
        /// Creamos un layout donde estara el contenido de la ventana y la ajustamos al QDialog
        bud->connect ( bud, SIGNAL ( destroyed ( QObject * ) ), diag, SLOT ( accept() ) );

        /// para que sea redimensionable y aparezca el titulo de la ventana.
        QHBoxLayout *layout = new QHBoxLayout;
        layout->addWidget ( bud );
        layout->setMargin ( 0 );
        layout->setSpacing ( 0 );
        diag->setLayout ( layout );
        diag->setWindowTitle ( bud->windowTitle() );

	bud->show();
	bud->pintar();

	QString idalumnoold = "";
	BlDbRecordSet *curold = mainCompany()->loadQuery("SELECT max(idalumno) AS id FROM alumno");
	if( !curold->eof()) {
		      idalumnoold = curold->value("id");
	} // end if
	delete curold;
	
        diag->exec();      
      
	BlDbRecordSet *cur = mainCompany()->loadQuery("SELECT max(idalumno) AS id FROM alumno");
	if( !cur->eof()) {
		      if (idalumnoold != cur->value("id")) {
			mdb_idalumno = cur->value("id");
			close();
			emit ( selected ( mdb_idalumno ) );
		      } // end if
	} // end if
	delete cur;
	
    } else {
      AlumnoView *bud = new AlumnoView ( ( BfCompany * ) mainCompany(), 0 );
      mainCompany() ->m_pWorkspace->addSubWindow ( bud );
      bud->show();
      bud->pintar();
    } // end if

    
}


/** La impresion de listados esta completamente delegada a la clase BlSubForm
*/
void AlumnosList::imprimir()
{
    BL_FUNC_DEBUG
    printPDF ( _ ( "Alumnos" ) );
    
}


/** SLOT que responde a la pulsacion del boton borrar.
    Comprueba que exista un elemento seleccionado.
    Instancia la clase AlumnoView, la inicializa y le lanza el metodo borrar.
    Esta es la forma correcta de implementar un borrado a partir de un listado
    ya que de esta forma si existen plugins que alteren el borrado tambien seran invocados.
*/
void AlumnosList::remove()
{
    BL_FUNC_DEBUG
    int a = mui_list->currentRow();
    if ( a < 0 ) {
        blMsgInfo ( _ ( "Debe seleccionar una linea" ) );
        return;
    } // end if
    try {
        mdb_idalumno = mui_list->dbValue ( "idalumno" );
        if ( editMode() ) {
            AlumnoView * cv = new AlumnoView ( ( BfCompany * ) mainCompany(), 0 );
            if ( cv->load ( mdb_idalumno ) )
                throw - 1;
            cv->on_mui_borrar_clicked();
            cv->close();
        } // end if
        presentar();
    } catch ( ... ) {
        blMsgInfo ( _ ( "Error al borrar el cobro a cliente." ) );
    } // end try
    
}


/** SLOT que responde al doble click en el subformulario.
    Dependiendo del modo (Edicion o Seleccion) hace unas operaciones u otras.
*/
/// \TODO: Deberia crearse el metodo editar y este llamar a ese.
void AlumnosList::editar ( int )
{
    BL_FUNC_DEBUG
    try {
        mdb_idalumno = mui_list->dbValue ( "idalumno" );
        if ( editMode() ) {
            AlumnoView * bud = new AlumnoView ( ( BfCompany * ) mainCompany(), 0 );
            if ( bud->load ( mdb_idalumno ) ) {
                delete bud;
                return;
            } // end if
            mainCompany() ->m_pWorkspace->addSubWindow ( bud );
            bud->show();
        } else {
	    close();
            emit ( selected ( mdb_idalumno ) );
        } // end if
    } catch ( ... ) {
        blMsgInfo ( _ ( "Debe seleccionar una fila primero" ) );
    } // end try
    

}

/** SLOT que responde a la peticion de menu contextual en el subformulario.
*/
/// \TODO: Revisar si este metodo es util.
void AlumnosList::submenu ( const QPoint & )
{
    BL_FUNC_DEBUG
    int a = mui_list->currentRow();
    if ( a < 0 )
        return;
    QMenu *popup = new QMenu ( this );
    QAction *edit = popup->addAction ( _ ( "Editar Alumno" ) );
    QAction *del = popup->addAction ( _ ( "Borrar Alumno" ) );
    QAction *opcion = popup->exec ( QCursor::pos() );
    if ( opcion == del )
        on_mui_borrar_clicked();
    if ( opcion == edit )
        on_mui_editar_clicked();
    delete popup;

    
}

/** Inicializa la clase con el puntero a la company que se esta utilizando
**/
void AlumnosList::setMainCompany ( BfCompany *comp )
{
    BL_FUNC_DEBUG
    BlMainCompanyPointer::setMainCompany ( comp );
    mui_list->setMainCompany ( comp );
    
    mui_idclase->setMainCompany ( mainCompany() );
    mui_idclase->setQuery ( "SELECT * FROM clase ORDER BY nomclase" );
    mui_idclase->setTableName ( "clase" );
    mui_idclase->setFieldId ( "idclase" );
    mui_idclase->m_valores["nomclase"] = "";
    mui_idclase->setAllowNull ( TRUE );
    mui_idclase->setId ( "" );
    

}

/** Devuelve el identificador del cobro seleccionado
**/
QString AlumnosList::idalumno()
{
    BL_FUNC_DEBUG
    
    return mdb_idalumno;
}



/// =============================================================================
///                    SUBFORMULARIO
/// =============================================================================
/** Prepara el subformulario para trabajar con la tabla alumno.
    Establece que no se puedan insertar elementos ya que es un listado.
*/
AlumnosListSubForm::AlumnosListSubForm ( QWidget *parent ) : BfSubForm ( parent )
{
    BL_FUNC_DEBUG
    /// Disparamos los plugins.
    int res = g_plugins->run ( "AlumnosListSubForm_AlumnosListSubForm", this );
    if ( res != 0 )
        return;
    setDbTableName ( "alumno" );
    setDbFieldId ( "idalumno" );
    addSubFormHeader ( "idalumno", BlDbField::DbInt, BlDbField::DbNotNull | BlDbField::DbPrimaryKey, BlSubFormHeader::DbHideView | BlSubFormHeader::DbNoWrite, _ ( "Id alumno" ) );
    addSubFormHeader ( "nombrealumno", BlDbField::DbVarChar, BlDbField::DbNoSave, BlSubFormHeader::DbNone | BlSubFormHeader::DbNoWrite, _ ( "Nombre" ) );
    addSubFormHeader ( "apellido1alumno", BlDbField::DbVarChar, BlDbField::DbNoSave, BlSubFormHeader::DbNone | BlSubFormHeader::DbNoWrite, _ ( "1er apellido" ) );
    addSubFormHeader ( "apellido2alumno", BlDbField::DbVarChar, BlDbField::DbNoSave, BlSubFormHeader::DbNone | BlSubFormHeader::DbNoWrite, _ ( "2o apellido" ) );
    addSubFormHeader ( "fechanacimientoalumno", BlDbField::DbVarChar, BlDbField::DbNoSave, BlSubFormHeader::DbNone | BlSubFormHeader::DbNoWrite, _ ( "F. Nacimiento" ) );

    setInsert ( FALSE );
    setDelete ( FALSE );
    setSortingEnabled ( TRUE );
    
}


AlumnosListSubForm::~AlumnosListSubForm()
{
    BL_FUNC_DEBUG
    
}

