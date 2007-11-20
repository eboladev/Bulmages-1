/***************************************************************************
 *   Copyright (C) 2007 by Tomeu Borras Riera                              *
 *   tborras@conetxia.com                                                  *
 *   http://www.iglues.org                                                 *
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


#include <QWidget>
#include <QCloseEvent>

#include "empresabase.h"
#include "ticket.h"



Ticket::Ticket(EmpresaBase *emp, QWidget *parent) : BLWidget(emp, parent), DBRecord(emp) {
    _depura("Ticket::Ticket", 0);
    /// Inicializamos los parametros del ticket para la base de datos.
    setDBTableName ( "ticket" );
    setDBCampoId ( "idticket" );
    addDBCampo ( "idticket", DBCampo::DBint, DBCampo::DBPrimaryKey, QApplication::translate ( "Ticket", "Id ticket" ) );
    addDBCampo ( "fechaticket", DBCampo::DBdate, DBCampo::DBNotNull, QApplication::translate ( "Ticket", "Id ticket" ) );

    m_lineaActual = NULL;
    m_listaLineas = new QList<DBRecord *>;
    _depura("END Ticket::Ticket", 0);
}

Ticket::~Ticket() {
    _depura("Ticket::~Ticket", 0);
    _depura("END Ticket::~Ticket", 0);
}

DBRecord * Ticket::agregarLinea() {
    _depura("Ticket::agregarLinea",0);

    /// Creamos un nuevo DBRecord y lo inicializamos.
    DBRecord * item = new DBRecord(empresaBase());
    item->setDBTableName ( "lticket" );
    item->setDBCampoId ( "idlticket" );
    item->addDBCampo ( "idlticket", DBCampo::DBint, DBCampo::DBPrimaryKey, QApplication::translate ( "Ticket", "Id lticket" ) );
    item->addDBCampo ( "cantlticket", DBCampo::DBnumeric, DBCampo::DBNotNull, QApplication::translate ( "Ticket", "Cantidad" ) );
    item->addDBCampo ( "pvplticket", DBCampo::DBnumeric, DBCampo::DBNotNull, QApplication::translate ( "Ticket", "Precio" ) );
    item->addDBCampo ( "idarticulo", DBCampo::DBint, DBCampo::DBNotNull, QApplication::translate ( "Ticket", "Id Articulo" ) );
    item->addDBCampo ( "codigocompletoarticulo", DBCampo::DBvarchar, DBCampo::DBNoSave, QApplication::translate ( "Ticket", "Codigo Articulo" ) );
    item->addDBCampo ( "nomarticulo", DBCampo::DBvarchar, DBCampo::DBNoSave, QApplication::translate ( "Ticket", "Nombre Articulo" ) );


    /// Agregamos el DBRecord a la lista de lineas de ticket.
    m_listaLineas->append(item);

    _depura("END Ticket::agregarLinea",0);
    return item;
}

void Ticket::pintar() {
    _depura("Ticket::pintar", 0, "Metodo para reimplementar en clases derivadas");
    _depura("END Ticket::pintar", 0);
}

QList<DBRecord *> *Ticket::listaLineas() {
    return m_listaLineas;
}


DBRecord *Ticket::insertarArticulo(QString idArticulo, Fixed cantidad, int nuevaLinea) {
    _depura("Ticket::insertarArticulo", 0);

    /// Buscamos si ya hay una linea con el articulo que buscamos
    m_lineaActual = NULL;
    DBRecord *item;
    for (int i = 0; i < listaLineas()->size(); ++i) {
        item = listaLineas()->at(i);
	if (item->DBvalue("idarticulo") == idArticulo)
		m_lineaActual = item;
    }// end for

    if (m_lineaActual) {
	/// Ya hay una linea con este articulo (es un agregado)
	Fixed cantidadib(m_lineaActual->DBvalue("cantlticket"));
	Fixed cant1 = cantidadib + cantidad;
	m_lineaActual->setDBvalue("cantlticket", cant1.toQString());
    } else {
	/// No hay ningun item con este articulo (es una insercion)
	m_lineaActual = agregarLinea();
	m_lineaActual->setDBvalue("idarticulo", idArticulo);
	m_lineaActual->setDBvalue("cantlticket", cantidad.toQString());
	
	/// Buscamos los parametros en la base de datos.
	QString query = "SELECT * FROM articulo WHERE idarticulo = " + idArticulo;
	cursor2 *cur = empresaBase()->cargacursor(query);
	if (!cur->eof()) {
		m_lineaActual->setDBvalue("pvplticket", cur->valor("pvparticulo"));
		m_lineaActual->setDBvalue("codigocompletoarticulo", cur->valor("codigocompletoarticulo"));
		m_lineaActual->setDBvalue("nomarticulo", cur->valor("nomarticulo"));
	} // end if
	delete cur;
    } // end if


    /// Pintamos el ticket ya que se ha modificado.
    pintar();

    _depura("END Ticket::insertarArticulo", 0);
    return m_lineaActual;
}


void  Ticket::borrarArticulo(DBRecord *linea, Fixed cantidad) {}


void  Ticket::vaciarTicket() {}

void  Ticket::subirPosArticulo(DBRecord *linea, int filas) {}

void  Ticket::bajarPosArticulo(DBRecord *linea, int filas) {}

void  Ticket::inicioPosTicket(DBRecord *) {}

void  Ticket::finPosTicket(DBRecord *) {}


DBRecord * Ticket::lineaTicket(int posicion) {

	return NULL;
}


DBRecord *Ticket::lineaActTicket() {
   return m_lineaActual;
}

void Ticket::setLineaActual(DBRecord *rec) {
	m_lineaActual = rec;
}


void  Ticket::setDescuentoGlobal(Fixed descuento) {}

void  Ticket::imprimir() {}

