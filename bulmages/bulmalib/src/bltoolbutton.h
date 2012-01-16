/****************************************************************************
**
** Copyright (C) 2010-2010 Tomeu Borrás Riera. All rights reserved.
**
** This file is part of the documentation of the Qt Toolkit.
**
** This file may be used under the terms of the GNU General Public
** License version 2.0 as published by the Free Software Foundation
** and appearing in the file LICENSE.GPL included in the packaging of
** this file.  Please review the following information to ensure GNU
** General Public Licensing requirements will be met:
** http://www.trolltech.com/products/qt/opensource.html
**
** If you are unsure which license is appropriate for your use, please
** review the following information:
** http://www.trolltech.com/products/qt/licensing.html or contact the
** sales department at sales@trolltech.com.
**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
**
****************************************************************************/

#ifndef BLTOOLBUTTON_H
#define BLTOOLBUTTON_H

#include <QToolButton>
#include <QWidget>
#include "blmaincompanypointer.h"
#include "blmaincompany.h"


class BL_EXPORT BlToolButton : public QToolButton, public BlMainCompanyPointer
{
    Q_OBJECT

public:
    BlToolButton ( QWidget *parent = 0 );
    BlToolButton ( BlMainCompany *company, QWidget *parent = 0);
    virtual ~BlToolButton();

public slots:
    virtual void buttonReleased();
};

#endif

