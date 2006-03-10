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
//
//
#include "fpago.h"
#include "company.h"
#include "funcaux.h"

#include <QLineEdit>
#include <QMessageBox>
#include <QCloseEvent>


/** Constructor de la clase inicializa la clase y llama a la clase de pintar para que pinte */
FPagoView::FPagoView(company *emp,QWidget *parent, const char *name)
        : QDialog(parent, name) , dialogChanges(this) {
    setupUi(this);
    m_companyact=emp;
    setModoEdicion();
    m_cursorFPagoView=NULL;
    m_item=NULL;
    pintar();
}


/** Carga el query de la base de datos y carga el qlistview */
void FPagoView::pintar() {
    mui_lista->clear();
    if (m_cursorFPagoView != NULL)
        delete m_cursorFPagoView;
    m_cursorFPagoView = m_companyact->cargacursor("SELECT * FROM forma_pago ORDER BY idforma_pago");
    mui_lista->clear();
    while (!m_cursorFPagoView->eof()) {
        new QListWidgetItem(m_cursorFPagoView->valor("descforma_pago"), mui_lista);
        m_cursorFPagoView->siguienteregistro ();
    }// end while
    /// Comprobamos cual es la cadena inicial.
    dialogChanges_cargaInicial();
}// end pintar


FPagoView::~FPagoView() {
    if (m_cursorFPagoView != NULL)
        delete m_cursorFPagoView;
}




void FPagoView::on_mui_lista_currentItemChanged(QListWidgetItem *cur, QListWidgetItem *prev) {
    _depura("on_mui_lista_currentItemChanged",0);

    int row = mui_lista->row(cur);
    trataModificado();
    m_descforma_pago->setText(m_cursorFPagoView->valor("descforma_pago",row));
    m_dias1tforma_pago->setText(m_cursorFPagoView->valor("dias1tforma_pago",row));
    m_descuentoforma_pago->setText(m_cursorFPagoView->valor("descuentoforma_pago",row));
    mdb_idforma_pago = m_cursorFPagoView->valor("idforma_pago",row);
    m_item = cur;

    /// Comprobamos cual es la cadena inicial.
    dialogChanges_cargaInicial();

}// end if




void FPagoView::on_mui_guardar_clicked() {
    QString query = "UPDATE forma_pago SET descforma_pago='"+
                    m_companyact->sanearCadena(m_descforma_pago->text())+"', dias1tforma_pago= "+
                    m_companyact->sanearCadena(m_dias1tforma_pago->text())+" , descuentoforma_pago = "+
                    m_companyact->sanearCadena(m_descuentoforma_pago->text())+" WHERE idforma_pago="+mdb_idforma_pago;
    int error = m_companyact->ejecuta(query);
    if (error) {
        m_companyact->rollback();
        return;
    }// end if
    if (m_cursorFPagoView != NULL)
        delete m_cursorFPagoView;
    m_cursorFPagoView = m_companyact->cargacursor("SELECT * FROM forma_pago ORDER BY idforma_pago");
    if(m_item)
        m_item->setText(m_descforma_pago->text());


    dialogChanges_cargaInicial();

}// end if


bool FPagoView::trataModificado() {
    /// Si se ha modificado el contenido advertimos y guardamos.
    if (dialogChanges_hayCambios()) {
        if ( QMessageBox::warning( this, "Guardar Forma de Pago",
                                   "Desea guardar los cambios.",
                                   QMessageBox::Ok ,
                                   QMessageBox::Cancel ) == QMessageBox::Ok)
            on_mui_guardar_clicked();
        return (TRUE);
    }// end if
    return(FALSE);
}// end trataModificado


/** SLOT que responde a la pulsación del botón de nuevo tipo de iva
  * Inserta en la tabla de ivas
  */
void FPagoView::on_mui_crear_clicked() {
    /// Si se ha modificado el contenido advertimos y guardamos.
    trataModificado();
    QString query = "INSERT INTO forma_pago (descforma_pago, dias1tforma_pago, descuentoforma_pago) VALUES ('NUEVA FORMA DE PAGO',0,0)";
    m_companyact->begin();
    int error = m_companyact->ejecuta(query);
    if (error) {
        m_companyact->rollback();
        return;
    }// end if
    cursor2 *cur = m_companyact->cargacursor("SELECT max(idforma_pago) AS idFPagoView FROM forma_pago");
    m_companyact->commit();
    mdb_idforma_pago = cur->valor("idFPagoView");
    delete cur;
    pintar();
}// end s_newTipoIVA


/** SLOT que responde a la pulsaci� del bot� de borrar la familia que se est�editando.
  * Lo que hace es que se hace un update de todos los campos
  */
void FPagoView::on_mui_borrar_clicked() {
    trataModificado();
    m_companyact->begin();
    QString query = "DELETE FROM forma_pago WHERE idforma_pago="+mdb_idforma_pago;
    int error = m_companyact->ejecuta(query);
    if (error) {
        m_companyact->rollback();
        return;
    }// end if
    m_companyact->commit();
    pintar();
}// end s_saveTipoIVA



void FPagoView::closeEvent( QCloseEvent *e) {
    _depura("FPagoView::closeEvent",0);
    if (dialogChanges_hayCambios())  {
        int val = QMessageBox::warning( this, "Guardar Forma de Pago",
                                        "Desea guardar los cambios.","Si","No","Cancelar",0,2);
        if (val == 0)
            on_mui_guardar_clicked();
        if (val == 2)
            e->ignore();
    }// end if
}
