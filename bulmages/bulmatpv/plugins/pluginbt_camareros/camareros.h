#ifndef CAMAREROS_H
#define CAMAREROS_H

#include <QDialog>
#include <QProcess>
#include <QDockWidget>
#include "blwidget.h"
#include "ui_camarerosbase.h"

class BtCompany;


class Camareros : public BlWidget, public Ui_CamarerosBase
{
    Q_OBJECT
public:
    QWidget *m_centralWidget;
//    DistroCamareros *m_distro;
public:
    Camareros ( BtCompany *emp, QWidget *parent );
    virtual ~Camareros();

public slots:
    virtual void on_mui_camareros_clicked();
};

#endif

