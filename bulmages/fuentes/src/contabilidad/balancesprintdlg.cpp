/****************************************************************************
** Form implementation generated from reading ui file 'balancesprintdlg.ui'
**
** Created: vie ene 2 00:26:15 2004
**      by: The User Interface Compiler ($Id$)
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/

#include "balancesprintdlg.h"

#include <qvariant.h>
#include <qlabel.h>
#include <qlineedit.h>
#include <qgroupbox.h>
#include <qradiobutton.h>
#include <qpushbutton.h>
#include <qprogressbar.h>
#include <qlayout.h>
#include <qtooltip.h>
#include <qwhatsthis.h>

/*
 *  Constructs a balancesprintdlg as a child of 'parent', with the
 *  name 'name' and widget flags set to 'f'.
 *
 *  The dialog will by default be modeless, unless you set 'modal' to
 *  TRUE to construct a modal dialog.
 */
balancesprintdlg::balancesprintdlg( QWidget* parent, const char* name, bool modal, WFlags fl )
    : QDialog( parent, name, modal, fl )
{
    if ( !name )
	setName( "balancesprintdlg" );

    textLabel1_2 = new QLabel( this, "textLabel1_2" );
    textLabel1_2->setGeometry( QRect( 20, 80, 80, 21 ) );

    textLabel1_2_2 = new QLabel( this, "textLabel1_2_2" );
    textLabel1_2_2->setGeometry( QRect( 270, 80, 70, 21 ) );

    textLabel1 = new QLabel( this, "textLabel1" );
    textLabel1->setGeometry( QRect( 20, 30, 60, 21 ) );

    fechafin = new QLineEdit( this, "fechafin" );
    fechafin->setGeometry( QRect( 350, 80, 140, 21 ) );

    nombalance = new QLineEdit( this, "nombalance" );
    nombalance->setGeometry( QRect( 100, 30, 401, 21 ) );

    fechain = new QLineEdit( this, "fechain" );
    fechain->setGeometry( QRect( 120, 80, 131, 21 ) );

    groupBox1 = new QGroupBox( this, "groupBox1" );
    groupBox1->setGeometry( QRect( 10, 170, 200, 50 ) );

    radioButton1_2 = new QRadioButton( groupBox1, "radioButton1_2" );
    radioButton1_2->setGeometry( QRect( 120, 20, 60, 21 ) );

    radioButton1 = new QRadioButton( groupBox1, "radioButton1" );
    radioButton1->setGeometry( QRect( 10, 20, 90, 21 ) );

    pushButton1 = new QPushButton( this, "pushButton1" );
    pushButton1->setGeometry( QRect( 220, 190, 131, 31 ) );

    pushButton1_2 = new QPushButton( this, "pushButton1_2" );
    pushButton1_2->setGeometry( QRect( 360, 190, 131, 31 ) );

    progreso = new QProgressBar( this, "progreso" );
    progreso->setGeometry( QRect( 20, 120, 490, 31 ) );
    languageChange();
    resize( QSize(528, 242).expandedTo(minimumSizeHint()) );
    clearWState( WState_Polished );

    // signals and slots connections
    connect( pushButton1, SIGNAL( clicked() ), this, SLOT( accept() ) );
    connect( pushButton1_2, SIGNAL( clicked() ), this, SLOT( close() ) );
    connect( fechain, SIGNAL( lostFocus() ), this, SLOT( fechaincambiada() ) );
    connect( fechafin, SIGNAL( lostFocus() ), this, SLOT( fechafincambiada() ) );
    connect( fechain, SIGNAL( returnPressed() ), this, SLOT( fechaincambiada() ) );
    connect( fechafin, SIGNAL( returnPressed() ), this, SLOT( fechafincambiada() ) );
}

/*
 *  Destroys the object and frees any allocated resources
 */
balancesprintdlg::~balancesprintdlg()
{
    // no need to delete child widgets, Qt does it all for us
}

/*
 *  Sets the strings of the subwidgets using the current
 *  language.
 */
void balancesprintdlg::languageChange()
{
    setCaption( trUtf8( "\x49\x6d\x70\x72\x65\x73\x69\xc3\xb3\x6e\x20\x64\x65\x20\x42\x61\x6c\x61\x6e\x63\x65\x73" ) );
    textLabel1_2->setText( tr( "Fecha Inicial" ) );
    textLabel1_2_2->setText( tr( "Fecha Final" ) );
    textLabel1->setText( tr( "Balance" ) );
    groupBox1->setTitle( tr( "Formato" ) );
    radioButton1_2->setText( tr( "HTML" ) );
    radioButton1->setText( tr( "texto plano" ) );
    pushButton1->setText( tr( "Aceptar" ) );
    pushButton1_2->setText( tr( "Cancelar" ) );
}

void balancesprintdlg::fechaincambiada()
{
    qWarning( "balancesprintdlg::fechaincambiada(): Not implemented yet" );
}

void balancesprintdlg::fechafincambiada()
{
    qWarning( "balancesprintdlg::fechafincambiada(): Not implemented yet" );
}

