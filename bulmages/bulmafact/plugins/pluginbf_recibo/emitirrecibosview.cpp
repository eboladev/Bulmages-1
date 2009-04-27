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
    QString query = "SELECT nomcliente, cliente.idcliente, coalesce (suma, 0) AS numhijos, cuotacuotaporalumno FROM cliente LEFT JOIN (SELECT idcliente, count(idalumnocliente) AS suma FROM alumnocliente GROUP BY idcliente) AS t1 ON cliente.idcliente = t1.idcliente LEFT JOIN (SELECT * FROM cuotaporalumno) AS t2 ON t2.numalumnoscuotaporalumno = coalesce(t1.suma, 0) WHERE cliente.sociocliente = TRUE";
    BlDbRecordSet *cur = mainCompany() -> loadQuery( query );
    m_progreso->setMaximum(cur->numregistros());
    m_progreso->setValue(0);
    while (! cur -> eof () ) {

      QDate fechain = m_fechain->date();
      QDate fechafin = m_fechafin->date();

      QString query = "INSERT INTO recibo(descrecibo, idcliente) VALUES ('Recibo automático', " + cur->valor("idcliente") + ")";
      mainCompany()-> runQuery(query);
      BlDbRecordSet *cur1 = mainCompany()->loadQuery("SELECT MAX(idrecibo) AS id FROM recibo");
      QString idrecibo = cur1->valor("id");
      delete cur1;
      
      query = "INSERT INTO lrecibo(idrecibo, cantlrecibo, conceptolrecibo) VALUES (" + idrecibo + ", " + cur->valor("cuotacuotaporalumno") + ", 'Cuota por "  +cur->valor("numhijos")+" alumno/s')";
      mainCompany() -> runQuery(query);
      
      query = "SELECT * FROM alumnoactividad LEFT JOIN alumno as t1 ON alumnoactividad.idalumno = t1.idalumno LEFT JOIN actividad AS t2 ON alumnoactividad.idactividad = t2.idactividad LEFT JOIN alumnocliente AS t3 ON t3.idalumno = t1.idalumno LEFT JOIN cliente AS t4 ON t4.idcliente = t3.idcliente WHERE t3.idcliente = " + cur->valor("idcliente");
      cur1 = mainCompany() -> loadQuery(query);
      while (! cur1 -> eof () ) {
        query = "INSERT INTO lrecibo(idrecibo, cantlrecibo, conceptolrecibo) VALUES (" + idrecibo + ", " + cur1->valor("precioactividad") + ", '"+cur1->valor("nombreactividad")+" Cuota por "  +cur1->valor("nombrealumno")+" ')";
        mainCompany() -> runQuery(query);
        
        cur1 -> nextRecord();
      } // end while
      
      cur -> nextRecord();
      m_progreso->setValue(cur->currentRecord());
    } // end while
    delete cur;
    mensajeInfo(_("Terminado"));
    m_progreso->setValue(0);
    _depura ( "END EmitirRecibosView::on_mui_crear_released", 0 );
}

