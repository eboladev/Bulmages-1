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

#ifndef ALUMNOVIEW_H
#define ALUMNOVIEW_H

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


class PLUGINBF_ALUMNO_EXPORT ListAlumnosTutorView : public BfSubForm
{
    Q_OBJECT
    
public:
    ListAlumnosTutorView ( QWidget *parent = 0 );
    ~ListAlumnosTutorView() {};
    
public slots:
    virtual void load ( QString  );
};


class PLUGINBF_ALUMNO_EXPORT ListAlumnosActividadView : public BfSubForm
{
    Q_OBJECT
    
public:
    ListAlumnosActividadView ( QWidget *parent = 0 );
    ~ListAlumnosActividadView() {};
    
public slots:
    virtual void load ( QString  );
};


#include "ui_alumnobase.h"


/** Ventana de ficha de alumno.
    Se encarga de la presentacion de la ficha de alumno y del tratamiento de eventos producidos
    en dicha ventana **/
class PLUGINBF_ALUMNO_EXPORT AlumnoView : public BfForm, public Ui_AlumnoBase
{
    Q_OBJECT
    
private:
    /// La imagen de un alumno es una excepcion ya que no se guarda en la base de datos
    /// por ese motivo la almacenamos en esta variable que hace que corresponda con un archivo.
    QString m_archivoimagen;

public:
    AlumnoView ( BfCompany *, QWidget * );
    ~AlumnoView();
    virtual void imprimir();
    virtual QString templateName(void) ;
    virtual int afterSave();
    virtual int beforeDelete();
    virtual int cargarPost(QString id);
    virtual void pintarPost();
    
public slots:
    virtual void on_mui_cambiarimagen_clicked();
    virtual void on_mui_borrarimagen_clicked();
};

#endif

