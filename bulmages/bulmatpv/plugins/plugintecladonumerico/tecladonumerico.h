#ifndef TECLADONUMERICO_H
#define TECLADONUMERICO_H

#include <QWidget>
#include <QTableWidget>

#include "blfunctions.h"
#include "ui_tecladonumericobase.h"
#include "blwidget.h"
#include "empresatpv.h"


class TecladoNumerico : public BlWidget, public Ui_TecladoNumericoBase
{
    Q_OBJECT

public:
    TecladoNumerico ( BlMainCompany *emp, QWidget *parent );
    virtual ~TecladoNumerico();

public slots:
    virtual void on_mui_00_clicked() {
        ( ( EmpresaTPV * ) empresaBase() ) ->pulsaTecla ( Qt::Key_0, "0" );
        ( ( EmpresaTPV * ) empresaBase() ) ->pulsaTecla ( Qt::Key_0, "0" );
    };
    virtual void on_mui_0_clicked() {
        ( ( EmpresaTPV * ) empresaBase() ) ->pulsaTecla ( Qt::Key_0, "0" );
    };
    virtual void on_mui_1_clicked() {
        ( ( EmpresaTPV * ) empresaBase() ) ->pulsaTecla ( Qt::Key_1, "1" );
    };
    virtual void on_mui_2_clicked() {
        ( ( EmpresaTPV * ) empresaBase() ) ->pulsaTecla ( Qt::Key_2, "2" );
    };
    virtual void on_mui_3_clicked() {
        ( ( EmpresaTPV * ) empresaBase() ) ->pulsaTecla ( Qt::Key_3, "3" );
    };
    virtual void on_mui_4_clicked() {
        ( ( EmpresaTPV * ) empresaBase() ) ->pulsaTecla ( Qt::Key_4, "4" );
    };
    virtual void on_mui_5_clicked() {
        ( ( EmpresaTPV * ) empresaBase() ) ->pulsaTecla ( Qt::Key_5, "5" );
    };
    virtual void on_mui_6_clicked() {
        ( ( EmpresaTPV * ) empresaBase() ) ->pulsaTecla ( Qt::Key_6, "6" );
    };
    virtual void on_mui_7_clicked() {
        ( ( EmpresaTPV * ) empresaBase() ) ->pulsaTecla ( Qt::Key_7, "7" );
    };
    virtual void on_mui_8_clicked() {
        ( ( EmpresaTPV * ) empresaBase() ) ->pulsaTecla ( Qt::Key_8, "8" );
    };
    virtual void on_mui_9_clicked() {
        ( ( EmpresaTPV * ) empresaBase() ) ->pulsaTecla ( Qt::Key_9, "9" );
    };
    virtual void on_mui_punto_clicked() {
        ( ( EmpresaTPV * ) empresaBase() ) ->pulsaTecla ( Qt::Key_Colon, "." );
    };
    virtual void on_mui_borrar_clicked() {
        ( ( EmpresaTPV * ) empresaBase() ) ->pulsaTecla ( Qt::Key_Backspace );
    };
    virtual void on_mui_articulo_clicked() {
        ( ( EmpresaTPV * ) empresaBase() ) ->pulsaTecla ( Qt::Key_Return );
    };
    virtual void on_mui_cantidad_clicked() {
        ( ( EmpresaTPV * ) empresaBase() ) ->pulsaTecla ( Qt::Key_Asterisk, "*" );
    };
    virtual void on_mui_precio_clicked() {
        ( ( EmpresaTPV * ) empresaBase() ) ->pulsaTecla ( Qt::Key_Slash, "/" );
    };
    virtual void on_mui_c_clicked() {
        ( ( EmpresaTPV * ) empresaBase() ) ->pulsaTecla ( Qt::Key_C, "C" );
    };
    virtual void on_mui_plus_clicked() {
        ( ( EmpresaTPV * ) empresaBase() ) ->pulsaTecla ( Qt::Key_Plus, "+" );
    };
    virtual void on_mui_minus_clicked() {
        ( ( EmpresaTPV * ) empresaBase() ) ->pulsaTecla ( Qt::Key_Minus, "-" );
    };
    virtual void on_mui_mesa_clicked() {
        ( ( EmpresaTPV * ) empresaBase() ) ->pulsaTecla ( Qt::Key_M, "M" );
    };
};

#endif

