/***************************************************************************
 *   Copyright (C) 2004 by Tomeu Borras Riera                              *
 *   tborras@conetxia.com                                                  *
 *   Copyright (C) 2006 by Fco. Javier M. C. (Porting to QT4)              *
 *   fcojavmc@todo-redes.com                                               *
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

#ifndef LISTTIPOIVAVIEW_H
#define LISTTIPOIVAVIEW_H

#include <QLineEdit>
#include <QLabel>
#include <QCheckBox>

#include "postgresiface2.h"
#include "busquedafecha.h"
#include "subform2bf.h"
#include "fichabf.h"


/// Muestra y administra las l&iacute;neas de detalle del listado de tipos de IVA.
/** */
class ListTipoIVASubForm : public SubForm2Bf
{
    Q_OBJECT

public:
    ListTipoIVASubForm ( QWidget *parent = 0 );
    virtual ~ListTipoIVASubForm() {};

public slots:
    virtual void cargar();
};


#include "ui_listtipoivabase.h"


/// Muestra y administra el listado de tipos de IVA.
/** */
class ListTipoIVAView : public FichaBf, public Ui_ListTipoIVABase
{
    Q_OBJECT
public:
    ListTipoIVAView ( Company *comp, QWidget *parent );
    ~ListTipoIVAView();
public slots:
    virtual void on_mui_aceptar_clicked();
};

#endif

