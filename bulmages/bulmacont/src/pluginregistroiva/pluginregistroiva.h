#ifdef Q_WS_WIN
# define MY_EXPORT __declspec(dllexport)
#else
# define MY_EXPORT
#endif


#include "bulmages01.h"
#include "postgresiface2.h"
#include "empresa.h"
#include "asiento1.h"

extern "C" MY_EXPORT void entryPoint(Bulmages01 *);
extern "C" MY_EXPORT int  Asiento1_guardaAsiento1_post(Asiento1 *);

//class QT_WIDGET_PLUGIN_EXPORT myplugin : public QObject {
class myplugin : public QObject {
Q_OBJECT
public:
  empresa *companyact;
public:
   myplugin();
   ~myplugin();
       void inicializa(Bulmages01 *);
public slots:
	void elslot();
};

