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
#include <QFile>

#include "empresabase.h"
#include "ticket.h"
#include "plugins.h"



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
    /// Disparamos los plugins.
    int res = g_plugins->lanza("Ticket_pintar", this);
    if (res != 0) {
        return;
    } // end if
    _depura("END Ticket::pintar", 0);
}

QList<DBRecord *> *Ticket::listaLineas() {
    return m_listaLineas;
}


DBRecord *Ticket::insertarArticulo(QString idArticulo, Fixed cantidad, bool nuevaLinea) {
    _depura("Ticket::insertarArticulo", 0);

    /// Buscamos si ya hay una linea con el articulo que buscamos
    m_lineaActual = NULL;
    DBRecord *item;
    for (int i = 0; i < listaLineas()->size(); ++i) {
        item = listaLineas()->at(i);
        if (item->DBvalue("idarticulo") == idArticulo)
            m_lineaActual = item;
    }// end for

    if (m_lineaActual && nuevaLinea == FALSE) {
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

void  Ticket::imprimir() {
        QFile file( confpr->valor(CONF_TICKET_PRINTER_FILE) );
        if ( !file.open(QIODevice::WriteOnly | QIODevice::Unbuffered)) {
            _depura("Error en la Impresion de ticket", 2);
        } // end if


        file.write (QString("Conetxia Soluciones Informaticas S.L\n").toAscii());
        file.write (QString("====================================\n").toAscii());
        file.write(QString("Joaquin Turina, 1 Local 4\n").toAscii());
        file.write(QString("CP: 07004 Palma de Mallorca\n").toAscii());
        file.write(QString("Tel: 971 29 06 29\n").toAscii());
        /// Imprimimos espacios
        file.write ( "\n \n", 3);
        /// Imprimimos la fecha
        file.write( QString("Fecha: ").toAscii());
        QDate fecha = QDate::currentDate();
        QString sfecha = fecha.toString("d-M-yyyy");
        file.write( sfecha.toAscii());
        file.write ( "\n", 1);

/*
        file.write( QString("Trabajador: ").toAscii());
        file.write( m_albaranClienteView->mui_idtrabajador->currentText().toAscii());
        file.write ( "\n", 1);

        file.write( QString("Cliente: ").toAscii());
        file.write( m_albaranClienteView->mui_idcliente->cifcliente().toAscii());
        file.write ( " ", 1);
        file.write( m_albaranClienteView->mui_idcliente->nomcliente().toAscii());
        file.write ( "\n", 1);
*/


/*
        file.write( QString("Descripcion: ").toAscii());
        file.write( m_albaranClienteView->DBvalue("descalbaran").toAscii());
        file.write ( "\n", 1);

        /// Imprimimos espacios
        file.write ( "\n \n", 3);

        QString l;
        SDBRecord *linea;
        for (int i = 0; i < m_albaranClienteView->getlistalineas()->rowCount(); ++i) {
            linea = m_albaranClienteView->getlistalineas()->lineaat(i);
            if (linea->DBvalue( "idarticulo") != "") {
                QString str = linea->DBvalue("cantlalbaran").rightJustified(5,' ')+QString("   ")+linea->DBvalue("desclalbaran").leftJustified(23,' ', TRUE)+linea->DBvalue("pvplalbaran").rightJustified(10, ' ');
                file.write(str.toAscii());
                file.write ( "\n", 1);
            } // end if
        } // end for

        /// Imprimimos espacios
        file.write ( "\n \n", 3);

        /// Imprimimos la base imponible
        QString stotalbases = QString("Base Imp").rightJustified(30,' ');
        stotalbases = stotalbases + m_albaranClienteView->m_totalBases->text().rightJustified(10,' ');
        file.write(stotalbases.toAscii());
        file.write ( "\n", 1);

        /// Imprimimos el IVA
        QString stotaltases = QString("I.V.A.").rightJustified(30,' ');
        stotaltases = stotaltases + m_albaranClienteView->m_totalTaxes->text().rightJustified(10,' ');
        file.write(stotaltases.toAscii());
        file.write ( "\n", 1);

        /// Imprimimos el total
        file.write (QString("____________________\n").rightJustified(42,' ').toAscii());

        QString stotal = QString("TOTAL").rightJustified(30,' ');
        stotal = stotal + m_albaranClienteView->m_totalalbaran->text().rightJustified(10,' ');
        file.write(stotal.toAscii());
        file.write ( "\n", 1);


        /// Imprimimos espacios
        file.write ( "\n \n \n \n", 7);

        /// Preparamos para un codigo de barras
        /// Especificamos la altura del codigo de barras
        file.write ("\x1Dh\x40",3);
        /// Especificamos que los caracteres vayan debajo del codigo de barras
        file.write ( "\x1DH\x02",3);
        /// Establecemos el tipo de codificacion para el codigo de barras
        file.write ( "\x1D",1);
        file.write ( "f\x01",2);
        ;
        /// Ponemos el ancho de la fuente a uno
        file.write ( "\x1D\x77\x01",3);
        /// Imprimimos la palabra top con el juego de caracteres 04
        file.write ( "\x1Dk\x04",3);
        file.write (QString("ALB").toAscii());
        file.write (" ", 1);
        file.write (m_albaranClienteView->DBvalue("idalbaran").toAscii());
        file.write (" ", 1);
        file.write (m_albaranClienteView->DBvalue("numalbaran").toAscii());
        file.write (" ", 1);
        file.write (m_albaranClienteView->DBvalue("refalbaran").toAscii());
        file.write ("\x00", 1);


*/
        /// Imprimimos el dibujo final
        /*
                file.write ("\x1B\x2A\x00\xD2\x00", 5);
                for (int i=0; i <15; i++) {
                    file.write ( "\x01\x02\x04\x08\x10\x20\x40\x80", 8);
                    file.write ("\x40\x20\x10\x08\x04\x02", 6);
                }
        */
        /// Imprimimos espacios
        file.write ( "\n \n \n \n \n", 9);


        /// El corte de papel.
        file.write ("\x1D\x56\x01", 3);
        file.close();
}


void Ticket::subir() {
    int i = listaLineas()->indexOf(lineaActTicket());
    if (i > 0) i--;
    setLineaActual(listaLineas()->at(i));
    pintar();
}

void Ticket::bajar() {
    int i = listaLineas()->indexOf(lineaActTicket());
    if (i < listaLineas()->size() -1) i++;
    setLineaActual(listaLineas()->at(i));
    pintar();
}


void Ticket::agregarCantidad(QString cantidad) {
    Fixed cant(cantidad);
    Fixed cantorig(m_lineaActual->DBvalue("cantlticket"));
    Fixed suma = cant + cantorig;
    if (suma == Fixed("0.00")) {
        listaLineas()->removeAt(listaLineas()->indexOf(m_lineaActual));
        m_lineaActual = listaLineas()->at(0);
    } else {
        m_lineaActual->setDBvalue("cantlticket", suma.toQString());
    } // end if
    pintar();
}

void Ticket::ponerCantidad(QString cantidad) {
    if (cantidad == "0") {
        listaLineas()->removeAt(listaLineas()->indexOf(m_lineaActual));
        m_lineaActual = listaLineas()->at(0);
    } else {
        m_lineaActual->setDBvalue("cantlticket", cantidad);
    } // end if
    pintar();
}

void Ticket::ponerPrecio(QString precio) {
    m_lineaActual->setDBvalue("pvplticket", precio);
    pintar();
}

void Ticket::insertarArticuloCodigo(QString codigo) {
    QString query = "SELECT * FROM articulo WHERE codigocompletoarticulo= '"+ codigo +"'";
    cursor2 *cur = empresaBase()->cargacursor(query);
    if (!cur->eof()) {
        insertarArticulo(cur->valor("idarticulo"), Fixed("1"));
    } // end if
    delete cur;
}

void Ticket::insertarArticuloCodigoNL(QString codigo) {
    QString query = "SELECT * FROM articulo WHERE codigocompletoarticulo= '"+ codigo +"'";
    cursor2 *cur = empresaBase()->cargacursor(query);
    if (!cur->eof()) {
        insertarArticulo(cur->valor("idarticulo"), Fixed("1"), TRUE);
    } // end if
    delete cur;
}
