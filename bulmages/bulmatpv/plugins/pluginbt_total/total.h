#ifndef TOTAL_H
#define TOTAL_H

#include <QtWidgets/QTableWidget>

#include "blwidget.h"
#include "ui_totalbase.h"
#include "btcompany.h"

class Total : public BlWidget, public Ui_TotalBase
{
    Q_OBJECT

public:
    Total ( BtCompany *emp, QWidget *parent );
    virtual ~Total();

public slots:
    virtual void on_mui_cobrar_clicked();
    virtual void on_mui_abrircajon_clicked();
    virtual void on_mui_imprimir_clicked();
};

#endif

