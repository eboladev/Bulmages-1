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

#include <QLayout>
#include <QTabWidget>
#include <QPushButton>
#include <QCheckBox>
#include <QHBoxLayout>

#include "calendario.h"

calendario::calendario(class QDialog * parent) : QDialog(parent) {
    init();
}

calendario::~calendario() {
    dn->removeNonWorkingDayList(); /// remember to do this _before_ deleting the list!
    nwDays->clear(); /// autoDelete is set to true -> see ::init()
    delete nwDays; /// Finally, it's safe to delete.
    dn->removeEventDayList();
    eDays->clear();
    delete eDays;
    /// You shouldn't have to explicitely delete the QmcDateNav widget, as Qt should take care of it.
}


void calendario::init() {
    for (int t = 1; t < 6; t++)
        nonWorkDays[t] = false;
    nonWorkDays[6] = true;
    nonWorkDays[7] = true;

    dn = new QmcDateNav(this);

    eDays = new QList<QDate>;
    nwDays = new QList<QDate>;

    dn->installNonWorkingDayList(nwDays);
    dn->installEventDayList(eDays);

    updateNWDs(QDate::currentDate().year());
    updateEventDays(QDate::currentDate().year());

    mainLayout = new QHBoxLayout(this);
    mainLayout->setMargin(4);
    mainLayout->setSpacing(4);
    mainLayout->setObjectName("mainLayout");
    mainLayout->addWidget(dn);

    connect(dn, SIGNAL(yearChanged(int)), this, SLOT(updateNWDs(int)));
    connect(dn, SIGNAL(yearChanged(int)), this, SLOT(updateEventDays(int)));
    connect(dn, SIGNAL(close()), this, SLOT(close()));
}


/// TODO: Hacer que el programa importe de un fichero los dias no laborables oficiales.
void calendario::updateNWDs(int yr) {
    /// Note that there are a maximum of 10 days from the previous and next years that
    /// you need to cater for,
    /// I don't worry about those here, but you should.
    nwDays->clear();

    for (int t = 1; t < 8; t++) {
        if (nonWorkDays[t] == false)
            continue;
        QDate d(yr, 1, 1);
        while (d.dayOfWeek() != t) {
            d = d.addDays(1);
        }
        int cd = d.dayOfYear();
        int maxDays = d.daysInYear();
        while (cd <= maxDays) {
            nwDays->append(QDate(d));
            d = d.addDays(7);
            cd += 7;
        } // end while
    } // end for
    /// Add stuff from the diary here.
    dn->forceUpdate();
}


/// TODO: Hacer que el programa importe de un fichero los eventos
/// (como plazos de entrega de declaraciones, etc.).
void calendario::updateEventDays(int yr) {
    eDays->clear();
    eDays->append(QDate(yr, 1, 1));
    eDays->append(QDate(yr, 1, 6)); /// My birthday ;-)
    eDays->append(QDate(yr, 12, 25));
    eDays->append(QDate(1999, 8, 24)); /// Release of QmcDateNav 0.0.2
    dn->forceUpdate();
}


void calendario::showOptions() {
    if (optionsDialog)
        optionsDialog->show();
    else
        return;
    if (qcbOutlook->isChecked() != dn->outlook())
        dn->setOutlook(! dn->outlook());
    if (qcbFrame->isChecked() != dn->frame())
        dn->setFrame(! dn->frame());
    if (qcbWorkMon->isChecked() != nonWorkDays[1])
        nonWorkDays[1] = qcbWorkMon->isChecked();
    if (qcbWorkTue->isChecked() != nonWorkDays[2])
        nonWorkDays[2] = qcbWorkTue->isChecked();
    if (qcbWorkWed->isChecked() != nonWorkDays[3])
        nonWorkDays[3] = qcbWorkWed->isChecked();
    if (qcbWorkThu->isChecked() != nonWorkDays[4])
        nonWorkDays[4] = qcbWorkThu->isChecked();
    if (qcbWorkFri->isChecked() != nonWorkDays[5])
        nonWorkDays[5] = qcbWorkFri->isChecked();
    if (qcbWorkSat->isChecked() != nonWorkDays[6])
        nonWorkDays[6] = qcbWorkSat->isChecked();
    if (qcbWorkSun->isChecked() != nonWorkDays[7])
        nonWorkDays[7] = qcbWorkSun->isChecked();
    updateNWDs(dn->selectedDates().at(0).year());
    dn->forceUpdate();
}

