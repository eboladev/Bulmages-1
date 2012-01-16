/***************************************************************************
 *   Copyright (C) 2003 by Tomeu Borras Riera                              *
 *   tborras@conetxia.com                                                  *
 *   http://www.iglues.org Asociación Iglues -- Contabilidad Linux         *
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

#ifndef BCTIPOIVAVIEW_H
#define BCTIPOIVAVIEW_H

#include "blpostgresqlclient.h"
#include "bldialogchanges.h"
#include "ui_bctipoivabase.h"
#include "bcform.h"


class BcCompany;


/// Esta clase hace el mantenimiento de la tabla de Tipos de IVA.
/** Su funcionamiento se basa un poco en el funcionamiento de la pantalla de canales. */
class BC_EXPORT BcTipoIVAView : public BcForm, public Ui_BcTipoIVABase
{
    Q_OBJECT

private:
    BlDbRecordSet *m_curtipoiva;
    /// Indica la posici&oacute;n en el combobox que est&aacute; seleccionada.
    /// Se usa para guardar en caso de modificaciones.
    int m_posactual;

public:
    BcTipoIVAView ( BcCompany *, QWidget *parent = 0 );
    ~BcTipoIVAView();

private:
    void pintar ( QString idtipoiva = "" );
    void mostrarPlantilla ( int pos = 0 );

private slots:
    virtual void on_mui_comboTipoIVA_currentIndexChanged ( int );
    virtual void on_mui_crear_clicked();
};

#endif
