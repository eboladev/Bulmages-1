/***************************************************************************
 *   Copyright (C) 2005 by Tomeu Borras Riera                              *
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

#ifndef _SPLASH_H
#define _SPLASH_H

#include <qdialog.h>
#include <QPixmap>
#include <QString>
#include <q3progressbar.h>
#include <QLabel>
#include <QEvent>
#include <stdio.h>
#include <QLayout>
#include <QLabel>
#include <q3progressbar.h>
#include <QMessageBox>
#include <QPixmap>
#include <qtimer.h>
#include <q3textbrowser.h>

#include <stdlib.h>


class Splash : public QDialog
{
	Q_OBJECT

private:
	QLabel *l;
	//QLabel *l2;
	Q3TextBrowser *l2;

public:
	Splash();
	~Splash();

public slots:
	void paint();
	bool event(QEvent *);
};

#endif
