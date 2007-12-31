#ifndef INPUT_H
#define INPUT_H


#include <QLineEdit>

#include "funcaux.h"
#include "blwidget.h"
#include "dbrecord.h"
#include "fixed.h"


class EmpresaTPV;


class Input : public QLineEdit
{
    Q_OBJECT

private:
    EmpresaTPV *m_empresaTPV;

public:
    Input ( EmpresaTPV * );
    virtual ~Input();
    virtual void keyPressEvent ( QKeyEvent *e );
    virtual void pulsaTecla ( int tecla, const QString &texto = QString() );
    QString valorInput();
    void setValorInput ( QString val );

signals:
    void sendTecla ( int tecla );
};

#endif

