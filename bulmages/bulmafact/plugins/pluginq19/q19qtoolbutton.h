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
#ifndef IMPQTOOLBUTTON_H
#define IMPQTOOLBUTTON_H

#include <QWidget>
#include <QToolButton>
#include <QString>
#include "company.h"
#include "blfunctions.h"
#include <QtXml/QDomDocument>
#include <QMap>
#include <QString>
#include <QDataStream>
#include "cobroslist.h"


class Q19QToolButton : public QToolButton
{
    Q_OBJECT

private:
    Company *m_companyact;
    CobrosList     *m_cobrosList;

public:
    Q19QToolButton ( CobrosList *cob = NULL, QWidget *parent = NULL );
    ~Q19QToolButton();
    void setBoton();
    QByteArray cobroQ19 ( QDataStream &, QString );
    QByteArray cabeceraPresentador ( QDataStream &, QString );
    QByteArray cabeceraOrdenante ( QDataStream &, QString );
    QByteArray totalOrdenante ( QDataStream &, QString, QString, QString );
    QByteArray totalGeneral ( QDataStream &, QString, QString, QString );

public slots:
    virtual void click();

};


#endif
