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

#include <QtCore/QString>

#include "blfixed.h"
#include "bfform.h"
#include "bcasientosubform.h"
#include "bfcompany.h"
#include "pdefs_pluginbf_asiento.h"


/// Clase BcAsientoForm.
/** */
class PLUGINBF_ASIENTO_EXPORT BcAsientoForm : public BfForm
{
    Q_OBJECT

public:
    enum EstadoAsiento {AsientoVacio = 0, AsientoAbierto = 1, AsientoCerrado = 2};

protected:
    BcAsientoSubForm *m_listaLineas;

public:
    BcAsientoForm ( BfCompany *company, QWidget *parent );
    virtual ~BcAsientoForm();
    BfCompany *company();
    int load ( QString );
    BlFixed totalDebe ( QString );
    BlFixed totalHaber ( QString );
    void vaciar();
    void abrir();
    void cerrar();
    EstadoAsiento estadoBcAsientoForm();
    int save();
    void setIdAsiento ( QString val );
    QString idAsiento();
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

