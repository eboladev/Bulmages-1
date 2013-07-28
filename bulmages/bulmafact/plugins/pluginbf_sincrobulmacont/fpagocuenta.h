#include <QtWidgets/QWidget>

#include "fpagoview.h"
#include "bfbuscarbanco.h"


class FPagoCuenta : public QWidget
{
    Q_OBJECT

    FPagoView *m_fpagoview;

public:
    FPagoCuenta(QWidget *parent);
    ~FPagoCuenta();

public slots:
    virtual void mui_lista_currentItemChanged ( QListWidgetItem *current, QListWidgetItem *previous );
    virtual void mui_idbanco_currentIndexChanged ( int );
};

