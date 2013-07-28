#ifndef TECLADONUMERICO_H
#define TECLADONUMERICO_H

#include <QtWidgets/QWidget>
#include <QtWidgets/QTableWidget>
#include <QtCore/QFile>

#include "blfunctions.h"
#include "ui_tecladonumericobase.h"
#include "blwidget.h"
#include "btcompany.h"


class TecladoNumerico : public BlWidget, public Ui_TecladoNumericoBase
{
    Q_OBJECT

public:
    TecladoNumerico ( BlMainCompany *emp, QWidget *parent );
    virtual ~TecladoNumerico();

public slots:
    virtual void playSound();
    virtual void on_mui_00_clicked();
    virtual void on_mui_0_clicked();
    virtual void on_mui_1_clicked();
    virtual void on_mui_2_clicked();
    virtual void on_mui_3_clicked();
    virtual void on_mui_4_clicked();
    virtual void on_mui_5_clicked();
    virtual void on_mui_6_clicked();
    virtual void on_mui_7_clicked();
    virtual void on_mui_8_clicked();
    virtual void on_mui_9_clicked();
    virtual void on_mui_punto_clicked();
    virtual void on_mui_borrar_clicked();
    virtual void on_mui_articulo_clicked();
    virtual void on_mui_cantidad_clicked();
    virtual void on_mui_precio_clicked();
    virtual void on_mui_c_clicked();
    virtual void on_mui_plus_clicked();
    virtual void on_mui_minus_clicked();
    virtual void on_mui_mesa_clicked();
};

#endif

