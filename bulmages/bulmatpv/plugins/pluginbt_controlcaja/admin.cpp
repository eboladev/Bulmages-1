#include "admin.h"
#include <QWidget>

#include "blmainwindow.h"


Admin::Admin ( BtCompany *emp, QWidget *parent ) : BlWidget ( emp, parent )
{
    setupUi ( this );
}


Admin::~Admin()
{}

void Admin::on_mui_z_released()
{
    if ( QMessageBox::warning ( this,
                                _ ( "Salir" ),
                                _ ( "Realmente desea Realizar la Z?" ),
                                QMessageBox::Ok,
                                QMessageBox::Cancel ) == QMessageBox::Ok ) {
        ( ( BtCompany * ) mainCompany() ) ->z();
    } // end if

}

void Admin::on_mui_x_released()
{
    ( ( BtCompany * ) mainCompany() ) ->x();
}

void Admin::on_mui_salir_released()
{
    if ( QMessageBox::warning ( this,
                                _ ( "Salir" ),
                                _ ( "Realmente desea abandonar el programa?" ),
                                QMessageBox::Ok,
                                QMessageBox::Cancel ) == QMessageBox::Ok ) {
        g_main->close();
    } // end if
}

