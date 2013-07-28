#ifndef BTINPUT_H
#define BTINPUT_H

#include <QtWidgets/QLineEdit>

#include "blfunctions.h"
#include "blwidget.h"
#include "bldb.h"
#include "blfixed.h"

class BtCompany;

class BT_EXPORT BtInput : public QLineEdit
{
    Q_OBJECT

private:
    BtCompany *m_company;

public:
    BtInput ( BtCompany * );
    virtual ~BtInput();
    virtual void keyPressEvent ( QKeyEvent *e );
    virtual void pulsaTecla ( int tecla, const QString &texto = QString() );
    QString valorBtInput();
    void setValorBtInput ( QString val );

signals:
    void sendTecla ( int tecla );
};

#endif
