/***************************************************************************
 *   Copyright (C) 2009 by Tomeu Borras Riera                              *
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

#ifndef CONVREUNIONVIEW_H
#define CONVREUNIONVIEW_H

#include <QtWidgets/QLineEdit>
#include <QtWidgets/QLabel>
#include <QtWidgets/QCheckBox>

#include "blpostgresqlclient.h"
#include "bldatesearch.h"
#include "bldialogchanges.h"
#include "bfform.h"
#include "pdefs_pluginbf_asociacion.h"


class BfCompany;


extern "C++" class BusquedaProfesor;


#include "ui_convreunionbase.h"


/** Ventana de ficha de reunion.
    Se encarga de la presentacion de la ficha de reunion y del tratamiento de eventos producidos
    en dicha ventana. **/
class PLUGINBF_ASOCIACION_EXPORT ConvReunionView : public BfForm, public Ui_ConvReunionBase
{
    Q_OBJECT

public:
    ConvReunionView ( BfCompany *, QWidget * );
    ~ConvReunionView();
    virtual void imprimir();
    virtual QString templateName(void) ;
    virtual int afterSave();
    virtual int beforeDelete();
    virtual int cargarPost(QString );
    virtual int junta();
    
public slots:
    virtual void on_mui_email_clicked();
    virtual void on_m_agregarconvocados_clicked();
};

#endif

