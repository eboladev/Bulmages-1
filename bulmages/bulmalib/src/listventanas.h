/***************************************************************************
 *   Copyright (C) 2005 by Tomeu Borras Riera                              *
 *   tborras@conetxia.com                                                  *
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

#ifndef __LISTVENTANAS__
#define __LISTVENTANAS__

#include <QDockWidget>
#include <QListWidget>

#include "blworkspace.h"


class QListWidget1 : public QListWidget
{
    Q_OBJECT

public:
    QListWidget1 ( QWidget * parent = 0 );
    ~QListWidget1();
};


class QListWidgetItem1 : public QListWidgetItem
{
private:
    QObject *m_obj;
    QString m_nombre;
    QListWidget1 *m_list;

public:
    QListWidgetItem1 ( QListWidget1 *l, QPixmap &p );
    ~QListWidgetItem1();
    void setObject ( QObject *m );
    void setNombre ( QString m );
    QObject *object();
    QString nombre();
};


/// Provides the dock window to list all windows in BulmaFact
/// \param m_listBox this variable is the listBox that contains all
/// titles of the diferent windows.
class ListVentanas : public QDockWidget
{
    Q_OBJECT

private:
    QListWidget1 *m_listBox;
    BlWorkspace *m_pWorkspace;

public:
    void setWorkspace ( BlWorkspace *w );
    ListVentanas ( QWidget *a = 0 );
    ~ListVentanas();

public:
    int meteWindow ( QString, QObject *, bool compdup = TRUE );
    int seleccionaWindow ( QString, QObject * );
    int deSeleccionaWindow();
    void sacaWindow ( QObject * );
    void cambiaVisible ( bool );
    void vaciar();
    void vaciarCompleto();
    int numVentanas();
    QObject *ventana ( int index );

protected:
    void closeEvent ( QCloseEvent *event );

public slots:
    virtual void dclicked();
    virtual void clicked();

signals:
    void cambiaEstadoVisible ( bool );
};

#endif

