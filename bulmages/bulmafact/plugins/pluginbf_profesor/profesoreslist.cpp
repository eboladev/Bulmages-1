/***************************************************************************
 *   Copyright (C) 2009 by Tomeu Borras Riera                              *
 *   tborras@conetxia.com                                                  *
 *                                                                         *
 *   Copyright (C) 2009 by Arturo Martin Llado                             *
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
#include "profesoreslist.h"
#include "bfcompany.h"
#include "profesorview.h"
#include "blfunctions.h"

/** Inicializa todos los componentes.
    Mete la ventana en el workSpace.
    Este constructor no es completo, debe inicializarse con setcompany para que la clase pueda operar.
*/
ProfesoresList::ProfesoresList ( QWidget *parent, Qt::WFlags flag, edmode editmodo )
        : BlFormList ( NULL, parent, flag, editmodo )
{
    blDebug ( "ProfesoresList::ProfesoresList", 0 );
    
    setupUi ( this );
    
    /// Disparamos los plugins.
    int res = g_plugins->lanza ( "ProfesoresList_ProfesoresList", this );
    if ( res != 0 )
        return;
        
    mdb_idprofesor = "";
    setSubForm ( mui_list );
    hideBusqueda();
    /// Llamamos a los scripts
    blScript(this);
    blDebug ( "END ProfesoresList::ProfesoresList", 0 );
}

/** Inicializa todos los componentes.
    Hace una presentacion inicial.
    Mete la ventana en el workSpace.
*/
ProfesoresList::ProfesoresList ( BfCompany *comp, QWidget *parent, Qt::WFlags flag, edmode editmodo )
        : BlFormList ( comp, parent, flag, editmodo )
{
    blDebug ( "ProfesoresList::ProfesoresList", 0 );
    
    setupUi ( this );
    
    /// Disparamos los plugins.
    int res = g_plugins->lanza ( "ProfesoresList_ProfesoresList", this );
    if ( res != 0 )
        return;

    mui_list->setMainCompany ( comp );


    setSubForm ( mui_list );
    
    /// Cargamos los filtros guardados.
    cargaFiltrosXML();
    
    presentar();
    mdb_idprofesor = "";
    
    if ( modoEdicion() )
        mainCompany() ->insertWindow ( windowTitle(), this );
        
    hideBusqueda();
    
    /// Hacemos el tratamiento de los permisos que desabilita botones en caso de no haber suficientes permisos.
    trataPermisos ( "profesor" );
    /// Llamamos a los scripts
    blScript(this);
    blDebug ( "END ProfesoresList::ProfesoresList", 0 );
}


/** No requiere acciones especiales en el destructor.
*/
ProfesoresList::~ProfesoresList()
{
    blDebug ( "ProfesoresList::~ProfesoresList", 0 );
    blDebug ( "END ProfesoresList::~ProfesoresList", 0 );
}


/** Hace la carag del listado.
    PAra ello genera los SELECTS con ayuda de generaFiltro y los pasa al SubFormulario para que los presente.
    Tambien hace un select de calculo de totales y lo presenta en el textEdit correspondiente.
*/
void ProfesoresList::presentar()
{
    blDebug ( "ProfesoresList::presentar", 0 );
    
    if ( mainCompany() != NULL ) {
        mui_list->cargar ( "SELECT * FROM profesor WHERE 1 = 1 " + generaFiltro() );
    } // end if
    
    blDebug ( "END ProfesoresList::presentar", 0 );
}

/** Metodo auxiliar que genera la clausula WHERE del listado con las opciones de filtrado especificadas.
*/
QString ProfesoresList::generaFiltro()
{
    blDebug ( "ProfesoresList::generaFiltro", 0 );
    
    QString filtro = "";
    if ( m_filtro->text() != "" ) {
        filtro = " AND ( lower(nombreprofesor) LIKE lower('%" + m_filtro->text() + "%') ";
        filtro += " ) ";
    } // end if

    blDebug ( "END ProfesoresList::generaFiltro", 0 );
    
    return ( filtro );
}

/** SLOT que responde a la pulsacion del boton de crear en el listado.
    Instancia la clase ProfesorView, y la presenta.
*/
void ProfesoresList::crear()
{
    blDebug ( "ProfesoresList::crear", 0 );
    
    if (modoConsulta()) {
	/// El modo consulta funciona algo diferente
        QDialog *diag = new QDialog ( 0 );
        diag->setModal ( true );
        diag->setGeometry ( QRect ( 0, 0, 750, 550 ) );
        blCenterOnScreen ( diag );

	ProfesorView *bud = new ProfesorView ( ( BfCompany * ) mainCompany(), 0 );
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

	QString idprofesorold = "";
	BlDbRecordSet *curold = mainCompany()->loadQuery("SELECT max(idprofesor) AS id FROM profesor");
	if( !curold->eof()) {
		      idprofesorold = curold->value("id");
	} // end if
	delete curold;
	
	
        diag->exec();      
      
	BlDbRecordSet *cur = mainCompany()->loadQuery("SELECT max(idprofesor) AS id FROM profesor");
	if( !cur->eof()) {
		  if (cur->value("id") != idprofesorold) {
		      mdb_idprofesor = cur->value("id");
	              emit ( selected ( mdb_idprofesor ) );
		  } // end if
	} // end if
	delete cur;
	
    } else {
      ProfesorView *bud = new ProfesorView ( ( BfCompany * ) mainCompany(), 0 );
      mainCompany() ->m_pWorkspace->addSubWindow ( bud );
      bud->show();
      bud->pintar();
    }
    blDebug ( "ProfesoresList::crear", 0 );
}

/** La impresion de listados esta completamente delegada a la clase SubForm3
*/
void ProfesoresList::imprimir()
{
    blDebug ( "ProfesoresList::imprimir", 0 );
    
    mui_list->printPDF ( _ ( "Monitores" ) );
    
    blDebug ( "END ProfesoresList::imprimir", 0 );
}

/** SLOT que responde a la pulsacion del boton borrar.
    Comprueba que exista un elemento seleccionado.
    Instancia la clase ProfesorView, la inicializa y le lanza el metodo borrar.
    Esta es la forma correcta de implementar un borrado a partir de un listado
    ya que de esta forma si existen plugins que alteren el borrado tambien seran invocados.
*/
void ProfesoresList::borrar()
{
    blDebug ( "ProfesoresList::borrar", 0 );
    
    int a = mui_list->currentRow();
    
    if ( a < 0 ) {
        blMsgInfo ( _ ( "Debe seleccionar una linea" ) );
        return;
    } // end if
    
    try {
        mdb_idprofesor = mui_list->dbValue ( "idprofesor" );
        if ( modoEdicion() ) {
            ProfesorView * cv = new ProfesorView ( ( BfCompany * ) mainCompany(), 0 );
            if ( cv->cargar ( mdb_idprofesor ) )
                throw - 1;
            cv->on_mui_borrar_clicked();
            cv->close();
        } // end if
        presentar();
    } catch ( ... ) {
        blMsgInfo ( _ ( "Error al borrar el cobro a cliente" ) );
    } // end try
    
    blDebug ( "END:ProfesoresList::borrar", 0 );
}

/** SLOT que responde al doble click en el subformulario.
    Dependiendo del modo (Edicion o Seleccion) hace unas operaciones u otras.
*/
/// \TODO: Deberia crearse el metodo editar y este llamar a ese.
void ProfesoresList::editar ( int )
{
    blDebug ( "ProfesoresList::editar", 0 );
    
    try {
        mdb_idprofesor = mui_list->dbValue ( "idprofesor" );
        if ( modoEdicion() ) {
            ProfesorView * bud = new ProfesorView ( ( BfCompany * ) mainCompany(), 0 );
            if ( bud->cargar ( mdb_idprofesor ) ) {
                delete bud;
                return;
            } // end if
            mainCompany() ->m_pWorkspace->addSubWindow ( bud );
            bud->show();
        } else {
            emit ( selected ( mdb_idprofesor ) );
        } // end if
    } catch ( ... ) {
        blMsgInfo ( _ ( "Debe seleccionar una fila primero" ) );
    } // end try
    
    blDebug ( "END ProfesoresList::editar", 0 );
}

/** SLOT que responde a la peticion de menu contextual en el subformulario.
*/
/// \TODO: Revisar si este metodo es util.
void ProfesoresList::submenu ( const QPoint & )
{
    blDebug ( "ProfesoresList::submenu", 0 );
    
    int a = mui_list->currentRow();
    
    if ( a < 0 )
        return;
        
    QMenu *popup = new QMenu ( this );
    QAction *edit = popup->addAction ( _ ( "Editar profesor" ) );
    QAction *del = popup->addAction ( _ ( "Borrar profesor" ) );
    QAction *opcion = popup->exec ( QCursor::pos() );
    
    if ( opcion == del )
        on_mui_borrar_clicked();
        
    if ( opcion == edit )
        on_mui_editar_clicked();
        
    delete popup;
    
    blDebug ( "END ProfesoresList::submenu", 0 );
}

/** Inicializa la clase con el puntero a la company que se esta utilizando
**/
void ProfesoresList::setMainCompany ( BfCompany *comp )
{
    blDebug ( "ProfesoresList::setMainCompany", 0 );
    
    BlMainCompanyPointer::setMainCompany ( comp );
    mui_list->setMainCompany ( comp );
    
    blDebug ( "END ProfesoresList::setMainCompany", 0 );
}

/** Devuelve el identificador del cobro seleccionado
**/
QString ProfesoresList::idprofesor()
{
    blDebug ( "ProfesoresList::idprofesor", 0 );
    blDebug ( "END ProfesoresList::idprofesor", 0 );
    
    return mdb_idprofesor;
}

/// =============================================================================
///                    SUBFORMULARIO
/// =============================================================================
/** Prepara el subformulario para trabajar con la tabla profesor.
    Establece que no se puedan insertar elementos ya que es un listado.
*/
ProfesoresListSubForm::ProfesoresListSubForm ( QWidget *parent ) : BfSubForm ( parent )
{
    blDebug ( "ProfesoresListSubForm::ProfesoresListSubForm", 0 );
    
    /// Disparamos los plugins.
    int res = g_plugins->lanza ( "ProfesoresListSubForm_ProfesoresListSubForm", this );
    if ( res != 0 )
        return;
        
    setDbTableName ( "profesor" );
    setDbFieldId ( "idprofesor" );
    addSubFormHeader ( "idprofesor", BlDbField::DbInt, BlDbField::DbNotNull | BlDbField::DbPrimaryKey, BlSubFormHeader::DbHideView | BlSubFormHeader::DbNoWrite, _ ( "Id profesor" ) );
    addSubFormHeader ( "nombreprofesor", BlDbField::DbVarChar, BlDbField::DbNoSave, BlSubFormHeader::DbNone | BlSubFormHeader::DbNoWrite, _ ( "Nombre" ) );
    addSubFormHeader ( "dniprofesor", BlDbField::DbVarChar, BlDbField::DbNoSave, BlSubFormHeader::DbNone | BlSubFormHeader::DbNoWrite, _ ( "DNI" ) );
    addSubFormHeader ( "telprofesor", BlDbField::DbVarChar, BlDbField::DbNoSave, BlSubFormHeader::DbNone | BlSubFormHeader::DbNoWrite, _ ( "Telefono" ) );
    addSubFormHeader ( "poblprofesor", BlDbField::DbVarChar, BlDbField::DbNoSave, BlSubFormHeader::DbNone | BlSubFormHeader::DbNoWrite, _ ( "Poblacion" ) );

    setInsert ( FALSE );
    setDelete ( FALSE );
    setSortingEnabled ( TRUE );
    
    blDebug ( "END ProfesoresListSubForm::ProfesoresListSubForm", 0 );
}

ProfesoresListSubForm::~ProfesoresListSubForm()
{
    blDebug ( "ProfesoresListSubForm::~ProfesoresListSubForm", 0 );
    blDebug ( "END ProfesoresListSubForm::~ProfesoresListSubForm", 0 );
}
