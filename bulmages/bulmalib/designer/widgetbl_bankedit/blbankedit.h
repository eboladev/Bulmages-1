#ifndef BLBANKEDIT_H
#define BLBANKEDIT_H

#include <QtWidgets/QWidget>
#include <QtDesigner/QDesignerExportWidget>

#include <QtGui/QtGui>
#include "../../src/ui_blbankeditbase.h"


class QDESIGNER_WIDGET_EXPORT BlBankEdit : public QWidget, public Ui_BlBankEditBase
{
    Q_OBJECT

public:
    BlBankEdit(QWidget *parent = 0);

};

#endif
