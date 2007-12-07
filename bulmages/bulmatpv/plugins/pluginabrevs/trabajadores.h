#ifndef TRABAJADORESS_H
#define TRABAJADORESS_H

#include <QDialog>
#include <QProcess>
#include <QDockWidget>
#include "blwidget.h"
#include "ui_trabajadoresbase.h"

class EmpresaTPV;

class Trabajadores : public QDialog, PEmpresaBase, public Ui_TrabajadoresBase {
    Q_OBJECT

public:
    Trabajadores(EmpresaBase *emp, QWidget *parent);
    virtual ~Trabajadores();
public slots:
    virtual void trabajadorClicked();
};

#endif

