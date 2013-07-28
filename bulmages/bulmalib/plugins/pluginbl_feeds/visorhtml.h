/***************************************************************************
 *   Copyright (C) 2011 by Tomeu Borras Riera                              *
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

#ifndef FEEDSWIDGET_H
#define FEEDSWIDGET_H

#include <QtWidgets/QDockWidget>
#include <QtCore/QString>
#include <QtCore/QObject>
#include <QtWidgets/QWidget>

#include "blfunctions.h"
#include "ui_visorhtmlbase.h"
#include "pdefs_pluginbl_feeds.h"

/** Ver documentos en formato HTML. */
class PLUGINBL_FEEDS_EXPORT visorhtml : public QWidget, public Ui_visorhtmlbase
{
    Q_OBJECT

public:
    /// El informe generado se forma en HTML y se presenta mediante este QString.
    QString textBrowser;
    /// El corrector es una ventana del tipo dock, que se puede anexar a las esquinas
    /// del worspace de la aplicacion. Este puntero apunta a la ventana contenedora del
    /// corrector.
    QDockWidget *dock;

public:
    visorhtml ( QWidget* parent = 0, Qt::WindowFlags fl = 0 );
    ~visorhtml();

public slots:
    virtual void on_webView_linkClicked(const QUrl &url);
    virtual void sloadFinished(bool ok);
    virtual void on_webView_loadFinished(bool ok);
};

#endif
