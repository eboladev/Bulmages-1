/***************************************************************************
 *   Copyright (C) 2005 by Tomeu Borras Riera                              *
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

#ifndef BCASIENTOSUBFORM_H
#define BCASIENTOSUBFORM_H

#include "bldefs.h"
#include "bcsubform.h"
#include "blfixed.h"
#include "pdefs_pluginbc_asiento.h"


/** Implementa el subformulario de asientos contables.
    Deriva de BcSubForm.
*/
class PLUGINBC_ASIENTO_EXPORT BcAsientoSubForm : public BcSubForm
{
    Q_OBJECT

public:
    BcAsientoSubForm ( QWidget *parent = 0, const char *name = 0 );
    ~BcAsientoSubForm();
    virtual void load ( QString );
    BlFixed totalDebe ( QString );
    BlFixed totalHaber ( QString );

public slots:
    virtual void s_pintaMenu ( QMenu *menu );
    virtual void s_trataMenu ( QAction *action );
};

#endif

