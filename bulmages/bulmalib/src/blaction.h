/****************************************************************************
**
** Copyright (C) 2010-2010 Tomeu Borras Riera. All rights reserved.
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

#ifndef BLACTION_H
#define BLACTION_H

#include <QToolButton>
#include <QAction>
#include "blmaincompanypointer.h"


class BL_EXPORT BlAction : public QAction, public BlMainCompanyPointer
{
    Q_OBJECT

public:
    BlAction ( const QString &text, QWidget *parent = 0 );
    BlAction ( BlMainCompany *,const QString &text, QWidget *parent = 0);
    virtual ~BlAction();

public slots:
    virtual void actionTriggered(bool trigg);
    
};

// BlAction * newBlAction(BlMainCompany *, QWidget *parent = 0, Qt::WFlags f = 0 );


#endif