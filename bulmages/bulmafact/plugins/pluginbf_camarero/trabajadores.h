#ifndef TRABAJADORESS_H
#define TRABAJADORESS_H

#include <QtWidgets/QDialog>
#include <QtCore/QProcess>
#include <QtWidgets/QDockWidget>
#include "blwidget.h"
#include "ui_trabajadoresbase.h"

class BtCompany;

class Trabajadores : public QDialog, BlMainCompanyPointer, public Ui_TrabajadoresBase
{
    Q_OBJECT

protected:
    void closeEvent(QCloseEvent *event);
    bool m_validUser;
    bool m_deleteOnClose;
     
public:
    Trabajadores ( BlMainCompany *emp, QWidget *parent, bool deleteOnClose = false );
    virtual ~Trabajadores();
    
public slots:
    void closeDialog();
    virtual void trabajadorClicked();
    void on_mui_cerrar_clicked();

    /// La parte de las teclas
    virtual void on_mui_00_clicked() {
        mui_password->setText ( mui_password->text() + "00" );
    };
    virtual void on_mui_0_clicked() {
        mui_password->setText ( mui_password->text() + "0" );
    };
    virtual void on_mui_1_clicked() {
        mui_password->setText ( mui_password->text() + "1" );
    };
    virtual void on_mui_2_clicked() {
        mui_password->setText ( mui_password->text() + "2" );
    };
    virtual void on_mui_3_clicked() {
        mui_password->setText ( mui_password->text() + "3" );
    };
    virtual void on_mui_4_clicked() {
        mui_password->setText ( mui_password->text() + "4" );
    };
    virtual void on_mui_5_clicked() {
        mui_password->setText ( mui_password->text() + "5" );
    };
    virtual void on_mui_6_clicked() {
        mui_password->setText ( mui_password->text() + "6" );
    };
    virtual void on_mui_7_clicked() {
        mui_password->setText ( mui_password->text() + "7" );
    };
    virtual void on_mui_8_clicked() {
        mui_password->setText ( mui_password->text() + "8" );
    };
    virtual void on_mui_9_clicked() {
        mui_password->setText ( mui_password->text() + "9" );
    };
    virtual void on_mui_punto_clicked() {
        mui_password->setText ( mui_password->text() + "." );
    };
    virtual void on_mui_borrar_clicked() {
        mui_password->setText ( "" );
    };
    virtual void on_mui_c_clicked() {
        mui_password->setText ( "" );
    };

};

#endif

