#ifndef ADMIN_H
#define ADMIN_H

#include "blwidget.h"
#include <QTableWidget>
#include "ui_adminbase.h"
#include "empresatpv.h"

class Admin : public BLWidget, public Ui_AdminBase {
    Q_OBJECT

public:
    Admin(EmpresaTPV *emp, QWidget *parent);
    virtual ~Admin();
public slots:
    virtual void on_mui_z_clicked();
    virtual void on_mui_x_clicked();
};

#endif

