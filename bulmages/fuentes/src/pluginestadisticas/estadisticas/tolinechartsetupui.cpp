/****************************************************************************
** Form implementation generated from reading ui file 'tolinechartsetupui.ui'
**
** Created: sáb abr 16 02:07:21 2005
**      by: The User Interface Compiler ($Id$)
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/

#include "tolinechartsetupui.h"

#include <qvariant.h>
#include <qpushbutton.h>
#include <qgroupbox.h>
#include <qlabel.h>
#include <qlineedit.h>
#include <qcheckbox.h>
#include <qspinbox.h>
#include <qlayout.h>
#include <qtooltip.h>
#include <qwhatsthis.h>

/*
 *  Constructs a toLineChartSetupUI as a child of 'parent', with the
 *  name 'name' and widget flags set to 'f'.
 *
 *  The dialog will by default be modeless, unless you set 'modal' to
 *  TRUE to construct a modal dialog.
 */
toLineChartSetupUI::toLineChartSetupUI( QWidget* parent, const char* name, bool modal, WFlags fl )
    : QDialog( parent, name, modal, fl )
{
    if ( !name )
	setName( "toLineChartSetupUI" );
    toLineChartSetupUILayout = new QGridLayout( this, 1, 1, 11, 6, "toLineChartSetupUILayout"); 

    GroupBox1 = new QGroupBox( this, "GroupBox1" );
    GroupBox1->setColumnLayout(0, Qt::Vertical );
    GroupBox1->layout()->setSpacing( 6 );
    GroupBox1->layout()->setMargin( 11 );
    GroupBox1Layout = new QGridLayout( GroupBox1->layout() );
    GroupBox1Layout->setAlignment( Qt::AlignTop );

    TextLabel1 = new QLabel( GroupBox1, "TextLabel1" );

    GroupBox1Layout->addWidget( TextLabel1, 0, 0 );

    TextLabel4 = new QLabel( GroupBox1, "TextLabel4" );

    GroupBox1Layout->addWidget( TextLabel4, 3, 0 );

    MinValue = new QLineEdit( GroupBox1, "MinValue" );

    GroupBox1Layout->addWidget( MinValue, 5, 0 );

    MaxValue = new QLineEdit( GroupBox1, "MaxValue" );

    GroupBox1Layout->addWidget( MaxValue, 2, 0 );

    AutoMax = new QCheckBox( GroupBox1, "AutoMax" );

    GroupBox1Layout->addWidget( AutoMax, 1, 0 );

    AutoMin = new QCheckBox( GroupBox1, "AutoMin" );

    GroupBox1Layout->addWidget( AutoMin, 4, 0 );

    toLineChartSetupUILayout->addMultiCellWidget( GroupBox1, 0, 0, 0, 2 );
    Spacer1 = new QSpacerItem( 20, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
    toLineChartSetupUILayout->addItem( Spacer1, 6, 0 );

    CancelButton = new QPushButton( this, "CancelButton" );

    toLineChartSetupUILayout->addWidget( CancelButton, 6, 2 );

    OkButton = new QPushButton( this, "OkButton" );
    OkButton->setDefault( TRUE );

    toLineChartSetupUILayout->addWidget( OkButton, 6, 1 );
    Spacer2 = new QSpacerItem( 20, 20, QSizePolicy::Minimum, QSizePolicy::Expanding );
    toLineChartSetupUILayout->addItem( Spacer2, 5, 0 );

    ShowLast = new QCheckBox( this, "ShowLast" );

    toLineChartSetupUILayout->addMultiCellWidget( ShowLast, 1, 1, 0, 2 );

    ShowAxis = new QCheckBox( this, "ShowAxis" );

    toLineChartSetupUILayout->addMultiCellWidget( ShowAxis, 2, 2, 0, 2 );

    ShowLegend = new QCheckBox( this, "ShowLegend" );

    toLineChartSetupUILayout->addMultiCellWidget( ShowLegend, 3, 3, 0, 2 );

    Layout1 = new QHBoxLayout( 0, 0, 6, "Layout1"); 

    TextLabel5 = new QLabel( this, "TextLabel5" );
    TextLabel5->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)7, (QSizePolicy::SizeType)1, 0, 0, TextLabel5->sizePolicy().hasHeightForWidth() ) );
    Layout1->addWidget( TextLabel5 );

    Grids = new QSpinBox( this, "Grids" );
    Layout1->addWidget( Grids );

    toLineChartSetupUILayout->addMultiCellLayout( Layout1, 4, 4, 0, 2 );
    languageChange();
    resize( QSize(246, 370).expandedTo(minimumSizeHint()) );
    clearWState( WState_Polished );

    // signals and slots connections
    connect( AutoMax, SIGNAL( toggled(bool) ), MaxValue, SLOT( setDisabled(bool) ) );
    connect( AutoMin, SIGNAL( toggled(bool) ), MinValue, SLOT( setDisabled(bool) ) );
    connect( OkButton, SIGNAL( clicked() ), this, SLOT( accept() ) );
    connect( CancelButton, SIGNAL( clicked() ), this, SLOT( reject() ) );

    // tab order
    setTabOrder( AutoMax, MaxValue );
    setTabOrder( MaxValue, AutoMin );
    setTabOrder( AutoMin, MinValue );
    setTabOrder( MinValue, ShowLast );
    setTabOrder( ShowLast, ShowAxis );
    setTabOrder( ShowAxis, ShowLegend );
    setTabOrder( ShowLegend, Grids );
    setTabOrder( Grids, OkButton );
    setTabOrder( OkButton, CancelButton );

    // buddies
    TextLabel5->setBuddy( Grids );
}

/*
 *  Destroys the object and frees any allocated resources
 */
toLineChartSetupUI::~toLineChartSetupUI()
{
    // no need to delete child widgets, Qt does it all for us
}

/*
 *  Sets the strings of the subwidgets using the current
 *  language.
 */
void toLineChartSetupUI::languageChange()
{
    setCaption( tr( "Chart setup" ) );
    GroupBox1->setTitle( tr( "Y-axis" ) );
    TextLabel1->setText( tr( "Max value" ) );
    TextLabel4->setText( tr( "Min value" ) );
    AutoMax->setText( tr( "Auto detect" ) );
    AutoMin->setText( tr( "Auto detect" ) );
    CancelButton->setText( tr( "Cancel" ) );
    OkButton->setText( tr( "&Ok" ) );
    ShowLast->setText( tr( "Show last value" ) );
    ShowAxis->setText( tr( "Show axis legend" ) );
    ShowLegend->setText( tr( "Show chart legend" ) );
    TextLabel5->setText( tr( "Divide chart in grids?" ) );
}

