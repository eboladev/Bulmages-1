/***************************************************************************
 *   Copyright (C) 2006 by Tomeu Borras Riera                              *
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

#ifndef BCASIENTOFORM_H
#define BCASIENTOFORM_H

#include <QString>

#include "blfixed.h"
#include "bcform.h"
#include "bcasientosubform.h"
#include "bccompany.h"
#include "pdefs_pluginbc_asiento.h"


/// Clase BcAsientoForm.
/** */
class PLUGINBC_ASIENTO_EXPORT BcAsientoForm : public BcForm
{
    Q_OBJECT

public:
    enum estadoasiento {ASVacio = 0, ASAbierto = 1, ASCerrado = 2};

protected:
    BcAsientoSubForm *listalineas;

public:
    BcAsientoForm ( BcCompany *, QWidget *parent );
    virtual ~BcAsientoForm();
    BcCompany *companyact();
    int load ( QString );
    BlFixed totaldebe ( QString );
    BlFixed totalhaber ( QString );
    void vaciar();
    void abrir();
    void cerrar();
    estadoasiento estadoBcAsientoForm();
    int save();
    void setidasiento ( QString val );
    QString idasiento();
    /// Establece cual es la lista subformulario del presupuesto.
    void setListLinAsiento1 ( BcAsientoSubForm *a );
    virtual int remove ( bool );
    virtual void pintaIdAsiento ( QString );
    virtual void pintaDescripcion ( QString );
    virtual void pintaFecha ( QString );
    virtual void pintaComentariosAsiento ( QString );
    virtual void pintaOrdenAsiento ( QString );
    virtual void pintaClase ( QString );
    virtual void calculaPintaTotales();
    virtual void pintar();
    virtual void trataEstadoBcAsientoForm();
};

#endif

