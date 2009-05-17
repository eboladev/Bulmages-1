/***************************************************************************
 *   Copyright (C) 2002 by Tomeu Borras Riera                              *
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

#ifndef DATOSVIEW_H
#define DATOSVIEW_H

#include <QWidget>
#include <QDialog>

#include "blform.h"
#include "ui_datosbase.h"
#include "blprogressbar.h"

class DatosView : public BlForm, public Ui_datosBase
{
    Q_OBJECT
public:
    QString m_informe;
    bool resultado; 
public:
    DatosView ( BlMainCompany *emp = NULL, QWidget *parent = 0, Qt::WFlags f = 0, edmode modo = BL_EDIT_MODE);
    ~DatosView();
    virtual void on_mui_aceptar_clicked();
    virtual void on_mui_cancelar_clicked();

// mui_fechainicialactual
// mui_fechafinalactual
// mui_fechainicialanterior
// mui_fechafinalanterior
// Message from Tomeu at 20:38:21 
// on_fechainicial_valueChanged( QString fecha)
// 
//  
// Message from Tomeu at 20:38:31 
// on la variable fecha conté el valor actual del camp
// 
//  
// Message from lepalom@jabber.cc at 20:38:55 
// i com es diu el valor del camp?
//  
// Message from Tomeu at 20:39:01 
// i se dispara cada cop que el valor del camp canvia
// 
//  
// Message from Tomeu at 20:39:24 
// fechainicial->text()
// 
//  
// Message from Tomeu at 20:39:35 
// també funciona fechainicial->date()
// 
// 

};

#endif

