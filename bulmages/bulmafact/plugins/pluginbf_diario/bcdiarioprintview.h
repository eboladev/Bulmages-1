/***************************************************************************
 *   Copyright (C) 2003 by Tomeu Borras Riera                              *
 *   tborras@conetxia.com                                                  *
 *   Copyright (C) 2003 by Antoni Mirabete i Teres                         *
 *   amirabet@biada.org                                                    *
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

#ifndef BCDIARIOPRINTVIEW_H
#define BCDIARIOPRINTVIEW_H

#include <errno.h>

#include <QWidget>
#include <QToolButton>
#include <QRadioButton>

#include "blfunctions.h"
#include "ui_bcdiarioprintbase.h"
#include "blwidget.h"
#include "pdefs_pluginbf_diario.h"

/// Declaramos la clase empresa como amiga para que la compilaci&oacute; no sea complicada.
#include "bfcompany.h"



/// Ventana de impresi&oacute; de diarios.
/// @class BcDiarioPrintView bcdiarioprintview.h
/** Es la ventana de impresi&oacute; de diarios. */
class PLUGINBC_DIARIO_EXPORT BcDiarioPrintView : public QDialog, public Ui_BcDiarioPrintBase, BlMainCompanyPointer
{
    Q_OBJECT

public:
    int numDigitos;
    /// Puntero al filtrado de datos.

public:
    /// Establece cual es la ventana de filtro de diario. Es imprescindible
    /// inicializar el filtro antes de mostrar la ventana o el bot&oacute;n de filtro
    /// no funcionar&aacute;.
    BcDiarioPrintView ( BfCompany *emp, QWidget *parent );
    ~BcDiarioPrintView();
    void presentar ( const char *tipus = "html" );

private:
    QString montaQuery();

public slots:
    virtual void accept();
};

#endif

