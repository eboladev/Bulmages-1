#ifndef ABREVS_H
#define ABREVS_H

#include "blwidget.h"
#include "ui_abrevsbase.h"


class BtCompany;


class Abrevs : public BlWidget, public Ui_AbrevsBase
{
    Q_OBJECT

public:
    Abrevs ( BlMainCompany *emp, QWidget *parent );
    virtual ~Abrevs();

public slots:
    virtual void on_mui_aparcar_clicked();
    virtual void on_mui_recuperar_clicked();
    virtual void on_mui_cliente_clicked();

};

#endif



