/***************************************************************************
 *   Copyright (C) 2004 by Tomeu Borr� Riera                              *
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
#ifndef REGISTROIVAVIEW_H
#define REGISTROIVAVIEW_H

#include "ui_registroivabase.h"
#include "registroiva.h"
#include "postgresiface2.h"
#include "ficha.h"
#include "fixed.h"


#include <QLineEdit>
#include <Q3TextEdit>
#include <QLabel>
#include <QCheckBox>

#include "dialogchanges.h"

/** @author Tomeu Borras Riera */
class empresa;

class RegistroIvaView : public Ficha, public Ui_RegistroIvaBase , public RegistroIva  {
    Q_OBJECT
private:
    cursor2 *m_cursorcombo;
    /// Cursor para las formas de pago.
    cursor2 *m_cursorFPago;
public:
    RegistroIvaView(empresa *, QWidget *);
    ~RegistroIvaView();
    void manageArticle(int);

    virtual void pintaidregistroiva(const QString &val) {}
    ;
    virtual void pintacontrapartida(const QString &val) {
        m_contrapartida->setidcuenta(val);
    };
    virtual void pintabaseimp(const QString &val) {
        Fixed total(val);
        total = total + Fixed(iva());
        m_baseImponible->setText(val);
        m_totalFactura->setText(total.toQString());
    };
    virtual void pintaiva(const QString &val) {
        Fixed total(val);
        total = total + Fixed(baseimp());
        m_totalFactura->setText(total.toQString());
        m_importeiva->setText(val);
    };
    virtual void pintaffactura(const QString &val) {
        m_ffactura->setText(val);
    };
    virtual void pintafemisionregistroiva(const QString &val) {
        m_femisionregistroiva->setText(val);
    };
    virtual void pintaserieregistroiva(const QString &val) {
        m_serieregistroiva->setText(val);
    };
    virtual void pintafactura(const QString &val) {
        m_factura->setText(val);
    };
    virtual void pintaidborrador(const QString &val) {}
    ;
    virtual void pintaregularizacion(const QString &val) {}
    ;
    virtual void pintaplan349(const QString &val) {}
    ;
    virtual void pintanumorden(const QString &val) {
        m_numorden->setText(val);
    };
    virtual void pintacif(const QString &val) {
        m_cif->setText(val);
    };
    virtual void pintaidfpago(const QString &val) {}
    ;
    virtual void pintafactemitida(const QString &val) {
        if (val == "t" || val == "TRUE")
            m_factEmitida->setChecked(TRUE);
        else
            m_factSoportada->setChecked(FALSE);
    };
virtual void pintarectificaaregistroiva(const QString &val) {}
    ;
    virtual void pintaincregistro(const QString &val) {
        if (val == "t" || val == "TRUE")
            m_incregistroIVA->setChecked(TRUE);
        else
            m_incregistroIVA->setChecked(FALSE);
    };


private:
    void generarPedidoCliente();
    virtual void accept() {
        if(!guardar())
            close();
    };
    virtual int guardar();
    void cargarComboFPago(QString);

public slots://
    virtual void s_saveRegistroIvaView() {
        guardar();
    };
    virtual int borrar() {return RegistroIva::borrar();};
    virtual int cargar(QString id);
    virtual void on_mui_borrar_clicked();
    virtual void s_removeRegistroIvaViewLine() {
        m_listiva->borraIvaAct();
    };
    virtual void boton_generarPrevisiones();

    /// Este slot se activa cuando hay cambios en los subformularios.
    virtual void s_listIvaChanged() {
        _depura("s_listIvaChanged",0);
        Fixed base = m_listalineas->calculabase();
        Fixed iva = m_listalineas->calculaiva();
        setbaseimp(base.toQString());
        setiva(iva.toQString());
        pintabaseimp(base.toQString());
        pintaiva(iva.toQString());
    };

};
#endif
