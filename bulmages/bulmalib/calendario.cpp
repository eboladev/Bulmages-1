/***************************************************************************
                          calendario.cpp  -  description
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
#include <qlayout.h>
#include <qtabdialog.h>
#include <qpushbutton.h>
#include <qvbuttongroup.h>
#include <qcheckbox.h>
#include <qvbox.h>


#include "calendario.h"

calendario::calendario( class QDialog * parent, const char * name )
        : QDialog( parent, name ) {
    init();
}

calendario::~calendario() {
    dn->removeNonWorkingDayList(); // remember to do this _before_ deleting the list!
    nwDays->clear(); // autoDelete is set to true -> see ::init()
    delete nwDays; // Finally, it's safe to delete.
    dn->removeEventDayList();
    eDays->clear();
    delete eDays;
    // You shouldn't have to explicitely delete the QmcDateNav widget, as Qt should take care of it.
}


void calendario::init() {
    for( int t = 1; t < 6; t++ )
        nonWorkDays[t] = false;
    nonWorkDays[6] = true;
    nonWorkDays[7] = true;

    dn = new QmcDateNav( this, "dn" );

//     optionsDialog = new QTabDialog( this, "optionsDialog", true );
//     optionsDialog->setCaption( "QmcDateNav Options" );
// 
//     generalOptions = new QVBox( this, "generalOptions" );
//     generalOptions->setMargin( 6 );
// 
//     optionsDialog->addTab( generalOptions, tr( "General" ) );
// 
//     qcbOutlook = new QCheckBox( "Outlook Mode", generalOptions, "qcbOutlook" );
//     qcbOutlook->setChecked( dn->outlook() );
// 
//     qcbFrame = new QCheckBox( "Draw Frame", generalOptions, "qcbFrame" );
//     qcbFrame->setChecked( dn->frame() );
// 
//     nwdOptions = new QVBox( this, "nwdOptions" );
//     nwdOptions->setMargin( 6 );
// 
//     optionsDialog->addTab( nwdOptions, tr( "Non-working Days" ) );
// 
//     qcbWorkMon = new QCheckBox( tr( "Monday" ), nwdOptions, "qcbWorkMon" );
//     qcbWorkMon->setChecked( nonWorkDays[1] );
//     qcbWorkTue = new QCheckBox( tr( "Tuesday" ), nwdOptions, "qcbWorkTue" );
//     qcbWorkTue->setChecked( nonWorkDays[2] );
//     qcbWorkWed = new QCheckBox( tr( "Wednesday" ), nwdOptions, "qcbWorkWed" );
//     qcbWorkWed->setChecked( nonWorkDays[3] );
//     qcbWorkThu = new QCheckBox( tr( "Thursday" ), nwdOptions, "qcbWorkThu" );
//     qcbWorkThu->setChecked( nonWorkDays[4] );
//     qcbWorkFri = new QCheckBox( tr( "Friday" ), nwdOptions, "qcbWorkFri" );
//     qcbWorkFri->setChecked( nonWorkDays[5] );
//     qcbWorkSat = new QCheckBox( tr( "Saturday" ), nwdOptions, "qcbWorkSat" );
//     qcbWorkSat->setChecked( nonWorkDays[6] );
//     qcbWorkSun = new QCheckBox( tr( "Sunday" ), nwdOptions, "qcbWorkSun" );
//     qcbWorkSun->setChecked( nonWorkDays[7] );

    eDays = new QList<QDate>;
    nwDays = new QList<QDate>;
    eDays->setAutoDelete( true );
    nwDays->setAutoDelete( true );

    dn->installNonWorkingDayList( nwDays );
    dn->installEventDayList( eDays );

    updateNWDs( QDate::currentDate().year() );
    updateEventDays( QDate::currentDate().year() );

//      optionsWidget = new QWidget( this, "optionsWidget" );

    mainLayout = new QHBoxLayout( this, 4, 4, "mainLayout" );
    mainLayout->addWidget( dn );
    //    mainLayout->addWidget( optionsWidget );

//      optionsButton = new QPushButton( "&Options...", optionsWidget, "optionsButton" );

    connect( dn, SIGNAL( yearChanged( int ) ), this, SLOT( updateNWDs( int ) ) );
    connect( dn, SIGNAL( yearChanged( int ) ), this, SLOT( updateEventDays( int ) ) );
    connect (dn, SIGNAL( close()), this, SLOT(close()));
    //   connect( optionsButton, SIGNAL( clicked() ), this, SLOT( showOptions() ) );
}

/** \todo Hacer que el programa importe de un fichero los dias no laborables oficiales
*/
void calendario::updateNWDs( int yr ) {
    // Note that there are a maximum of 10 days from the previous and next years that you need to cater for,
    // I don't worry about those here, but you should.

    nwDays->clear();

    for( int t = 1; t < 8; t++ ) {
        if( nonWorkDays[t] == false )
            continue;

        QDate d( yr, 1, 1 );
        while( d.dayOfWeek() != t ) {
            d = d.addDays( 1 );
        }

        int cd = d.dayOfYear();
        int maxDays = d.daysInYear();

        while( cd <= maxDays ) {
            nwDays->append( new QDate( d ) );
            d = d.addDays( 7 );
            cd += 7;
        }
    }

    // Add stuff from the diary here.

    dn->forceUpdate();
}

/** \todo Hacer que el programa importe de un fichero los eventos (como plazos de entrega de declaraciones, etc.)
*/
void calendario::updateEventDays( int yr ) {
    eDays->clear();
    eDays->append( new QDate( yr, 1, 1 ) );
    eDays->append( new QDate( yr, 1, 6 ) ); // My birthday ;-)
    eDays->append( new QDate( yr, 12, 25 ) );
    eDays->append( new QDate( 1999, 8, 24 ) ); // Release of QmcDateNav 0.0.2
    dn->forceUpdate();
}


void calendario::showOptions() {
    if( optionsDialog )
        optionsDialog->show();
    else
        return;

    if( qcbOutlook->isChecked() != dn->outlook() )
        dn->setOutlook( ! dn->outlook() );

    if( qcbFrame->isChecked() != dn->frame() )
        dn->setFrame( ! dn->frame() );

    if( qcbWorkMon->isChecked() != nonWorkDays[1] )
        nonWorkDays[1] = qcbWorkMon->isChecked();

    if( qcbWorkTue->isChecked() != nonWorkDays[2] )
        nonWorkDays[2] = qcbWorkTue->isChecked();

    if( qcbWorkWed->isChecked() != nonWorkDays[3] )
        nonWorkDays[3] = qcbWorkWed->isChecked();

    if( qcbWorkThu->isChecked() != nonWorkDays[4] )
        nonWorkDays[4] = qcbWorkThu->isChecked();

    if( qcbWorkFri->isChecked() != nonWorkDays[5] )
        nonWorkDays[5] = qcbWorkFri->isChecked();

    if( qcbWorkSat->isChecked() != nonWorkDays[6] )
        nonWorkDays[6] = qcbWorkSat->isChecked();

    if( qcbWorkSun->isChecked() != nonWorkDays[7] )
        nonWorkDays[7] = qcbWorkSun->isChecked();

    updateNWDs( dn->selectedDates().at( 0 )->year() );

    dn->forceUpdate();
}



/*
bool calendario::eventFilter( QObject *obj, QEvent *event ) {
   static bool ctrlpulsado= FALSE;   
   if ( event->type() == QEvent::KeyPress ) {
       QKeyEvent *keyEvent = (QKeyEvent *) event;
       int key = keyEvent->key();
       fprintf(stderr,"Key event %d %c\n", key, key);
       if ( key == '+') {
          return TRUE;
       }// end if
       if (key == '*') {
          return TRUE;
       }// end if
       if (key == 4100) { // El enter
          return TRUE;
       }// end if
       if (key == 4115) {// La tecla hacia arriba
         // if (ctrlpulsado)   // Solo en combinacion con el ctrl
       }// end if
       if (key == 4117) {// La tecla hacia arriba
         // if (ctrlpulsado)   // Solo en combinacion con el ctrl
       }// end if
       if (key == 4129) { // el Control
         ctrlpulsado = TRUE;
       }// end if
   }// end if
   if (event->type() == QEvent::KeyRelease) {
      QKeyEvent *keyEvent = (QKeyEvent *) event;
      int key = keyEvent->key();
         if (key == 4129) {
               ctrlpulsado = FALSE;
         }// end if
   }// end if
   return QDialog::eventFilter(obj, event);
}// end eventFilter
 
*/

