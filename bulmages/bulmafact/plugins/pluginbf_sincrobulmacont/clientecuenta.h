#include <QtWidgets/QWidget>

#include "clienteview.h"


class ClienteCuenta : public QWidget
{
    Q_OBJECT

    ClienteView *m_clienteview;

public:
    ClienteCuenta(QWidget *parent);
    ~ClienteCuenta();
};

