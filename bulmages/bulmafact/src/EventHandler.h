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

#ifndef EVENTHANDLER_H
#define EVENTHANDLER_H

#include <Q3Table>
#include <QObject>
#include <QEvent>

#define QUANTITY 1
#define DATE 2


class EventHandler : public QObject {
    Q_OBJECT

public:
    EventHandler(QWidget *, const int);
    EventHandler(QWidget *, Q3Table *);
    ~EventHandler();
    bool eventFilter(QObject *, QEvent *);
    //int const QUANTITY = 1;
    //int const DATE = 2;

private:
    void nextCell(QObject *);
    void duplicateCell(QObject *);
    int m_TextEditType;
    void manageDate(QObject *);
    void manageQuantity(QObject *);
    //QString * columnFunctions[];

protected:
    //bool eventFilter(QObject *obj, QEvent *ev);
};

#endif

