#ifndef INPUT_H
#define INPUT_H

#include <QLineEdit>

#include "blfunctions.h"
#include "blwidget.h"
#include "bldb.h"
#include "blfixed.h"


class BtCompany;


class Input : public QLineEdit
{
    Q_OBJECT

private:
    BtCompany *m_empresaTPV;

public:
    Input ( BtCompany * );
    virtual ~Input();
    virtual void keyPressEvent ( QKeyEvent *e );
    virtual void pulsaTecla ( int tecla, const QString &texto = QString() );
    QString valorInput();
    void setValorInput ( QString val );

signals:
    void sendTecla ( int tecla );
};

#endif

