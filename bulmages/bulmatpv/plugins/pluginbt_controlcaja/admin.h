#ifndef ADMIN_H
#define ADMIN_H

#include <QtWidgets/QTableWidget>

#include "blwidget.h"
#include "ui_adminbase.h"
#include "btcompany.h"


class Admin : public BlWidget, public Ui_AdminBase
{
    Q_OBJECT

    BtCompany *m_btCompany;

public:
    Admin ( BtCompany *emp, QWidget *parent );
    virtual ~Admin();

public slots:
    virtual void on_mui_z_clicked();
    virtual void on_mui_x_clicked();
    virtual void on_mui_salir_clicked();
};

#endif

