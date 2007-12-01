#ifndef TECLADONUMERICO_H
#define TECLADONUMERICO_H

#include <QWidget>
#include <QTableWidget>
#include "ui_tecladonumericobase.h"
#include "blwidget.h"
#include "empresatpv.h"

class TecladoNumerico : public BLWidget, public Ui_TecladoNumericoBase {
    Q_OBJECT

public:
    TecladoNumerico(EmpresaBase *emp, QWidget *parent);
    virtual ~TecladoNumerico();
public slots:
    virtual void on_mui_00_clicked() {
	((EmpresaTPV *)empresaBase())->pulsaTecla(Qt::Key_0);
	((EmpresaTPV *)empresaBase())->pulsaTecla(Qt::Key_0);
    };
    virtual void on_mui_0_clicked() {
	((EmpresaTPV *)empresaBase())->pulsaTecla(Qt::Key_0);
    };
    virtual void on_mui_1_clicked() {
	((EmpresaTPV *)empresaBase())->pulsaTecla(Qt::Key_1);
    };
    virtual void on_mui_2_clicked() {
	((EmpresaTPV *)empresaBase())->pulsaTecla(Qt::Key_2);
    };
    virtual void on_mui_3_clicked() {
	((EmpresaTPV *)empresaBase())->pulsaTecla(Qt::Key_3);
    };
    virtual void on_mui_4_clicked() {
	((EmpresaTPV *)empresaBase())->pulsaTecla(Qt::Key_4);
    };
    virtual void on_mui_5_clicked() {
	((EmpresaTPV *)empresaBase())->pulsaTecla(Qt::Key_5);
    };
    virtual void on_mui_6_clicked() {
	((EmpresaTPV *)empresaBase())->pulsaTecla(Qt::Key_6);
    };
    virtual void on_mui_7_clicked() {
	((EmpresaTPV *)empresaBase())->pulsaTecla(Qt::Key_7);
    };
    virtual void on_mui_8_clicked() {
	((EmpresaTPV *)empresaBase())->pulsaTecla(Qt::Key_8);
    };
    virtual void on_mui_9_clicked() {
	((EmpresaTPV *)empresaBase())->pulsaTecla(Qt::Key_9);
    };
    virtual void on_mui_cant_clicked() {
	((EmpresaTPV *)empresaBase())->pulsaTecla(Qt::Key_Asterisk);
    };
    virtual void on_mui_price_clicked() {
	((EmpresaTPV *)empresaBase())->pulsaTecla(Qt::Key_Slash);
    };
    virtual void on_mui_plus_clicked() {
	((EmpresaTPV *)empresaBase())->pulsaTecla(Qt::Key_Plus);
    };
    virtual void on_mui_minus_clicked() {
	((EmpresaTPV *)empresaBase())->pulsaTecla(Qt::Key_Minus);
    };
};

#endif