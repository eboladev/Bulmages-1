/***************************************************************************
                          calendario.h  -  description
                             -------------------
    begin                : sáb may 31 2003
    copyright            : (C) 2003 by Tomeu Borrás Riera
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

#ifndef CALENDARIO_H
#define CALENDARIO_H


/***@author Tomeu Borrás Riera */
#include "qmcdatenav.h"

#include <qdialog.h>
#include <qdatetime.h>
#include <qlist.h>

class QmcDateNav;
class QHBoxLayout;
class QTabDialog;
class QPushButton;
class QCheckBox;
class QVBox;


class calendario: public QDialog
{
    Q_OBJECT

public:
    calendario( class QDialog * parent = 0, const char * name = 0 );
    ~calendario();
    QmcDateNav *dn;
    QList<QDate> *eDays;

public slots:
    void updateEventDays( int );
    void updateNWDs( int );

private slots:
    void showOptions();

private:
    void init();

    QHBoxLayout *mainLayout;


    QList<QDate> *nwDays;

    QTabDialog *optionsDialog;
    QWidget *optionsWidget;
    QPushButton *optionsButton;

    // Pages...
    QVBox *generalOptions;
    QVBox *nwdOptions;

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
	 
	 
	 // TBR 12/06/03
	// bool calendario::eventFilter( QObject *, QEvent * );
};



#endif
