#ifndef TOTAL_H
#define TOTAL_H

#include "blwidget.h"
#include <QTableWidget>
#include "ui_totalbase.h"
#include "empresatpv.h"

class Total : public BLWidget, public Ui_TotalBase
{
    Q_OBJECT

public:
    Total ( EmpresaTPV *emp, QWidget *parent );
    virtual ~Total();
public slots:
    virtual void on_mui_cobrar_clicked();
    virtual void on_mui_abrircajon_clicked();
};

#endif

