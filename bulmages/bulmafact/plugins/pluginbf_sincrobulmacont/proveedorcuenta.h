#include <QtWidgets/QWidget>

#include "provedit.h"


class ProveedorCuenta : public QWidget
{
    Q_OBJECT

    ProveedorView *m_proveedorview;

public:
    ProveedorCuenta(QWidget *parent);
    ~ProveedorCuenta();

};

