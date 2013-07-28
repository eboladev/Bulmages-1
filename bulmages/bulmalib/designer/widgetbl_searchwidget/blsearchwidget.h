#ifndef BLSEARCHWIDGET_H
#define BLSEARCHWIDGET_H

#include <QtWidgets/QWidget>
#include <QtDesigner/QDesignerExportWidget>

#include <QtGui/QtGui>
#include "../../src/ui_blsearchwidgetbase.h"


class QDESIGNER_WIDGET_EXPORT BlSearchWidget : public QWidget, public Ui_BlSearchWidgetBase
{
    Q_OBJECT

public:
    BlSearchWidget(QWidget *parent = 0);

};

#endif
