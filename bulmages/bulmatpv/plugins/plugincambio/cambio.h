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
    int m_value;

public:
    Cambio ( EmpresaTPV *emp, QWidget *parent );
    virtual ~Cambio();

public slots:
    virtual void on_mui_pago_textChanged();
    virtual void on_mui_cancelar_clicked();
    virtual void on_mui_cobrar_clicked();
    virtual void on_mui_visa_clicked();
};

#endif

