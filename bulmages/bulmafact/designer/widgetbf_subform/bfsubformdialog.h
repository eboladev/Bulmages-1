#ifndef BFSUBFORMDIALOG_H
#define BFSUBFORMDIALOG_H

#include <QtWidgets/QDialog>

#include "ui_bfsubformdialogbase.h"

QT_BEGIN_NAMESPACE
class QDialogButtonBox;
QT_END_NAMESPACE
class BfSubForm;


class BfSubFormDialog : public QDialog, public Ui_BfSubFormDialogBase
{
    Q_OBJECT

public:
    BfSubFormDialog(BfSubForm *sf = 0, QWidget *parent = 0);


public slots:
    void on_mui_nuevocampo_clicked();
    void on_mui_borrarcampo_clicked();
    void on_mui_aceptar_clicked();
    void on_mui_cerrar_clicked();

};

#endif

