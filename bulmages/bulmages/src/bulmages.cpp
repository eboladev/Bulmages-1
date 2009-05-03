/***************************************************************************
 *                                                                         *
 *   Copyright (C) 2009 Fco. Javier M. C. <fcojavmc@todo-redes.com>        *
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

#include <unistd.h>
#include <stdlib.h>

#include <QAction>
#include <QFile>
#include <QMessageBox>

#include "bulmages.h"

BulmaGes::BulmaGes()
{
    setupUi ( this );
    setWindowTitle ( "Lanzador BulmaGes" );

    createTrayIcon();
    trayIcon->setIcon ( QIcon ( ":/images/iconbulmages.svg" ) );
    trayIcon->show();
}


BulmaGes::~BulmaGes()
{
}


void BulmaGes::on_mui_lanzabulmacont_clicked()
{
    launchBulmaCont();
}


void BulmaGes::on_mui_lanzabulmafact_clicked()
{
    launchBulmaFact();
}


void BulmaGes::on_mui_lanzabulmatpv_clicked()
{
    launchBulmaTPV();
}


void BulmaGes::on_mui_lanzabulmasetup_clicked()
{
    launchBulmaSetup();
}


void BulmaGes::on_mui_cerrar_clicked()
{
    hide();
}


void BulmaGes::activado ( QSystemTrayIcon::ActivationReason reason )
{

    if ( reason == QSystemTrayIcon::Trigger ) {
        // Click boton izquierdo
        show();
    } // end if

}


void BulmaGes::createTrayIcon()
{
    QAction *bulmaContAction;
    QAction *bulmaFactAction;
    QAction *bulmaTPVAction;
    QAction *bulmaSetupAction;
    QAction *salirAction;

    bulmaContAction = new QAction ( tr ( "BulmaCont" ), this );
    bulmaContAction->setIcon ( QIcon ( ":/images/iconbulmacont.svg" ) );
    bulmaFactAction = new QAction ( tr ( "BulmaFact" ), this );
    bulmaFactAction->setIcon ( QIcon ( ":/images/iconbulmafact.svg" ) );
    bulmaTPVAction = new QAction ( tr ( "BulmaTPV" ), this );
    bulmaTPVAction->setIcon ( QIcon ( ":/images/iconbulmatpv.svg" ) );
    bulmaSetupAction = new QAction ( tr ( "BulmaSetup" ), this );
    bulmaSetupAction->setIcon ( QIcon ( ":/images/iconiglues.svg" ) );
    salirAction = new QAction ( tr ( "&Salir" ), this );
    salirAction->setIcon ( QIcon ( ":images/iconexit.png" ) );
    
    connect ( bulmaContAction, SIGNAL ( triggered() ), this , SLOT ( launchBulmaCont() ) );
    connect ( bulmaFactAction, SIGNAL ( triggered() ), this , SLOT ( launchBulmaFact() ) );
    connect ( bulmaTPVAction, SIGNAL ( triggered() ), this , SLOT ( launchBulmaTPV() ) );
    connect ( bulmaSetupAction, SIGNAL ( triggered() ), this , SLOT ( launchBulmaSetup() ) );
    connect ( salirAction, SIGNAL ( triggered() ), qApp , SLOT ( quit() ) );

    trayIconMenu = new QMenu ( this );
    trayIconMenu->addAction ( bulmaContAction );
    trayIconMenu->addAction ( bulmaFactAction );
    trayIconMenu->addAction ( bulmaTPVAction );
    trayIconMenu->addSeparator();
    trayIconMenu->addAction ( bulmaSetupAction );
    trayIconMenu->addSeparator();
    trayIconMenu->addAction ( salirAction );

    trayIcon = new QSystemTrayIcon ( this );
    trayIcon->setContextMenu ( trayIconMenu );

    connect ( trayIcon, SIGNAL ( activated ( QSystemTrayIcon::ActivationReason ) ), this, SLOT ( activado ( QSystemTrayIcon::ActivationReason ) ) );

}


void BulmaGes::launchBulmaCont()
{
    hide();
    runCommand ( "bulmacont" );
}


void BulmaGes::launchBulmaFact()
{
    hide();
    runCommand ( "bulmafact" );
}


void BulmaGes::launchBulmaTPV()
{
    hide();
    runCommand ( "bulmatpv" );
}


void BulmaGes::launchBulmaSetup()
{
    hide();
    /// Entre distintas versiones de KDE el kdesudo cambia de nombre.
    QFile f("/usr/bin/kdesu");
    QFile j("/usr/bin/kdesudo");
    if (f.exists()) {
      runCommand ( "kdesu -c bulmasetup --" );
    } else if (j.exists()) {
      runCommand ( "kdesudo -c bulmasetup" );
    } else {
      QMessageBox msgBox;
      msgBox.setText("No se encuentra kdesudo");
      msgBox.exec();    
    } // end if
}


void BulmaGes::runCommand ( QString command )
{
    command = command + QString ( " &" );
    system ( command.toAscii().constData() );
}

