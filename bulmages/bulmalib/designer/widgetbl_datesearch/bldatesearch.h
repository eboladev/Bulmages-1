#ifndef BLDATESEARCH_H
#define BLDATESEARCH_H

#include <QtWidgets/QWidget>
#include <QtDesigner/QDesignerExportWidget>

#include <QtGui/QtGui>
#include "../../src/ui_bldatesearchbase.h"


class QDESIGNER_WIDGET_EXPORT BlDateSearch : public QWidget, public Ui_BlDateSearchBase
{
    Q_OBJECT

public:
    BlDateSearch(QWidget *parent = 0);

};

#endif
