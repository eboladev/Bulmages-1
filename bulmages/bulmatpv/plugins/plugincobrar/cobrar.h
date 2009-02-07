#ifndef COBRAR_H
#define COBRAR_H

#include <QTableWidget>

#include "blwidget.h"
#include "ui_cobrarbase.h"
#include "empresatpv.h"


class Cobrar : public BlWidget, public Ui_CobrarBase
{
    Q_OBJECT

public:
    Cobrar ( EmpresaTPV *emp, QWidget *parent );
    virtual ~Cobrar();

public slots:
    virtual void on_mui_cobrar_clicked();
    virtual void on_mui_abrircajon_clicked();
};

#endif

