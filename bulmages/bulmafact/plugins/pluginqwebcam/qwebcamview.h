/***************************************************************************
 *   Copyright (C) 2004 by Alvaro de Miguel                                *
 *   alvaro_demiguel@gmail.com                                             *
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

#ifndef QWEBCAMVIEW_H
#define QWEBCAMVIEW_H

#include "ui_qwebcambase.h"
#include "blpostgresqlclient.h"
#include "dialogchanges.h"
#include "dbrecord.h"


class Company;

class QWebCamView : public QWidget, public Ui_QWebCamBase, public dialogChanges, public DBRecord
{
    Q_OBJECT

private:
    Company *m_companyact;

public:
    QWebCamView ( Company *emp, QWidget *parent = 0 );
    ~QWebCamView();
    Company *companyact() {
        return m_companyact;
    };

public:

    int cargar ( QString );

};

#endif

