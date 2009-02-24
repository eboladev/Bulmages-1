#ifdef Q_WS_WIN
# define MY_EXPORT __declspec(dllexport)
#else
# define MY_EXPORT
#endif

#include <QStringList>
#include <QWidget>
#include <QApplication>
#include <QObject>

#include <qwidgetplugin.h>
#include <qiconset.h>

#include "bulmages01.h"
#include "blpostgresqlclient.h"
#include "bccompany.h"


extern "C" MY_EXPORT void entryPoint ( Bulmages01 * );

extern QApplication *theApp;


class QT_WIDGET_PLUGIN_EXPORT myplugin : public QObject
{
    Q_OBJECT

public:
    BlPostgreSqlClient *dbConnection;
    empresa *empresaactual;

public:
    myplugin();
    ~myplugin();
    void inicializa ( Bulmages01 * );

public slots:
    virtual void SegCuentas();
    virtual void BalanceGrafico();
};
