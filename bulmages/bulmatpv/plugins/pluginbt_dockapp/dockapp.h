#ifndef DOCKAPP_H
#define DOCKAPP_H

#include <QtWidgets/QTableWidget>
#include "btcompany.h"
#include "blwidget.h"



#include "ui_dockappbase.h"



class DockApp : public BlWidget, public Ui_DockAppBase
{
    Q_OBJECT

public:
    DockApp ( BtCompany *emp, QWidget *parent );
    virtual ~DockApp();

public slots:
    virtual void on_mui_dockapp_clicked();
};





#endif



