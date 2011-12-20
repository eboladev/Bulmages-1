#ifndef BLPROGRESSBAR_H
#define BLPROGRESSBAR_H

#include <QWidget>
#include <QtDesigner/QDesignerExportWidget>

#include <QtGui>
#include "../../src/ui_blprogressbarbase.h"


class QDESIGNER_WIDGET_EXPORT BlProgressBar : public QWidget, public Ui_BlProgressBarBase
{
    Q_OBJECT

public:
    BlProgressBar(QWidget *parent = 0);

};

#endif
