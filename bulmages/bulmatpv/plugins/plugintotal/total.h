#ifndef TOTAL_H
#define TOTAL_H

#include <QWidget>
#include <QTableWidget>
#include "ui_totalbase.h"


class Total : public QWidget, public Ui_TotalBase {
    Q_OBJECT

public:
    Total(QWidget *parent);
    virtual ~Total();
};

#endif