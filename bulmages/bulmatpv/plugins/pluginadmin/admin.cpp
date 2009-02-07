#include "admin.h"
#include <QWidget>

Admin::Admin ( EmpresaTPV *emp, QWidget *parent ) : BlWidget ( emp, parent )
{
    setupUi ( this );
}


Admin::~Admin()
{}

void Admin::on_mui_z_clicked()
{
    if ( QMessageBox::warning ( this,
                                _( "Salir" ),
                                _( "Realmente desea Realizar la Z?" ),
                                QMessageBox::Ok,
                                QMessageBox::Cancel ) == QMessageBox::Ok ) {
  	  ( ( EmpresaTPV * ) empresaBase() ) ->z();
    } // end if

}

void Admin::on_mui_x_clicked()
{
    ( ( EmpresaTPV * ) empresaBase() ) ->x();
}

void Admin::on_mui_salir_clicked()
{
    if ( QMessageBox::warning ( this,
                                _( "Salir" ),
                                _( "Realmente desea abandonar el programa?" ),
                                QMessageBox::Ok,
                                QMessageBox::Cancel ) == QMessageBox::Ok ) {
        g_main->close();
    } // end if
}

