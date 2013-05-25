/****************************************************************************
**
** Copyright (C) 2009 Fco. Javier M. C. <fcojavmc@todo-redes.com>
**
** Copyright (C) 2008 Nokia Corporation and/or its subsidiary(-ies).
** Contact: Qt Software Information (qt-info@nokia.com)
**
** This file is part of the example classes of the Qt Toolkit.
**
** No Commercial Usage
** This file contains pre-release code and may only be used for
** evaluation and testing purposes.  It may not be used for commercial
** development.  You may use this file in accordance with the terms and
** conditions contained in the either Technology Preview License
** Agreement or the Beta Release License Agreement.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License versions 2.0 or 3.0 as published by the Free
** Software Foundation and appearing in the file LICENSE.GPL included in
** the packaging of this file.  Please review the following information
** to ensure GNU General Public Licensing requirements will be met:
** http://www.fsf.org/licensing/licenses/info/GPLv2.html and
** http://www.gnu.org/copyleft/gpl.html.  In addition, as a special
** exception, Nokia gives you certain additional rights. These rights
** are described in the Nokia Qt GPL Exception version 1.3, included in
** the file GPL_EXCEPTION.txt in this package.
**
** Qt for Windows(R) Licensees
** As a special exception, Nokia, as the sole copyright holder for Qt
** Designer, grants users of the Qt/Eclipse Integration plug-in the
** right for the Qt/Eclipse Integration to link to functionality
** provided by Qt Designer and its related libraries.
**
** If you are unsure which license is appropriate for your use, please
** contact the sales department at qt-sales@nokia.com.
**
****************************************************************************/

#ifndef BGBULMAGES_H
#define BGBULMAGES_H

#include <QtCore/QObject>
#include <QtWidgets/QSystemTrayIcon>
#include <QtWidgets/QDialog>
#include <QtWidgets/QMenu>
#include <QtCore/QString>

#include "blfunctions.h"
#include "ui_bgbulmagesbase.h"


class BgBulmaGes : public QDialog, private Ui::BgBulmaGesBase
{
    Q_OBJECT

public:
    BgBulmaGes();
    ~BgBulmaGes();

private:
    void createTrayIcon();
    QSystemTrayIcon *trayIcon;
    QMenu *trayIconMenu;
    void runCommand(QString command);

private slots:
    void activado(QSystemTrayIcon::ActivationReason reason);
    void launchBulmaFact();
    void launchBulmaCont();
    void launchBulmaTPV();
    void launchBulmaSetup();
    void on_mui_lanzabulmacont_clicked();
    void on_mui_lanzabulmafact_clicked();
    void on_mui_lanzabulmatpv_clicked();
    void on_mui_lanzabulmasetup_clicked();
    void on_mui_cerrar_clicked();
};

#endif

