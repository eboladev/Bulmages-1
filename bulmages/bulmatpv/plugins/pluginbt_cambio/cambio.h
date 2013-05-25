#ifndef COBRAR_H
#define COBRAR_H

#include <QtWidgets/QTableWidget>

#include "blwidget.h"
#include "ui_cambiobase.h"
#include "btcompany.h"


class Cambio : public BlWidget, public Ui_CambioBase
{
    Q_OBJECT

public:
    int m_value;

public:
    Cambio ( BtCompany *emp, QWidget *parent );
    virtual ~Cambio();

public slots:
    virtual void on_mui_pago_textChanged();
    virtual void on_mui_cancelar_clicked();
    virtual void on_mui_cobrar_clicked();
    virtual void on_mui_visa_clicked();

/// La parte de las teclas
public slots:
    virtual void on_mui_00_clicked() {
        mui_pago->setText ( mui_pago->text() + "00" );
    };
    virtual void on_mui_0_clicked() {
        mui_pago->setText ( mui_pago->text() + "0" );
    };
    virtual void on_mui_1_clicked() {
        mui_pago->setText ( mui_pago->text() + "1" );
    };
    virtual void on_mui_2_clicked() {
        mui_pago->setText ( mui_pago->text() + "2" );
    };
    virtual void on_mui_3_clicked() {
        mui_pago->setText ( mui_pago->text() + "3" );
    };
    virtual void on_mui_4_clicked() {
        mui_pago->setText ( mui_pago->text() + "4" );
    };
    virtual void on_mui_5_clicked() {
        mui_pago->setText ( mui_pago->text() + "5" );
    };
    virtual void on_mui_6_clicked() {
        mui_pago->setText ( mui_pago->text() + "6" );
    };
    virtual void on_mui_7_clicked() {
        mui_pago->setText ( mui_pago->text() + "7" );
    };
    virtual void on_mui_8_clicked() {
        mui_pago->setText ( mui_pago->text() + "8" );
    };
    virtual void on_mui_9_clicked() {
        mui_pago->setText ( mui_pago->text() + "9" );
    };
    virtual void on_mui_punto_clicked() {
        mui_pago->setText ( mui_pago->text() + "." );
    };
    virtual void on_mui_borrar_clicked() {
        mui_pago->setText ( "" );
    };
    virtual void on_mui_c_clicked() {
        mui_pago->setText ( "" );
    };
};

#endif

