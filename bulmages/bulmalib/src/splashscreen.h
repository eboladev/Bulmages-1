/***************************************************************************
 *   Copyright (C) 2005 by Tomeu Borras Riera                              *
 *   tborras@conetxia.com                                                  *
 *   Copyright (C) 2006 by Fco. Javier M. C.                               *
 *   fcojavmc@todo-redes.com                                               *
 *   http://www.iglues.org                                                 *
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

#include <QDialog>
#include <QPixmap>
#include <QLabel>
#include <QTimer>
#include <QTextBrowser>
#include <QPalette>
#include <QProgressBar>
#include <QDesktopWidget>
#include <QFile>

#include <stdlib.h>
#include <stdio.h>


/// Muestra y administra la ventana de bienvenida al programa.
/** */
class Splash : public QDialog
{
    Q_OBJECT

private:
    QProgressBar *barra;
    QLabel *m_label, *m_label1, *l0;
    QPixmap *image0;

public:
    Splash ( QString appSplash, QString appName, QString appVersion );
    ~Splash();
    void mensaje ( QString );
    void setBarraProgreso ( int );

private slots:
    void barraprogreso();
};

#endif

