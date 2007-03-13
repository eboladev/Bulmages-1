/***************************************************************************
 *   Copyright (C) 2003 by Tomeu Borras Riera                              *
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

#include "amortizacionesview.h"
#include "amortizacionview.h"

#include <empresa.h>


/** Constructor de la clase del listado.
 *  Inicializa las variables y elementos del listado.
 */
/// TODO: Esta derivando de Ficha y deberia derivar de FichaBc
amortizacionesview::amortizacionesview(empresa *emp, QWidget *parent)
        : FichaBc(emp, parent) {
    _depura("amortizacionesview::amortizacionesview", 0);
    this->setAttribute(Qt::WA_DeleteOnClose);
    setupUi(this);
   /// Hacemos la inicializacion de un listado embebido.
    mui_listado->setcompany(m_companyact);
    mui_listado->setDBTableName("amortizacion");
    mui_listado->setFileConfig("AmortizacionListSubform");
    mui_listado->setDBCampoId("idamortizacion");
    mui_listado->addSHeader("idamortizacion", DBCampo::DBint, DBCampo::DBNotNull | DBCampo::DBPrimaryKey, SHeader::DBNoView | SHeader::DBNoWrite, tr("Id amortizacion"));
    mui_listado->addSHeader("nomamortizacion", DBCampo::DBdate, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, tr("Nombre"));
    mui_listado->setinsercion(FALSE);
    inicializatabla();
    m_companyact->meteWindow(windowTitle(), this, FALSE);
    _depura("END amortizacionesview::amortizacionesview", 0);
}

/** Destructor de la clase que libera mermoria y saca la ventana de la lista
 * de ventanas.
 */
amortizacionesview::~amortizacionesview() {
    _depura("amortizacionesview::~amortizacionesview", 0);
    m_companyact->sacaWindow(this);
    _depura("END amortizacionesview::~amortizacionesview", 0);
}

/** Inicializa y rellena desde la base de datos el listado de amortizaciones.
 */
/// TODO: La clase se basa en QTableWidget y deberia usar SubForm2Bc
void amortizacionesview::inicializatabla()  {
    _depura("amortizacionesview::inicializatabla", 0);
    mui_listado->cargar("SELECT * FROM amortizacion");
    _depura("END amortizacionesview::inicializatabla", 0);
}

/** SLOT que responde a hacer doble click sobre la tabla listado.
 * lo que hace es invocar a la clase AmortizacionView y generar en esta una carga del elemento
 * que ha sido clickado y mostrarlo.
 * Si el modo es modo seleccion en lugar de modo edicion coge los valores del elemento seleccionado y cierra la ventana.
 */
void amortizacionesview::on_mui_listado_cellDoubleClicked(int row, int) {
    _depura("amortizacionesview::on_listado_cellDoubleClicked", 0);
    editAmortizacion( row);
    _depura("END amortizacionesview::on_listado_cellDoubleClicked", 0);
}


/** SLOT que responde a la pulsacion del boton crear una nueva amortizacion.
 * Crea una instancia de la clase AmortizacionView y la llama.
 */
void amortizacionesview::on_mui_crear_clicked() {
    _depura("amortizacionesview::on_mui_crear_clicked", 0);
    AmortizacionView *amor = new AmortizacionView(m_companyact, 0);
    m_companyact->pWorkspace()->addWindow(amor);
    amor->show();
    _depura("END amortizacionesview::on_mui_crear_clicked", 0);
}


/// Esta funci&oacute;n se encarga de borrar una amortizaci&oacute;n.
/// La que esta seleccionada en el listado.
void amortizacionesview::on_mui_borrar_clicked() {
    _depura("amortizacionesview::on_mui_borrar_clicked", 0);
    QString codigo = mui_listado->DBvalue("idamortizacion");
    if (codigo != "") {
        QString query = "DELETE FROM linamortizacion WHERE idamortizacion = " + codigo;
        m_companyact->begin();
        m_companyact->ejecuta(query);
        query = "DELETE FROM amortizacion WHERE idamortizacion = " + codigo;
        m_companyact->ejecuta(query);
        m_companyact->commit();
        inicializatabla();
    } // end if
    _depura("END amortizacionesview::on_mui_borrar_clicked", 0);
}

/// SLOT que responde a la pulsacion del boton actualizar. Que hace que se redibuje el listado de amortizaciones.
void amortizacionesview::on_mui_actualizar_clicked() {
	_depura("amortizacionesview::on_mui_actualizar_clicked", 0);
	inicializatabla();
	_depura("END amortizacionesview::on_mui_actualizar_clicked", 0);
}

/** Se encarga de la accion preseleccionada al hacer doble click o al darle
    al boton de editar. 
    Si estamos en modo seleccion actualiza el valor de los campos de seleccion y
    cierra la ventana.
    Si estamos en modo edicion abre una instancia de ArticuloView y lo carga con el
    valor seleccionado.
*/
/// \TODO: este metodo deberia ser editar
void amortizacionesview::editAmortizacion(int row) {
    _depura("amortizacionesview::editAmortizacion", 0);
    mdb_idamortizacion = mui_listado->DBvalue("idamortizacion", row);
    mdb_nomamortizacion = mui_listado->DBvalue("nomamortizacion", row);
    if (modoEdicion()) {
        /// Creamos el objeto mpatrimonialview, y lo lanzamos.
        AmortizacionView *amor = new AmortizacionView(m_companyact, 0);
        amor->cargar(mdb_idamortizacion);
        m_companyact->pWorkspace()->addWindow(amor);
        amor->show();
    } else {
        close();
        emit(selected(mdb_idamortizacion));
    } // end if
    _depura("END amortizacionesview::editAmortizacion", 0);
}

