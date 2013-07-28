/***************************************************************************
 *   Copyright (C) 2012 by Daniel Ripoll Osma.                             *
 *   info@danielripoll.es                                                  *
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

#ifndef CHOSEMAILER_H
#define CHOSEMAILER_H

#include <QtWidgets/QDialog>
#include <QtWidgets/QToolButton>
#include <QtWidgets/QFileDialog>
#include <QtCore/QUrl>
#include <QtCore/QDir>
#include <QtCore/QDirIterator>
#include <QtCore/QString>

#include "blfunctions.h"
#include "blconfiguration.h"
#include "ui_chosemailerbase.h"


/** Dialogo de eleccion de cliente de email.
**/
class ChoseMailer : public QDialog, public Ui_ChoseMailer
{
    Q_OBJECT

public:
    ChoseMailer(QWidget *parent = 0);
    ~ChoseMailer();
    QString programLocated;
    int SearchExecutable( QString &program);
    int GuardarConfig();

public slots:
    void on_mui_botonera_rejected();
    void on_mui_botonera_accepted();
    void on_mui_filechoser_clicked();
    void on_mui_mailclients_currentIndexChanged(int index);
    void on_mui_location_textChanged(const QString &arg1);

};

#endif // CHOSEMAILER_H
