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

#include <QCloseEvent>
#include <QComboBox>
#include <QLayout>
#include <QMessageBox>
#include <QObject>
#include <QToolButton>
#include <QWidget>

#include <fstream>

#include "albaranclientelist.h"
#include "albaranclienteview.h"
#include "clientslist.h"
#include "cobroview.h"
#include "company.h"
#include "contrato.h"
#include "contratoview.h"
#include "funcaux.h"
#include "informereferencia.h"
#include "plugins.h"

/** Inicializa la pantalla.
    Inicializa todos los componentes y mete la pantalla en el workSpace.
*/
ContratoView::ContratoView(company *comp, QWidget *parent)
        : Contrato(comp, parent) {
    _depura("ContratoView::ContratoView", 0);
    setAttribute(Qt::WA_DeleteOnClose);
    try {
        setupUi(this);
        
        /// Disparamos los plugins.
        int res = g_plugins->lanza("ContratoView_ContratoView", this);
        if (res != 0)
            return;
        
        subform2->setcompany(comp);
        mui_idcliente->setcompany(comp);
        mui_refcontrato->setcompany(comp);
	
	    /// Inicializamos FichaBf
	    setListaLineas(subform2);
        comp->meteWindow(windowTitle(), this, FALSE);
    } catch (...) {
        mensajeInfo(tr("Error al crear la contrato"));
    }
    _depura("END ContratoView::ContratoView");
}


/** Al destruir la clase provocamos que se refresque el listado de contratos.
    Para que salga actualizado.
*/
ContratoView::~ContratoView() {
    _depura("ContratoView::~ContratoView", 0);
    _depura("END ContratoView::~ContratoView", 0);
}


/** Inicializa los subformularios y resetea el control de cambios.
*/
void ContratoView::inicializar() {
    _depura("ContratoView::inicializar", 0);
    subform2->inicializar();
    dialogChanges_cargaInicial();
    _depura("END ContratoView::inicializar", 0);
}







/** Carga una contrato de la base de datos.
    Deleta toda la carga a la clase \ref Contrato 
    Cambia el titulo de la ventana y reseta el control de cambios.
*/
int ContratoView::cargar(QString id) {
    _depura("ContratoView::cargar", 0);
    try {
        Contrato::cargar(id);
        if (DBvalue("idcontrato") != "") {
            setWindowTitle(tr("Contrato") + " " + DBvalue("refcontrato") + " " + DBvalue("idcontrato"));
            m_companyact->meteWindow(windowTitle(), this);
        } // end if
        dialogChanges_cargaInicial();
    } catch (...) {
        return -1;
    } // end try
    _depura("END ContratoView::cargar", 0);
    return 0;
}


/** Guardado de la ficha en la base de datos.
    Utiliza los metodos setXXX para establecer los valores de la Ficha en el DBRecord
    y luego llama a \ref Contrato::guardar() para el guardado en la base de datos.
*/
/// \TODO: Una vez hecho el guardado deberia hacer una carga y dejar de resetear el
/// control de cambios que deberia estar en el metodo de carga.
int ContratoView::guardar() {
    _depura("ContratoView::guardar", 0);
    try {
        setDBvalue("refcontrato", mui_refcontrato->text());
        setDBvalue("idcliente", mui_idcliente->idcliente());
        setDBvalue("fincontrato", mui_fincontrato->text());
        setDBvalue("descontrato", mui_desccontrato->text());
        Contrato::guardar();
        dialogChanges_cargaInicial();
    } catch (...) {
        _depura("ContratoView::guardar error al guardar", 0);
        throw -1;
    } // end try
    _depura("END ContratoView::guardar", 0);
    return 0;
}


void ContratoView::on_m_cliente_valueChanged(QString id) {
	_depura("ContratoView::on_m_cliente_valueChanged", 0);
	subform2->setIdCliente(id);
	_depura("END ContratoView::on_m_cliente_valueChanged", 0);
}

