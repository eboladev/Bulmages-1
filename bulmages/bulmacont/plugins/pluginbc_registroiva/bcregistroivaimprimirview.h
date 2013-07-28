/***************************************************************************
 *   Copyright (C) 2003 by Tomeu Borras Riera                              *
 *   tborras@conetxia.com                                                  *
 *   Copyright (C) 2003 by Antoni Mirabete i Teres                         *
 *   amirabet@biada.org                                                    *
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

#ifndef BCREGISTROIVAIMPRIMIRVIEW_H
#define BCREGISTROIVAIMPRIMIRVIEW_H

#include <errno.h>

#include <QtWidgets/QWidget>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QRadioButton>

#include "blfunctions.h"
#include "ui_bcregistroivaimprimirbase.h"
#include "blwidget.h"


class BcCompany;


/// Trata el formulario de impresi&oacute;n del registro de IVA y genera los
/// listados pertinentes.
/** */
class BC_EXPORT BcRegistroIVAImprimirView : public QDialog, public Ui_BcRegistroIVAImprimirBase, public BlMainCompanyPointer
{
    Q_OBJECT

public:
    char *fichero;

public:
    BcRegistroIVAImprimirView ( BcCompany *, QWidget *parent = 0 );
    ~BcRegistroIVAImprimirView();
    void inicializa1 ( QString, QString );
    void presentar ( const char *tipus );

public slots:
    virtual void accept();
};

#endif

