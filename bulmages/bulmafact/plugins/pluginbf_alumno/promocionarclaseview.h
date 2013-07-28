/***************************************************************************
 *   Copyright (C) 2004 by Tomeu Borras Riera                              *
 *   tborras@conetxia.com                                                  *
 *   http://www.iglues.org                                                 *
 *                                                                         *
 *   Copyright (C) 2009 by Arturo Martin Llado                             *
 *   amartin@conetxia.com                                                  *
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

#ifndef PROMOCIONARCURSO_H
#define PROMOCIONARCURSO_H

#include <QtWidgets/QLineEdit>
#include <QtWidgets/QLabel>
#include <QtWidgets/QCheckBox>

#include "blfunctions.h"
#include "blpostgresqlclient.h"
#include "bldatesearch.h"
#include "bldialogchanges.h"
#include "bfform.h"
#include "pdefs_pluginbf_alumno.h"


class BfCompany;




#include "ui_promocionarclasebase.h"


/** Ventana de ficha de alumno.
    Se encarga de la presentacion de la ficha de alumno y del tratamiento de eventos producidos
    en dicha ventana **/
class PLUGINBF_ALUMNO_EXPORT PromocionarClaseView : public BfForm, public Ui_PromocionarClaseBase
{
    Q_OBJECT
    

public:
    PromocionarClaseView ( BfCompany *, QWidget * );
    ~PromocionarClaseView();
    
public slots:
    virtual void on_mui_acept_released();

};

#endif

