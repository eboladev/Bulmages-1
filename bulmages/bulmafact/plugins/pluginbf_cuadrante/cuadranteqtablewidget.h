/***************************************************************************
 *   Copyright (C) 2003 by Tomeu Borras Riera                              *
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

#ifndef CUADRANTEQTABLEWIDGET_H
#define CUADRANTEQTABLEWIDGET_H

#include <QtCore/QEvent>
#include <QtWidgets/QTableWidget>
#include <QtWidgets/QTableWidgetItem>
#include <QtWidgets/QStyledItemDelegate>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMenu>
#include "blfunctions.h"
#include "blwidget.h"


class BfCompany;


/// Reimplementa los items de BlTableWidget para que podamos programar cosas en ellos.
class CuadranteQTextDocument : public QLabel,/* public QTableWidgetItem,*/  public BlMainCompanyPointer
{
    Q_OBJECT

public:
    QString mdb_idalmacen;
    QDate   mdb_fechacuadrante;
    QString mdb_idcuadrante;
    QMenu   *m_menu;
    QAction *m_accion;

public:
    CuadranteQTextDocument ( BfCompany *emp, QWidget *parent = 0 );
    virtual void setAlmFecha ( QString idalmacen, const QDate &date );
    ~CuadranteQTextDocument();
    void addTrabajador ( QString );
    void pintaCuadrante ( QString idalmacen, const QDate &date );
    QString idcuadrante();
    QDate fechacuadrante();
    QString idalmacen();
    void setidcuadrante ( QString );
    const QString impresion();
    void contextMenuEvent ( QContextMenuEvent * e );
    void mouseDoubleClickEvent ( QMouseEvent * event );
    void setText ( QString );

public slots:
    virtual void refresh();
    void on_customContextMenuRequested ( const QPoint & pos );
};


/// Clase que deriva de QTableWidget para poder reprogramar ciertos aspectos y
/// funcionalidades.
/** Se usa principalmente como tabla para mostrar subformularios.*/
class CuadranteQTableWidget : public QTableWidget
{
    Q_OBJECT

public:
    CuadranteQTableWidget ( QWidget *parent = 0 );
    ~CuadranteQTableWidget();

protected:
    virtual bool dropMimeData ( int row, int column, const QMimeData *data, Qt::DropAction action );
    virtual void dragEnterEvent ( QDragEnterEvent *event );
    virtual void dropEvent ( QDropEvent *event );
    virtual void dragMoveEvent ( QDragMoveEvent *event );

public slots:
    void on_contextMenuRequested ( int row, int col, const QPoint & pos );


};


/// Reimplementa los items de BlTableWidget para que podamos programar cosas en ellos.
class ImpCuadrante :  public BlMainCompanyPointer
{
public:
    QString mdb_idalmacen;
    QDate   mdb_fechacuadrante;
    QString mdb_idcuadrante;
    QString m_html;

public:
    ImpCuadrante ( BfCompany *emp );
    ~ImpCuadrante();
    void generar();
    bool buscaConflictos ( QString idtrabajador, const QDate &date, QString horain, QString horafin );

};


#endif

