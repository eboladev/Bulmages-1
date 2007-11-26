#ifndef TECLADONUMERICO_H
#define TECLADONUMERICO_H

#include <QWidget>
#include <QTableWidget>
#include "ui_tecladonumericobase.h"
#include "blwidget.h"
#include "empresatpv.h"

class TecladoNumerico : public BLWidget, public Ui_TecladoNumericoBase {
    Q_OBJECT

public:
    TecladoNumerico(EmpresaBase *emp, QWidget *parent);
    virtual ~TecladoNumerico();
public slots:
    virtual void on_mui_0_clicked() {
	((EmpresaTPV *)empresaBase())->pulsaTecla(Qt::Key_0);
    };
};

#endif