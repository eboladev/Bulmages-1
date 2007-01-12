/***************************************************************************
 *   Copyright (C) 2004 by Marcos Mezo                                     *
 *   mmezo@selexco.net                                                     *
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

#ifndef CLIENTEVIEW_H
#define CLIENTEVIEW_H

#include "cliente.h"
#include "ficha.h"
#include "ui_clientebase.h"


class company;


/** Clase que se encarga de la pantalla de cliente.
    Deriva de la clase Ficha para la estandarizacion de pantallas y
    de la clase Cliente para el manejo con la base de datos.
    Se encarga de recibir y lanzar eventos.
*/
class ClienteView : public Ficha, public Ui_ClienteBase, public Cliente {
    Q_OBJECT
public:
    ClienteView(company *emp, QWidget *parent = 0);
    ~ClienteView();

public:
    int cargar(QString client);
    virtual int guardar();
    void deleteClient();
    void emptyForm();
    void pintaidcliente(QString) {}
    void pintanomcliente(QString val) {
        m_nomcliente->setText(val);
    }
    void pintanomaltcliente(QString val) {
        m_nomaltcliente->setText(val);
    }
    void pintacifcliente(QString val) {
        m_cifcliente->setText(val);
    }
    void pintacodcliente(QString val) {
        mui_codcliente->setText(val);
    }
    void pintacorpcliente(QString val) {
        mui_corpcliente->setText(val);
    }
    void pintabancocliente(QString val) {
        m_bancocliente->setText(val);
    }
    void pintadircliente(QString val) {
        m_dircliente->setText(val);
    }
    void pintapoblcliente(QString val) {
        m_poblcliente->setText(val);
    }
    void pintacpcliente(QString val) {
        m_cpcliente->setText(val);
    }
    void pintatelcliente(QString val) {
        m_telcliente->setText(val);
    }
    void pintateltrabcliente(QString val) {
        mui_teltrabcliente->setText(val);
    }
    void pintamovilcliente(QString val) {
        mui_movilcliente->setText(val);
    }
    void pintafaxcliente(QString val) {
        m_faxcliente->setText(val);
    }
    void pintamailcliente(QString val) {
        m_mailcliente->setText(val);
    }
    void pintaurlcliente(QString val) {
        m_urlcliente->setText(val);
    }
    void pintafaltacliente(QString) {}
    void pintafbajacliente(QString) {}
    void pintacomentcliente(QString val) {
        m_comentcliente->setPlainText(val);
    }
    void pintainactivocliente(QString) {}
    void pintaprovcliente(QString val) {
        m_provcliente->setProvincia(val);
    };
    void pintaregimenfiscalcliente(QString val) {
	mui_regimenfiscalcliente->setRegimenFiscal(val);
    };
    void pintarecargoeqcliente(QString val) {
	/// Pintamos el recargo de equivalencia
	if (val == "t") {
		mui_recargoeqcliente->setChecked(TRUE);
	} else {
		mui_recargoeqcliente->setChecked(FALSE);
	} // end if
    };
    void pintaidforma_pago(QString val) {
	mui_forma_pago->setidforma_pago(val);
    };

    virtual int sacaWindow();

public slots:
    /// \TODO: Muchos metodos aqui implementados deberian estar en Ficha.
    virtual void on_mui_guardar_clicked();
    virtual void on_mui_borrar_clicked();
    virtual void on_mui_aceptar_clicked() {
        if (!guardar()) {
            close();
        } // end if
    }
    virtual void on_mui_informe_clicked();
    virtual void on_mui_imprimir_clicked() {
        imprimir();
    }
};

#endif

