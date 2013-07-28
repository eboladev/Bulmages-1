/***************************************************************************
 *   Copyright (C) 2002 by Tomeu Borras Riera                              *
 *   tborras@conetxia.com                                                  *
 *   http://www.iglues.org Asociacion Iglues -- Contabilidad Linux         *
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

#ifndef BCABOUTVIEW_H
#define BCABOUTVIEW_H

#include <QtWidgets/QWidget>

#include "blfunctions.h"
#include "ui_bcaboutbase.h"
#include "blform.h"


/// Presenta la informaci&oacute;n sobre el programa.
/** Esta clase muestra el di&aacute;logo de About. Presentan los cr&eacute;ditos del programa. */
class BC_EXPORT BcAboutView : public QDialog, public Ui_BcAboutBase
{
    Q_OBJECT

public:
    BcAboutView ( QWidget *parent = 0 ); /// Constructor de la clase.
    ~BcAboutView(); /// Destructor de la clase.

private slots:
    void on_mui_aceptar_clicked();
};

#endif

