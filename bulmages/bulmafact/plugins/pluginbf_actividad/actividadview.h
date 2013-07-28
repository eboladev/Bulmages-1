/***************************************************************************
 *   Copyright (C) 2009 by Tomeu Borras Riera                              *
 *   tborras@conetxia.com                                                  *
 *   http://www.iglues.org                                                 *
 *                                                                         *
 *   Copyright (C) 2009 by Arturo Martin Llado                             *
 *   amartin@conetxia.com                                                  *
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

#ifndef ACTIVIDADVIEW_H
#define ACTIVIDADVIEW_H

#include <QtWidgets/QLineEdit>
#include <QtWidgets/QLabel>
#include <QtWidgets/QCheckBox>

#include "blpostgresqlclient.h"
#include "bldatesearch.h"
#include "bldialogchanges.h"
#include "bfform.h"
#include "pdefs_pluginbf_actividad.h"


class BfCompany;


extern "C++" class BusquedaProfesor;

/// Muestra y administra los alumnos de una actividad.
/** */
class PLUGINBF_ACTIVIDAD_EXPORT ListAlumnosActividadView : public BfSubForm
{
    Q_OBJECT

public:
    ListAlumnosActividadView ( QWidget *parent = 0 );
    ~ListAlumnosActividadView() {};
    
public slots:
    virtual void load ( QString idactividad );
};


/// Muestra y administra los alumnos de una actividad.
/** */
class PLUGINBF_ACTIVIDAD_EXPORT ListTutoresActividadView : public BfSubForm
{
    Q_OBJECT

public:
    ListTutoresActividadView ( QWidget *parent = 0 );
    ~ListTutoresActividadView() {};
    
public slots:
    virtual void load ( QString idactividad );
};



/// Muestra y administra las faltas de asistencia de una actividad.
/** */
class PLUGINBF_ACTIVIDAD_EXPORT ListFaltasAsistenciaActividadView : public BfSubForm
{
    Q_OBJECT

public:
    ListFaltasAsistenciaActividadView ( QWidget *parent = 0 );
    ~ListFaltasAsistenciaActividadView() {};
    
public slots:
    virtual void load ( QString idactividad );
};


#include "ui_actividadbase.h"


/** Ventana de ficha de actividad.
    Se encarga de la presentacion de la ficha de actividad y del tratamiento de eventos producidos
    en dicha ventana. **/
class PLUGINBF_ACTIVIDAD_EXPORT ActividadView : public BfForm, public Ui_ActividadBase
{
    Q_OBJECT

public:
    ActividadView ( BfCompany *, QWidget * );
    ~ActividadView();
    virtual void imprimir();
    virtual QString templateName(void) ;
    virtual int afterSave();
    virtual int beforeDelete();
    virtual int cargarPost(QString );
};

#endif

