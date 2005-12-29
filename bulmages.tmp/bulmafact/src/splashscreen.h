/***************************************************************************
                          splashscreen.h  -  description
                             -------------------
    begin                : Wed Nov 02 2005
    copyright            : (C) 2005 by Tomeu Borr� Riera
    email                : tborras@conetxia.com
 ***************************************************************************/
/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/
#ifndef _SPLASH_H
#define _SPLASH_H

#include <qdialog.h>
#include <qpixmap.h>
#include <qstring.h>
#include <q3progressbar.h>
#include <qlabel.h>
//Added by qt3to4:
#include <QEvent>
#include <stdio.h>
#include <qlayout.h>
#include <qlabel.h>
#include <q3progressbar.h>
#include <qmessagebox.h>
#include <qpixmap.h>
#include <stdlib.h>
#include <qtimer.h>
#include <q3textbrowser.h>

class Splash : public QDialog
{
  Q_OBJECT
private:
 QLabel *l;
// QLabel *l2;
Q3TextBrowser *l2;
public:
   Splash();
   ~Splash();
public slots:
   void paint();
   bool event( QEvent *);
};
#endif // _SPLASH_H
