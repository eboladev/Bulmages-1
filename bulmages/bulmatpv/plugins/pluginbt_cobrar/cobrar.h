#ifndef COBRAR_H
#define COBRAR_H

#include <QTableWidget>

#include "blwidget.h"
#include "ui_cobrarbase.h"
#include "btcompany.h"


class Cobrar : public BlWidget, public Ui_CobrarBase
{
    Q_OBJECT

public:
    Cobrar ( BtCompany *emp, QWidget *parent );
    virtual ~Cobrar();

public slots:
    virtual void on_mui_cobrar_released();
    virtual void on_mui_abrircajon_released();
};

#endif

