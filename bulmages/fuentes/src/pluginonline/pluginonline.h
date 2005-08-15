#ifdef Q_WS_WIN
# define MY_EXPORT __declspec(dllexport)
#else
# define MY_EXPORT
#endif


#include "bulmages01.h"
#include "postgresiface2.h"
#include "empresa.h"


#include <qwidgetplugin.h>
#include <qstringlist.h>
#include <qwidget.h>
#include <qiconset.h>
#include <qapplication.h>
#include <qobject.h>

extern "C" MY_EXPORT void entryPoint(Bulmages01 *);

extern QApplication *theApp;


class QT_WIDGET_PLUGIN_EXPORT myplugin : public QObject {
Q_OBJECT
public:
  postgresiface2 *conexionbase;
public:
   myplugin();
   ~myplugin();
       void inicializa(Bulmages01 *);
public slots:
	void elslot();
	void soporte();
};


 
