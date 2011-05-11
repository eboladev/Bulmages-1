#ifdef Q_WS_WIN
# define MY_EXPORT __declspec(dllexport)
#else
# define MY_EXPORT
#endif

#include "bulmages01.h"
#include "blpostgresqlclient.h"
#include "bccompany.h"

//#include <qwidgetplugin.h>
#include <QPluginLoader>
#include <qstringlist.h>
#include <qwidget.h>
#include <qicon.h>
#include <qapplication.h>
#include <qobject.h>


extern "C" MY_EXPORT int entryPoint ( Bulmages01 * );
extern QApplication *g_theApp;


//class QT_WIDGET_PLUGIN_EXPORT PluginBc_Online : public QObject {
class PluginBc_Online : public QObject
{
    Q_OBJECT

public:
    BlPostgreSqlClient *dbConnection;

public:
    PluginBc_Online();
    ~PluginBc_Online();
    void inicializa ( Bulmages01 * );

public slots:
    void elslot();
    void soporte();
};




