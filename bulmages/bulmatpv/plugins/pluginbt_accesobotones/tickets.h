#ifndef TICKETS_H
#define TICKETS_H

#include <QtWidgets/QDialog>
#include <QtCore/QProcess>
#include <QtWidgets/QDockWidget>
#include "blwidget.h"
#include "ui_ticketsbase.h"

class BtCompany;

class Tickets : public QDialog, BlMainCompanyPointer, public Ui_TicketsBase
{
    Q_OBJECT

public:
    Tickets ( BlMainCompany *emp, QWidget *parent );
    virtual ~Tickets();
public slots:
    virtual void ticketClicked();
};

#endif
