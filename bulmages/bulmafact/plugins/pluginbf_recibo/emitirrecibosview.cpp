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

#include <fstream>

#include <QMessageBox>
#include <QCloseEvent>
#include <QFile>
#include <QTextStream>

#include "emitirrecibosview.h"
#include "bfcompany.h"
#include "blconfiguration.h"
#include "bldatesearch.h"
#include "blfunctions.h"

/** inicializa todos los componentes de la clase.
    Resetea el sistema de control de cambios para que considere que no hay cambios por parte del usuario.
    Mete la ventana en el workSpace.
*/
EmitirRecibosView::EmitirRecibosView ( BfCompany *comp, QWidget *parent )
        : BfForm ( comp, parent )
{
    _depura ( "EmitirRecibosView::EmitirRecibosView", 0 );
    
    setAttribute ( Qt::WA_DeleteOnClose );
    
    try {
    
        setupUi ( this );
        centrarEnPantalla ( this );

        setTitleName ( _ ( "EmitirRecibos" ) );
        setDbTableName ( "recibo" );
        setDbFieldId ( "idrecibo" );
        addDbField ( "idrecibo", BlDbField::DbInt, BlDbField::DbPrimaryKey, _ ( "ID recibo" ) );
        addDbField ( "cantrecibo", BlDbField::DbNumeric, BlDbField::DbNotNull, _ ( "Cantidad" ) );
        addDbField ( "idcliente", BlDbField::DbInt, BlDbField::DbNotNull, _ ( "Id cliente" ) );
        addDbField ( "idforma_pago", BlDbField::DbInt, BlDbField::DbNotNull, _ ( "Id Forma PAgo" ) );

        /// Establecemos los parametros de busqueda de Profesor
        mui_idactividad->setMainCompany ( comp );
        mui_idactividad->setLabel ( _ ( "Actividad:" ) );
        mui_idactividad->setTableName ( "actividad" );
        mui_idactividad->m_valores["codigoactividad"] = "";
        mui_idactividad->m_valores["nombreactividad"] = "";


        meteWindow ( windowTitle(), this, FALSE );

    } catch ( ... ) {
    
        mensajeInfo ( _ ( "Error al crear el recibo" ), this );
        
    } // end try
    
    _depura ( "END EmitirRecibosView::EmitirRecibosView", 0 );
}

/** No precisa acciones adicionales en el destructor.
*/
EmitirRecibosView::~EmitirRecibosView()
{
    _depura ( "EmitirRecibosView::~EmitirRecibosView", 0 );
    _depura ( "END EmitirRecibosView::~EmitirRecibosView", 0 );
}

void EmitirRecibosView::on_mui_crear_released() {

    _depura ( "EmitirRecibosView::on_mui_crear_released", 0 );
    
    // Iteramos para cada tutor / socio
    QString query = "SELECT nomcliente, cliente.idcliente, coalesce (suma, 0) AS numhijos, cuotacuotaporalumno, sociocliente FROM cliente LEFT JOIN (SELECT idcliente, count(idalumnocliente) AS suma FROM alumnocliente GROUP BY idcliente) AS t1 ON cliente.idcliente = t1.idcliente LEFT JOIN (SELECT * FROM cuotaporalumno) AS t2 ON t2.numalumnoscuotaporalumno = coalesce(t1.suma, 0)";
    BlDbRecordSet *cur = mainCompany() -> loadQuery( query );
    m_progreso->setMaximum(cur->numregistros());
    m_progreso->setValue(0);
    
    while (! cur -> eof () ) {

        QDate fechain = m_fechain->date();
        QDate fechafin = m_fechafin->date();
	bool haylineas = FALSE;

	mainCompany()->begin();

        QString query = "INSERT INTO recibo(descrecibo, idcliente) VALUES ('Recibo automático', " + cur->valor("idcliente") + ")";
        mainCompany()-> runQuery(query);
        BlDbRecordSet *cur1 = mainCompany()->loadQuery("SELECT MAX(idrecibo) AS id FROM recibo");
        QString idrecibo = cur1->valor("id");
        
        delete cur1;

	if (mui_cuotas->isChecked()) {
	  /// Si el tutor es socio se emite la cuota.
	  if (cur->valor("sociocliente") == "t") {
	    query = "INSERT INTO lrecibo(idrecibo, cantlrecibo, conceptolrecibo) VALUES (" + idrecibo + ", " + cur->valor("cuotacuotaporalumno") + ", 'Cuota por "  +cur->valor("numhijos")+" alumno/s')";
	    mainCompany() -> runQuery(query);
	    haylineas = TRUE;
	  } // end if
	} // end if

	if (mui_actividades -> isChecked() ) {
	  /// Si hay actividades se facturan.
	  query = "SELECT * FROM alumnoactividad LEFT JOIN alumno as t1 ON alumnoactividad.idalumno = t1.idalumno LEFT JOIN actividad AS t2 ON alumnoactividad.idactividad = t2.idactividad LEFT JOIN alumnocliente AS t3 ON t3.idalumno = t1.idalumno LEFT JOIN cliente AS t4 ON t4.idcliente = t3.idcliente WHERE t3.idcliente = " + cur->valor("idcliente") + " AND t3.porcentalumnocliente <> 0";
	  cur1 = mainCompany() -> loadQuery(query);
	  while (! cur1 -> eof () ) {
	      query = "INSERT INTO lrecibo(idrecibo, cantlrecibo, conceptolrecibo) VALUES (" + idrecibo + ", " + cur1->valor("porcentalumnocliente") +"*"+cur1->valor("precioactividad")+"/100" + ", '"+cur1->valor("nombreactividad")+" Cuota por "  +cur1->valor("nombrealumno")+" ')";
	      mainCompany() -> runQuery(query);
	      cur1 -> nextRecord();
	      haylineas = TRUE;
	  } // end while
	} // end if
        
	if (mui_actividad -> isChecked() && mui_idactividad->id() != "") {
	  /// Si hay actividades se facturan.
	  query = "SELECT * FROM alumnoactividad LEFT JOIN alumno as t1 ON alumnoactividad.idalumno = t1.idalumno LEFT JOIN actividad AS t2 ON alumnoactividad.idactividad = t2.idactividad LEFT JOIN alumnocliente AS t3 ON t3.idalumno = t1.idalumno LEFT JOIN cliente AS t4 ON t4.idcliente = t3.idcliente WHERE t3.idcliente = " + cur->valor("idcliente") + " AND t3.porcentalumnocliente <> 0 AND t2.idactividad = " +mui_idactividad->id();
	  cur1 = mainCompany() -> loadQuery(query);
	  while (! cur1 -> eof () ) {
	      query = "INSERT INTO lrecibo(idrecibo, cantlrecibo, conceptolrecibo) VALUES (" + idrecibo + ", " + cur1->valor("porcentalumnocliente") +"*"+cur1->valor("precioactividad")+"/100" + ", '"+cur1->valor("nombreactividad")+" Cuota por "  +cur1->valor("nombrealumno")+" ')";
	      mainCompany() -> runQuery(query);
	      cur1 -> nextRecord();
	      haylineas = TRUE;
	  } // end while
	} // end if


	/// Si al final resulta que no habia lineas se hace un delete.
        if (haylineas == FALSE) {
	  mainCompany()->runQuery("DELETE FROM recibo WHERE idrecibo = "+ idrecibo);
	} // end if

	mainCompany()->commit();

        cur -> nextRecord();
        m_progreso->setValue(cur->currentRecord());
      
    } // end while
    
    delete cur;
    
    mensajeInfo(_("Terminado"));
    
    m_progreso->setValue(0);
    
    _depura ( "END EmitirRecibosView::on_mui_crear_released", 0 );
}
