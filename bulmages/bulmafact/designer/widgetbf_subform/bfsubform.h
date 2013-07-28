#ifndef BFSUBFORM_H
#define BFSUBFORM_H

#include <QtWidgets/QTableWidget>
#include <QtDesigner/QDesignerExportWidget>

#include <QtGui/QtGui>


class QDESIGNER_WIDGET_EXPORT BfSubForm : public QTableWidget
{
    Q_OBJECT

public:
    BfSubForm(QWidget *parent = 0);

};

#endif
