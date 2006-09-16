/***************************************************************************
 *   Copyright (C) 2003 by Tomeu Borras Riera                              *
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

#ifndef CALENDARIO_H
#define CALENDARIO_H

#include <QDialog>
#include <QDateTime>
#include <Q3PtrList>
#include <QHBoxLayout>

#include "qmcdatenav.h"


class QmcDateNav;
class QHBoxLayout;
class Q3TabDialog;
class QPushButton;
class QCheckBox;
class Q3VBox;

/// \bug Se produce una violacion de segmento si nos desplazamos con los cursores
/// hacia otro mes.
class calendario: public QDialog {
    Q_OBJECT

public:
    calendario(class QDialog * parent = 0);
    ~calendario();
    QmcDateNav *dn;
    Q3PtrList<QDate> *eDays;

public slots:
    void updateEventDays(int);
    void updateNWDs(int);

private slots:
    void showOptions();

private:
    void init();
    QHBoxLayout *mainLayout;
    Q3PtrList<QDate> *nwDays;
    Q3TabDialog *optionsDialog;
    QWidget *optionsWidget;
    QPushButton *optionsButton;
    /// Pages...
    Q3VBox *generalOptions;
    Q3VBox *nwdOptions;
    QCheckBox *qcbOutlook;
    QCheckBox *qcbFrame;
    QCheckBox *qcbWorkMon;
    QCheckBox *qcbWorkTue;
    QCheckBox *qcbWorkWed;
    QCheckBox *qcbWorkThu;
    QCheckBox *qcbWorkFri;
    QCheckBox *qcbWorkSat;
    QCheckBox *qcbWorkSun;
    bool nonWorkDays[8];
    /// TBR 12/06/03
    // bool calendario::eventFilter(QObject *, QEvent *);
};

#endif

