#include <QtWidgets/QWidget>

#include "bancoview.h"


class BancoCuenta : public QWidget
{
    Q_OBJECT

    BancoView *m_bancoview;

public:
    BancoCuenta(QWidget *parent);
    ~BancoCuenta();

public slots:
    virtual void mui_lista_currentItemChanged ( QListWidgetItem *current, QListWidgetItem *previous );
};

