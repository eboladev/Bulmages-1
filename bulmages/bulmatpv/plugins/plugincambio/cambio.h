#ifndef COBRAR_H
#define COBRAR_H

#include "blwidget.h"
#include <QTableWidget>
#include "ui_cambiobase.h"
#include "empresatpv.h"

class Cambio : public BLWidget, public Ui_CambioBase
{
    Q_OBJECT

public:
    Cambio ( EmpresaTPV *emp, QWidget *parent );
    virtual ~Cambio();

public slots:
    virtual void on_mui_cobrar_clicked();
    virtual void on_mui_abrircajon_clicked();
    virtual void on_mui_pago_textChanged();
};

#endif

