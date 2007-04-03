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

#include "busquedacuenta.h"
#include "../contabilidad/listcuentasview1.h"
#include "../contabilidad/empresa.h"


BusquedaCuenta::BusquedaCuenta(QWidget *parent)
        : QWidget(parent) {
    _depura("BusquedaCuenta::BusquedaCuenta", 10);
    setupUi(this);
    m_companyact = NULL;
    mdb_idcuenta = "";
    mdb_nomcuenta = "";
    mdb_codigocuenta = "";
    mdb_tipocuenta = "";
    connect(mui_codigocuenta, SIGNAL(editingFinished()), this, SLOT(s_lostFocus()));
    _depura("END BusquedaCuenta::BusquedaCuenta", 0);
}


BusquedaCuenta::~BusquedaCuenta() {
    _depura("END BusquedaCuenta::~BusquedaCuenta", 0);
}


void BusquedaCuenta::setempresa(empresa *comp) {
    _depura("BusquedaCuenta::setempresa", 10);
    m_companyact = comp;
    m_numdigitos = m_companyact->numdigitosempresa();
    _depura("END BusquedaCuenta::setempresa", 0);
}


void BusquedaCuenta::setidcuenta(QString val) {
    _depura("BusquedaCuenta::setidcuenta", 10);
    mdb_idcuenta=val;
    QString SQLQuery = "SELECT * FROM cuenta WHERE idcuenta = '" + mdb_idcuenta + "'";
    cursor2 *cur = m_companyact->cargacursor(SQLQuery);
    if (!cur->eof()) {
        mdb_codigocuenta = cur->valor("codigo");
        mdb_nomcuenta = cur->valor("descripcion");
        mdb_tipocuenta = cur->valor("tipocuenta");
    } else {
        mdb_idcuenta = "";
        mdb_nomcuenta = "";
        mdb_codigocuenta = "";
        mdb_tipocuenta = "";
    } // end if
    delete cur;
    mui_codigocuenta->setText(mdb_codigocuenta);
    mui_nomcuenta->setText(mdb_nomcuenta);
    _depura("END BusquedaCuenta::setidcuenta", 0);
}


void BusquedaCuenta::setcodigocuenta(QString val) {
    _depura("BusquedaCuenta::setcodigocuenta", 10);
    mdb_codigocuenta = val;
    QString SQLQuery = "SELECT * FROM cuenta WHERE codigo = '" + mdb_codigocuenta + "'";
    cursor2 *cur = m_companyact->cargacursor(SQLQuery);
    if (!cur->eof()) {
        mdb_idcuenta = cur->valor("idcuenta");
        mdb_nomcuenta = cur->valor("descripcion");
        mdb_tipocuenta = cur->valor("tipocuenta");
    } else {
        mdb_idcuenta = "";
        mdb_nomcuenta = "";
        mdb_tipocuenta = "";
    } // end if
    delete cur;
    mui_codigocuenta->setText(mdb_codigocuenta);
    mui_nomcuenta->setText(mdb_nomcuenta);
    _depura("END BusquedaCuenta::setcodigocuenta", 0);
}


/// B&uacute;squeda de cuentas.
void BusquedaCuenta::s_searchCuenta() {
    _depura("BusquedaCuenta::s_searchCuenta", 10);
    /// Generamos un di&aacute;logo.
    QDialog *diag = new QDialog(0);
    diag->setModal(true);

    /// Creamos una instancia del selector de cuentas.
    listcuentasview1 *listcuentas = new listcuentasview1(m_companyact, diag, 0, listcuentasview1::SelectMode);

    /// Hacemos la conexi&oacute;n del cerrar de las cuentas con el cerrar di&aacute;logo.
    connect(listcuentas, SIGNAL(selected(QString)), diag, SLOT(accept()));
    connect(listcuentas, SIGNAL(destroyed(QObject)), diag, SLOT(accept()));

    listcuentas->inicializa();

    /// Sacamos el di&aacute;logo.
    diag->exec();
    if (listcuentas->codcuenta() != "") {
        mdb_idcuenta = listcuentas->idcuenta();
        mdb_codigocuenta= listcuentas->codcuenta();
        mdb_nomcuenta = listcuentas->desccuenta();
        mui_codigocuenta->setText(mdb_codigocuenta);
        mui_nomcuenta->setText(mdb_nomcuenta);
    } // end if
    delete diag;
    _depura("END BusquedaCuenta::s_searchCuenta", 0);
}


void BusquedaCuenta::s_codigocuentatextChanged(const QString &val) {
    _depura("BusquedaCuenta::s_codigocuentatextChanged", 10, val);
    if (val == "+") {
        s_searchCuenta();
        emit(valueChanged(mui_codigocuenta->text()));
    } // end if
    _depura("END BusquedaCuenta::s_codigocuentatextChanged", 0);
}


void BusquedaCuenta::s_lostFocus() {
    _depura("BusquedaCuenta::s_lostFocus", 10);
    mdb_codigocuenta=mui_codigocuenta->text();
    QString cad = mdb_codigocuenta;
    if (cad != "") {
        cad = extiendecodigo(cad,m_numdigitos);
        cursor2 *cursorcta = m_companyact->cargacuenta(0, cad);
        int num = cursorcta->numregistros();
        if (num == 1) {
            mdb_codigocuenta = cursorcta->valor("codigo");
            mdb_idcuenta = cursorcta->valor("idcuenta");
            mdb_nomcuenta = cursorcta->valor("descripcion");
            mdb_tipocuenta = cursorcta->valor("tipocuenta");
            mui_codigocuenta->setText(mdb_codigocuenta);
            mui_nomcuenta->setText(mdb_nomcuenta);
        } else {
            mdb_idcuenta = "";
            mdb_codigocuenta = "";
            mdb_nomcuenta = "";
            mdb_tipocuenta = "";
            mui_nomcuenta->setText("");
        } // end if
        delete cursorcta;
    } // end if
    emit(valueChanged(mdb_idcuenta));
    _depura("END BusquedaCuenta::s_lostFocus", 0);
}


/// ===================================================================
/// Busqueda Cuenta Delegate para usar con los subforms
/// ===================================================================
/** Inicializa todos los componentes del Widget a NULL para que no haya posibles confusiones
    sobre si un elemento ha sido creado o no.
    Conecta el SIGNAL activated() con m_activated() para tratarlo.
*/
BusquedaCuentaDelegate::BusquedaCuentaDelegate(QWidget *parent)
        : QComboBox(parent) {
    _depura("BusquedaCuentaDelegate::BusquedaCuentaDelegate", 10);
    m_companyact = NULL;
    m_cursorcombo = NULL;
    setEditable(true);
    setSizeAdjustPolicy(QComboBox::AdjustToContents);
//    setCompleter(0);
//    connect(this, SIGNAL(activated(int)), this, SLOT(m_activated(int)));
    connect(this, SIGNAL(editTextChanged(const QString &)), this, SLOT(s_editTextChanged(const QString &)));
    _depura("END BusquedaCuentaDelegate::BusquedaCuentaDelegate", 0);
}


void BusquedaCuentaDelegate::setcompany(empresa *comp) {
    m_companyact = comp;
}


/** Libera la memoria reservada.
*/
BusquedaCuentaDelegate::~BusquedaCuentaDelegate() {
    _depura("BusquedaCuentaDelegate::~BusquedaCuentaDelegate", 10);
    if (m_cursorcombo != NULL)
        delete m_cursorcombo;
    _depura("END BusquedaCuentaDelegate::~BusquedaCuentaDelegate", 0);
}


/** Permite indicar al Widget cual es la serie de factura seleccionada por defecto.
    Recarga cursor de serie_factura y cuando encuentra un registro cuyo codigoserie_factura coincide con el pasado
    como parametro lo establece como el registro activo por el comboBox.
*/
void BusquedaCuentaDelegate::s_editTextChanged(const QString &cod) {
    _depura("BusquedaCuentaDelegate::s_editTextChanged", 10);
    static bool semaforo = FALSE;
    QString codigo = cod;
    QStringList listacodigos;

    if (codigo.size() < 3) {
        /// Si hay menos de 3 caracteres en el QComboBox no tiene que aparecer el autocompletar.
       // completar->popup->
        return;
    } // end if

    if (codigo.size() > 3) return;

    if (semaforo) {
        return;
    } else {
        semaforo = TRUE;
    } // end if

    codigo = codigo.left(codigo.indexOf(".-"));
    m_cursorcombo = m_companyact->cargacursor("SELECT codigo, descripcion FROM cuenta WHERE codigo LIKE '" + codigo + "%' ORDER BY codigo LIMIT 25");

    ///TODO: La idea es que salga en el desplegable del combobox el listado de cuentas que
    /// coincidan con el texto escrito para poder elegirlo.
    while (!m_cursorcombo->eof()) {
        //addItem(m_cursorcombo->valor("codigo") + ".-" + m_cursorcombo->valor("descripcion"));
        listacodigos << m_cursorcombo->valor("codigo");
        m_cursorcombo->siguienteregistro();
    }
    listacodigos.clear();
    listacodigos << "hola";

    completar = new QCompleter(listacodigos, this);
    completar->setCompletionMode(QCompleter::PopupCompletion);
    completar->setCaseSensitivity(Qt::CaseInsensitive);

    setCompleter(completar);

    semaforo = FALSE;
    _depura("END BusquedaCuentaDelegate::s_editTextChanged", 0);
}


void BusquedaCuenta::s_returnPressed() {
    _depura("s_returnPressed", 10);
    s_lostFocus();
    emit returnPressed();
}


