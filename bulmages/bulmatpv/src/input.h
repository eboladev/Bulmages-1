#ifndef INPUT_H
#define INPUT_H


#include <QObject>

#include "funcaux.h"
#include "blwidget.h"
#include "dbrecord.h"
#include "fixed.h"

class EmpresaTPV;


class Input :  public QObject {
    Q_OBJECT
private:
    QString m_valorInput;   //Contiene siempre el valor del input introducido.
    EmpresaTPV *m_empresaTPV;
public:
    Input(EmpresaTPV *);
    virtual ~Input();
    virtual void keyReleaseEvent ( QKeyEvent * e );
    virtual void pulsaTecla(int tecla);
    QString valorInput() {return m_valorInput;};
    void setValorInput(QString val) {m_valorInput = val;};

signals:
    void sendTecla(int tecla);
//    void keyUp();
};



#endif


