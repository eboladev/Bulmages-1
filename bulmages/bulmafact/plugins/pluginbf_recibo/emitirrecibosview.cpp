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

#include <QtWidgets/QMessageBox>
#include <QtGui/QCloseEvent>
#include <QtCore/QFile>
#include <QtCore/QTextStream>

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
    BL_FUNC_DEBUG
    
    setAttribute ( Qt::WA_DeleteOnClose );
    
    try {
    
        setupUi ( this );
        blCenterOnScreen ( this );

        setTitleName ( _ ( "Emitir Recibos" ) );
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


        insertWindow ( windowTitle(), this, false );
	blScript(this);
    } catch ( ... ) {
    
        blMsgInfo ( _ ( "Error al crear el recibo" ), this );
        
    } // end try
    
    
}

/** No precisa acciones adicionales en el destructor.
*/
EmitirRecibosView::~EmitirRecibosView()
{
    BL_FUNC_DEBUG
    
}


void EmitirRecibosView::on_mui_crear_clicked() {

    BL_FUNC_DEBUG
    
    mui_log->clear();
    
    // Iteramos para cada tutor / socio
    QString query = "SELECT nomcliente, cliente.idcliente, coalesce (suma, 0) AS numhijos, cuotacuotaporalumno, sociocliente FROM cliente LEFT JOIN (SELECT idcliente, count(idalumnocliente) AS suma FROM alumnocliente GROUP BY idcliente) AS t1 ON cliente.idcliente = t1.idcliente LEFT JOIN (SELECT * FROM cuotaporalumno) AS t2 ON t2.numalumnoscuotaporalumno = coalesce(t1.suma, 0)";
    BlDbRecordSet *cur = mainCompany() -> loadQuery( query );
    m_progreso->setMaximum(cur->numregistros());
    m_progreso->setValue(0);
    
    while (! cur -> eof () ) {

        QDate fechain = m_fechain->date();
        QDate fechafin = m_fechafin->date();
	bool haylineas = false;

	mui_log->append("<H2>"+cur->value("nomcliente")+"</H2>");
	
	
	mainCompany()->begin();

        QString query = "INSERT INTO recibo(descrecibo, idcliente) VALUES ('Recibo automático', " + cur->value("idcliente") + ")";
        mainCompany()-> runQuery(query);
        BlDbRecordSet *cur1 = mainCompany()->loadQuery("SELECT MAX(idrecibo) AS id FROM recibo");
        QString idrecibo = cur1->value("id");
        delete cur1;

	if (mui_cuotas->isChecked()) {
	  /// Si el tutor es socio se emite la cuota.
	  if (cur->value("sociocliente") == "t") {
	    query = "INSERT INTO lrecibo(idrecibo, cantlrecibo, conceptolrecibo) VALUES (" + idrecibo + ", " + cur->value("cuotacuotaporalumno") + ", 'Cuota por "  +cur->value("numhijos")+" alumno/s')";
	    mainCompany() -> runQuery(query);
	    haylineas = true;

	    mui_log->append("<H3>Cuota Socio "+cur->value("nomcliente")+"</H3>");
	    
	    // Si queremos recibos separados por todo ... entonces creamos un nuevo recibo y damos el anterior por cerrado.
	    if (!mui_recibounico->isChecked()) {
		QString query = "INSERT INTO recibo(descrecibo, idcliente) VALUES ('Recibo automático', " + cur->value("idcliente") + ")";
		mainCompany()-> runQuery(query);
		BlDbRecordSet *cur1 = mainCompany()->loadQuery("SELECT MAX(idrecibo) AS id FROM recibo");
		idrecibo = cur1->value("id");
		delete cur1;
	    } // end if
	    
	    
	  } // end if
	} // end if

	if (mui_actividades -> isChecked() || (mui_actividad -> isChecked() && mui_idactividad->id() != "") ) {
	  /// Si hay actividades se facturan.
	  query = "SELECT * FROM alumnoactividad LEFT JOIN alumno as t1 ON alumnoactividad.idalumno = t1.idalumno LEFT JOIN actividad AS t2 ON alumnoactividad.idactividad = t2.idactividad LEFT JOIN alumnocliente AS t3 ON t3.idalumno = t1.idalumno LEFT JOIN cliente AS t4 ON t4.idcliente = t3.idcliente WHERE t3.idcliente = " + cur->value("idcliente") + " AND t3.porcentalumnocliente <> 0";
	  if (mui_actividad -> isChecked() && mui_idactividad->id() != "") {
	      /// Si hay facturacion de una actividad concreta se factura.
	      query = "SELECT * FROM alumnoactividad LEFT JOIN alumno as t1 ON alumnoactividad.idalumno = t1.idalumno LEFT JOIN actividad AS t2 ON alumnoactividad.idactividad = t2.idactividad LEFT JOIN alumnocliente AS t3 ON t3.idalumno = t1.idalumno LEFT JOIN cliente AS t4 ON t4.idcliente = t3.idcliente WHERE t3.idcliente = " + cur->value("idcliente") + " AND t3.porcentalumnocliente <> 0 AND t2.idactividad = " +mui_idactividad->id();
	  } // end if
	  
	  cur1 = mainCompany() -> loadQuery(query);
	  while (! cur1 -> eof () ) {
		
	      /// Calculamos la cuota correspondiente aplicando los descuentos que toca.
	      BlFixed cuota(cur1->value("precioactividad"));
	      BlFixed cuotaorig(cur1->value("precioactividad"));
	      int numerodehijos = cur1->numregistros();
	      query = "SELECT * FROM cuotaporalumno WHERE numalumnoscuotaporalumno = " + QString::number(numerodehijos);
	      BlDbRecordSet * curcuota = mainCompany()->loadQuery(query);
	      if ( ! curcuota -> eof() ) {
		  BlFixed reduccionporhijos = curcuota->value("descuentocuotaporalumno");
		  cuota = cuota - cuotaorig * reduccionporhijos / 100;
	      } // end if
	      delete curcuota;
	    
	      /// Calculamos el descuento pertinente segun el numero de actividades que realiza.
	      query = "SELECT COALESCE(count(idalumnoactividad), 0) AS numactividades FROM alumnoactividad WHERE idalumno = " + cur1->value("idalumno");
	      BlDbRecordSet * curnumactividades = mainCompany() -> loadQuery(query);
	      query = "SELECT * FROM cuotaporactividad WHERE numactividadescuotaporactividad = " + curnumactividades->value("numactividades");
	      delete curnumactividades;
	      BlDbRecordSet * curdesporactividades = mainCompany() -> loadQuery(query);
	      if ( ! curdesporactividades -> eof() ) {
		  BlFixed reduccionporactividades = curdesporactividades->value("descuentocuotaporactividad");
		  cuota = cuota - cuotaorig * reduccionporactividades / 100;
	      } // end if
	      delete curdesporactividades;
	    
	      query = "INSERT INTO lrecibo(idrecibo, cantlrecibo, conceptolrecibo) VALUES (" + idrecibo + ", " + cuota.toQString('.') + ", '"+cur1->value("nombreactividad")+" Cuota por "  +cur1->value("nombrealumno")+" ')";
	      mainCompany() -> runQuery(query);
	      mui_log->append("<H3>"+cur1->value("nombreactividad")+" Cuota por "  +cur1->value("nombrealumno")+"</H3>");
	      cur1 -> nextRecord();
	      haylineas = true;


	      
	      // Si queremos recibos separados por todo ... entonces creamos un nuevo recibo y damos el anterior por cerrado.
	      if (!mui_recibounico->isChecked()) {
		  QString query = "INSERT INTO recibo(descrecibo, idcliente) VALUES ('Recibo automático', " + cur->value("idcliente") + ")";
		  mainCompany()-> runQuery(query);
		  BlDbRecordSet *cur1 = mainCompany()->loadQuery("SELECT MAX(idrecibo) AS id FROM recibo");
		  idrecibo = cur1->value("id");
		  delete cur1;
	      } // end if

	      
	  } // end while
	} // end if


	/// Si al final resulta que no habia lineas o que estamos en el modo recibos separados se hace un delete del recibo.
        if (haylineas == false || !mui_recibounico->isChecked()) {
	  mainCompany()->runQuery("DELETE FROM recibo WHERE idrecibo = "+ idrecibo);
	} // end if

	mainCompany()->commit();

        cur -> nextRecord();
        m_progreso->setValue(cur->currentRecord());
      
    } // end while
    
    delete cur;
    
    blMsgInfo(_("Terminado"));
    
    m_progreso->setValue(0);
    
    
}
