#ifndef TRABAJADORESS_H
#define TRABAJADORESS_H

#include <QDialog>
#include <QProcess>
#include <QDockWidget>
#include "blwidget.h"
#include "ui_trabajadoresbase.h"

class BtCompany;

class Trabajadores : public QDialog, BlMainCompanyPointer, public Ui_TrabajadoresBase
{
    Q_OBJECT

public:
    Trabajadores ( BlMainCompany *emp, QWidget *parent );
    virtual ~Trabajadores();
public slots:
    virtual void trabajadorClicked();
};

#endif

