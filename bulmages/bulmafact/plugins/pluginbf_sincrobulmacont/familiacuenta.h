#include <QtWidgets/QWidget>

#include "familiasview.h"


class FamiliaCuenta : public QWidget
{
    Q_OBJECT

private:
    FamiliasView *m_familiasview;

public:
    FamiliaCuenta(QWidget *parent);
    ~FamiliaCuenta();

public slots:
    virtual void on_m_listFamilias_currentItemChanged ( QTreeWidgetItem *current, QTreeWidgetItem *previous );
};

