/***************************************************************************
 *   Copyright (C) 2005 by Tomeu Borras Riera                              *
 *   tborras@conetxia.com                                                  *
 *   Copyright (C) 2010 by Fco. Javier M. C.                               *
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

#ifndef BLWINDOWLISTDOCK_H
#define BLWINDOWLISTDOCK_H

#include <QtWidgets/QDockWidget>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QListWidgetItem>
#include <QtGui/QPainter>
#include <QtWidgets/QAbstractItemDelegate>

#include "blfunctions.h"
#include "blworkspace.h"


class BlWorkspace;


  
class ListDelegate : public QAbstractItemDelegate
{
public:
ListDelegate(QObject *parent = 0);
  
void paint ( QPainter * painter, const QStyleOptionViewItem & option, const QModelIndex & index ) const;
QSize sizeHint ( const QStyleOptionViewItem & option, const QModelIndex & index ) const;
  
virtual ~ListDelegate();
  
};
    

class BL_EXPORT BlListWidget : public QListWidget
{
Q_OBJECT

public:
    BlListWidget ( QWidget * parent = 0 );
    ~BlListWidget();
};


class BL_EXPORT BlListWidgetItem : public QListWidgetItem
{
  
private:
    QObject *m_obj;
    QString m_nombre;
    QString m_titulo;
public:
    bool m_delete;

public:
    BlListWidgetItem ( BlListWidget *l, QPixmap &p, const QString &title="", const QString &desc="" );
    ~BlListWidgetItem();
    void setObject ( QObject *m );
    void setName ( QString m );
    void setTitle (QString titulo);
    void setDesc (const QString &desc );
    QObject *object();
    QString name();

};


/// Provides the dock window to list all windows in BulmaFact
/// \param m_listBox this variable is the listBox that contains all
/// titles of the diferent windows.
class BL_EXPORT BlWindowListDock : public QDockWidget
{
    Q_OBJECT

private:
    BlListWidget *m_listBox;
    BlWorkspace *m_pWorkspace;

public:
    void setWorkspace ( BlWorkspace *w );
    BlWindowListDock ( QWidget *a = 0 );
    ~BlWindowListDock();

public:
    int insertWindow ( QString name, QObject *object, bool checkDuplication = true, QString title="");
    int selectWindow ( QString nombre, QMdiSubWindow *obj );
    int deselectWindow();
    void removeWindow ( QObject * );
    void setVisibilityState ( bool );
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
    void visibilityStateChanged ( bool );
};

#endif

