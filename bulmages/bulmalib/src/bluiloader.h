/***************************************************************************
 *   Copyright (C) 2011 by Tomeu Borras Riera                              *
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
/*
Esta clase es un cargador de interficies graficas. Es una derivacion del cargador por defecto de Qt con la salvedad de
iniciar todos los componentes con la base de datos a utilizar.
Como todos los elementos derivan de BlWidget solo hay que utilizar el metodo setMainCompany en todos ellos y asi quedan preparados
para trabajar con la base de datos.
*/
#ifndef BLUILOADER_H
#define BLUILOADER_H

#include <QtWidgets/QWidget>
#include <QtUiTools/QUiLoader>
#include "blform.h"
#include "blformlist.h"


class BL_EXPORT BlUiLoader : public QUiLoader
{
    Q_OBJECT

private:
    BlMainCompany *m_company;

public:
    BlUiLoader ( BlMainCompany *company, QObject *parent = 0 );
    virtual ~BlUiLoader ();
    virtual QWidget *createWidget ( const QString &className, QWidget *parent = 0, const QString &name = QString() );
};

#endif

