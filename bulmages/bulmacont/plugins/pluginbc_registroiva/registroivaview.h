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

#ifndef REGISTROIVAVIEW_H
#define REGISTROIVAVIEW_H

#include <QtWidgets/QLineEdit>
#include <QtWidgets/QLabel>
#include <QtWidgets/QCheckBox>

#include "ui_registroivabase.h"
#include "registroiva.h"
#include "blpostgresqlclient.h"
#include "blform.h"
#include "blfixed.h"
#include "bldialogchanges.h"


class BcCompany;

class RegistroIvaView : public RegistroIva, public Ui_RegistroIvaBase
{
    Q_OBJECT

private:
    BlDbRecordSet *m_cursorcombo;
    /// Cursor para las formas de pago.
    BlDbRecordSet *m_cursorFPago;

public:
    RegistroIvaView ( BcCompany *, QWidget * );
    virtual ~RegistroIvaView();
    void manageArticle ( int );
    virtual void pintaidregistroiva ( const QString & ) {}
    ;
    virtual void pintacontrapartida ( const QString &val ) {
        m_contrapartida->setId ( val );
    };
    virtual void pintabaseimp ( const QString &val ) {
        BlFixed total ( val );
        total = total + BlFixed ( iva() );
        m_baseImponible->setText ( val );
        m_totalFactura->setText ( total.toQString() );
    };
    virtual void pintaiva ( const QString &val ) {
        BlFixed total ( val );
        total = total + BlFixed ( baseimp() );
        m_totalFactura->setText ( total.toQString() );
        m_importeiva->setText ( val );
    };
    virtual void pintaffactura ( const QString &val ) {
        m_ffactura->setText ( val );
    };
    virtual void pintafemisionregistroiva ( const QString &val ) {
        m_femisionregistroiva->setText ( val );
    };
    virtual void pintaserieregistroiva ( const QString &val ) {
        m_serieregistroiva->setText ( val );
    };
    virtual void pintafactura ( const QString &val ) {
        m_factura->setText ( val );
    };
    virtual void pintaidborrador ( const QString & ) {}
    ;
    virtual void pintaregularizacion ( const QString & ) {}
    ;
    virtual void pintaplan349 ( const QString & ) {}
    ;
    virtual void pintanumorden ( const QString &val ) {
        m_numorden->setText ( val );
    };
    virtual void pintacif ( const QString &val ) {
        m_cif->setText ( val );
    };
    virtual void pintaidfpago ( const QString & ) {}
    ;
    virtual void pintafactemitida ( const QString &val ) {
        if ( val == "t" || val == "true" )
            m_factEmitida->setChecked ( true );
        else
            m_factSoportada->setChecked ( true );
    };
    virtual void pintarectificaaregistroiva ( const QString & ) {}
    ;
    virtual void pintaincregistro ( const QString &val ) {
        if ( val == "t" || val == "true" )
            m_incregistroIVA->setChecked ( true );
        else
            m_incregistroIVA->setChecked ( false );
    };

    virtual void recalculaIva() {
        BlFixed base = mui_listIva->sumarCampo ( "baseiva" );
        BlFixed iva = mui_listIva->sumarCampo ( "ivaiva" );
        setbaseimp ( base.toQString() );
        setiva ( iva.toQString() );
        pintabaseimp ( base.toQString() );
        pintaiva ( iva.toQString() );
    };
    virtual int save();
    int load ( QString id );
    virtual int remove() {
        return RegistroIva::remove();
    };

private:
    void generarPedidoCliente();
    void cargarComboFPago ( QString );

public slots:
    virtual void on_mui_generarPrevisiones_clicked();
    /// Este slot se activa cuando hay cambios en los subformularios.
    virtual void on_mui_listIva_editFinish ( int, int ) {
        BL_FUNC_DEBUG
        recalculaIva();
    };
};

#endif

