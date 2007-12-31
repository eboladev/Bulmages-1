#include "admin.h"
#include <QWidget>

Admin::Admin ( EmpresaTPV *emp, QWidget *parent ) : BLWidget ( emp, parent )
{
    setupUi ( this );
}


Admin::~Admin()
{
}

void Admin::on_mui_z_clicked()
{
    ( ( EmpresaTPV * ) empresaBase() ) ->z();
}

void Admin::on_mui_x_clicked()
{
    ( ( EmpresaTPV * ) empresaBase() ) ->x();
}

void Admin::on_mui_salir_clicked()
{
    if ( QMessageBox::warning ( this,
                                tr ( "Salir" ),
                                tr ( "Realmente desea abandonar el programa ?" ),
                                QMessageBox::Ok,
                                QMessageBox::Cancel ) == QMessageBox::Ok ) {
        g_main->close();
    } // end if
}

