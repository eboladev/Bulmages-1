/***************************************************************************
 *   Copyright (C) 2006 by Tomeu Borras Riera                              *
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

#ifndef FICHABC_H
#define FICHABC_H

#include "ficha.h"
#include "subform2bc.h"
#include "dbrecord.h"
#include "empresa.h"


/** Esta clase es una plantilla para las ventanas de BulmaCont. Contiene metodos
    comunes a todas las pantallas.
*/
class FichaBc : public Ficha, public DBRecord {
    Q_OBJECT

public:
    /// Puntero al subformulario principal de la ventana (si no tiene debe estar a NULL)
    SubForm2Bc *m_listalineas;
    /// Puntero a la clase company para poder trabajar con la base de datos y hacer
    /// traspaso de mensajes.
    empresa *empresaBase();
    FichaBc(empresa *comp, QWidget *parent = 0, Qt::WFlags f = 0);
    virtual ~FichaBc();
    void setListaLineas(SubForm2Bc * form);
    SubForm2Bc* listalineas();
    virtual int sacaWindow();
    virtual void meteWindow(QString nom, QObject *obj);
};

#endif

