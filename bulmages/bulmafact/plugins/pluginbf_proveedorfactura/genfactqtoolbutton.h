/***************************************************************************
 *   Copyright (C) 2007 by Tomeu Borras Riera                              *
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

#include <QtXml/QDomDocument>
#include <QtCore/QMap>
#include <QtCore/QString>
#include <QtWidgets/QWidget>
#include <QtWidgets/QToolButton>
#include <QtCore/QString>
#include "bfcompany.h"
#include "blfunctions.h"


class GenFacProQToolButton : public QToolButton
{
    Q_OBJECT

private:
    BfCompany *m_companyact;
    QWidget *m_object;

public:
    GenFacProQToolButton ( QWidget *fac,   QWidget *parent = NULL );
    ~GenFacProQToolButton();
    void setBoton();
    void generarFacturaProveedor();
    void generarFacturaProveedor1();

public slots:
    virtual void click();

};

