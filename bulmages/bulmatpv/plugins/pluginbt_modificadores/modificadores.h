#ifndef MODIFICADORES_H
#define MODIFICADORES_H

#include <QDialog>
#include <QProcess>
#include <QDockWidget>
#include "blwidget.h"
#include "ui_modificadoresbase.h"

class BtCompany;

class Modificadores : public QDialog, BlMainCompanyPointer, public Ui_ModificadoresBase
{
    Q_OBJECT

public:
    Modificadores ( BlMainCompany *emp, QWidget *parent );
    virtual ~Modificadores();
public slots:
    virtual void trabajadorClicked();
};

#endif

