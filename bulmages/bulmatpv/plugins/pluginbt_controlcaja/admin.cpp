#include "admin.h"
#include <QWidget>

#include "blmainwindow.h"


Admin::Admin ( BtCompany *emp, QWidget *parent ) : BlWidget ( emp, parent )
{
    setupUi ( this );
    m_btCompany = emp;
}


Admin::~Admin()
{}

void Admin::on_mui_z_clicked()
{
    if ( QMessageBox::warning ( this,
                                _ ( "Realizar Z" ),
                                _ ( "Realmente desea Realizar la Z?" ),
                                QMessageBox::Ok,
                                QMessageBox::Cancel ) == QMessageBox::Ok ) {
        m_btCompany->z();
    } // end if

}

void Admin::on_mui_x_clicked()
{
    m_btCompany->x();
}

void Admin::on_mui_salir_clicked()
{
//     if ( QMessageBox::warning ( this,
//                                 _ ( "Salir" ),
//                                 _ ( "Realmente desea abandonar el programa?" ),
//                                 QMessageBox::Ok,
//                                 QMessageBox::Cancel ) == QMessageBox::Ok ) {
//         g_main->close();
//     } // end if

    g_main->close();
}

