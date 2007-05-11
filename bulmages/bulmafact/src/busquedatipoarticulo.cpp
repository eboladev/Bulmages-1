/***************************************************************************
 *   Copyright (C) 2005 by Tomeu Borras Riera                              *
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

#include "busquedatipoarticulo.h"
#include "tiposarticuloview.h"
#include "company.h"


/** Inicializa todos los componentes a NULL para que no haya confusiones sobre
    si un elemento ha sido inicializado o no.
*/
BusquedaTipoArticulo::BusquedaTipoArticulo(QWidget *parent)
        : BLWidget(parent) {
    _depura("BusquedaTipoArticulo::BusquedaTipoArticulo", 0);
    setupUi(this);
    mdb_idtipo_articulo = "";
    mdb_desctipo_articulo = "";
    mdb_codtipo_articulo = "";
    _depura("BusquedaTipoArticulo::BusquedaTipoArticulo", 0);
}


/** El destructor de la clase no requiere de acciones adicionales.
*/
BusquedaTipoArticulo::~BusquedaTipoArticulo() {
    _depura("BusquedaTipoArticulo::~BusquedaTipoArticulo", 0);
    _depura("END BusquedaTipoArticulo::~BusquedaTipoArticulo", 0);
}


/** Mediante este metodo se puede indicar cual es el tipo de articulo seleccionado.
    Busca en la base de datos el tipo que concuerde con el identificador pasado.
    Coge todos los datos necesarios para la presentacion y los muestra.
*/
void BusquedaTipoArticulo::setidtipo_articulo(QString val) {
    _depura("BusquedaTipoArticulo::setidtipo_articulo", 0);
    mdb_idtipo_articulo = val;

    /// Comprobamos que hayamos pasado un parametro valido.
    if (val == "") {
        mdb_idtipo_articulo = "";
        mdb_desctipo_articulo = "";
        mdb_codtipo_articulo = "";
    } else {
	QString SQLQuery = "SELECT * FROM tipo_articulo WHERE idtipo_articulo = '" + mdb_idtipo_articulo + "'";
	cursor2 *cur = empresaBase()->cargacursor(SQLQuery);
	
	if(!cur->eof()) {
		mdb_codtipo_articulo = cur->valor("codtipo_articulo");
		mdb_desctipo_articulo = cur->valor("desctipo_articulo");
	} // end if
    	delete cur;
    } // end if
    m_codtipo_articulo->setText(mdb_codtipo_articulo);
    m_desctipo_articulo->setText(mdb_desctipo_articulo);
    _depura("END BusquedaTipoArticulo::setidtipo_articulo", 0);
}


/** Mediante este metodo se puede indicar cual es el tipo de articulo seleccionado.
    Busca en la base de datos el tipo que coincide con el codigo pasado.
    Coge todos los datos necesarios para la presentacion y los muestra.
*/
void BusquedaTipoArticulo::setcodtipo_articulo(QString val) {
    _depura("BusquedaTipoArticulo::setcodtipo_articulo", 0);
    mdb_codtipo_articulo = val;
    QString SQLQuery = "SELECT * FROM tipo_articulo WHERE codtipo_articulo = '" + mdb_codtipo_articulo + "'";
    cursor2 *cur = empresaBase()->cargacursor(SQLQuery);

    if(!cur->eof()) {
        mdb_idtipo_articulo = cur->valor("idtipo_articulo");
        mdb_desctipo_articulo = cur->valor("desctipo_articulo");
    } else {
        mdb_idtipo_articulo = "";
        mdb_desctipo_articulo = "";
    } // end if

    delete cur;
    m_codtipo_articulo->setText(mdb_codtipo_articulo);
    m_desctipo_articulo->setText(mdb_desctipo_articulo);
    _depura("END BusquedaTipoArticulo::setcodtipo_articulo", 0);
}


/// Busqueda de TipoArticulos.
/** SLOT que responde a la pulsacion del boton de buscar tipo.
    Crea una instancia de TipoArticuloList y lo pone en modo Seleccionar.
    Lo muestra como un Dialog y espera a que termine la seleccion de tipo.
    Una vez seleccionado el tipo muestra todos los datos del tipo y lo pone como
    elemento seleccionado.
*/
void BusquedaTipoArticulo::on_mui_buscar_clicked() {
    _depura("BusquedaTipoArticulo::on_mui_buscar_clicked", 0);
    QDialog *diag = new QDialog(0);
    diag->setModal(true);

    TipoArticuloList *tip = ((company *)empresaBase())->newTipoArticuloList(diag, TRUE);
    connect(tip, SIGNAL(selected(QString)), diag, SLOT(accept()));

    /// Creamos un layout donde estara el contenido de la ventana y la ajustamos al QDialog
    /// para que sea redimensionable y aparezca el titulo de la ventana.
    QHBoxLayout *layout = new QHBoxLayout;
    layout->addWidget(tip);
    layout->setMargin(0);
    layout->setSpacing(0);
    diag->setLayout(layout);
    diag->setWindowTitle(tip->windowTitle());

    diag->exec();

    if (tip->codtipo_articulo() != "") {
        m_codtipo_articulo->setText(tip->codtipo_articulo());
        mdb_codtipo_articulo = tip->codtipo_articulo();
        m_desctipo_articulo->setText(tip->desctipo_articulo());
        mdb_desctipo_articulo = tip->desctipo_articulo();
        mdb_idtipo_articulo = tip->idtipo_articulo();
    } // end if

    delete diag;
    _depura("END BusquedaTipoArticulo::on_mui_buscar_clicked", 0);

}


/** SLOT que responde al cambio de texto en el campo de tipo de articulo.
    Busca un tipo de articulo cuyo codigo corresponda con el codigo introducido y si lo encuentra.
    lo establece como predeterminado y lo presenta.
    Emite un signal de valueChanged().
*/
void BusquedaTipoArticulo::on_m_codtipo_articulo_textChanged(const QString &val) {
    _depura("BusquedaTipoArticulo::on_m_codtipo_articulo_textChanged", 0);
    mdb_codtipo_articulo = val;
    QString SQLQuery = "SELECT * FROM tipo_articulo WHERE codtipo_articulo = '" + mdb_codtipo_articulo + "'";
    cursor2 *cur = empresaBase()->cargacursor(SQLQuery);

    if(!cur->eof()) {
        mdb_idtipo_articulo = cur->valor("idtipo_articulo");
        mdb_desctipo_articulo = cur->valor("desctipo_articulo");
    } else {
        mdb_idtipo_articulo = "";
        mdb_desctipo_articulo = "";
    } // end if

    delete cur;
    m_codtipo_articulo->setText(mdb_codtipo_articulo);
    m_desctipo_articulo->setText(mdb_desctipo_articulo);
    emit(valueChanged(mdb_idtipo_articulo));
    _depura("END BusquedaTipoArticulo::on_m_codtipo_articulo_textChanged", 0);
}



/** Devuelve el codigo de tipo de articulo seleccionado, si no hay ningun elemento seleccionado devuelve
    un string vacio
**/
QString BusquedaTipoArticulo::codtipo_articulo() {
    _depura("BusquedaTipoArticulo::codtipo_articulo", 0);
    _depura("END BusquedaTipoArticulo::codtipo_articulo", 0);
    return m_codtipo_articulo->text();
}

/** Devuelve el identificador del tipo de articulo seleccionado, si no hay ningun elemento seleccionado
    devuelve un string vacio
**/
QString BusquedaTipoArticulo::idtipo_articulo() {
    _depura("BusquedaTipoArticulo::idtipo_articulo", 0);
    _depura("END BusquedaTipoArticulo::idtipo_articulo", 0);
    return mdb_idtipo_articulo;
}

/** Devuelve la descripcion del tipo de articulo seleccionado, si no hay ningun elemento seleccionado
    devuelve un string vacio
**/
QString BusquedaTipoArticulo::desctipo_articulo() {
    _depura("BusquedaTipoArticulo::desctipo_articulo", 0);
    _depura("END BusquedaTipoArticulo::desctipo_articulo", 0);
    return desctipo_articulo();
}


