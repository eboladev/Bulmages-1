#ifdef Q_WS_WIN
# define MY_EXPORT __declspec(dllexport)
#else
# define MY_EXPORT
#endif

#include "postgresiface2.h"
#include "clienteview.h"
#include "bulmafact.h"

#include <qstringlist.h>
#include <qwidget.h>
#include <qicon.h>
#include <qapplication.h>
#include <qobject.h>

extern "C" MY_EXPORT int entryPoint(bulmafact *);
extern "C" MY_EXPORT int ClienteView_ClienteView_Post(ClienteView *);
extern "C" MY_EXPORT int ClienteView_Des_ClienteView(ClienteView *);
extern "C" MY_EXPORT int ClienteView_saveClient(ClienteView *);
extern "C" MY_EXPORT int Cliente_pintaCliente(Cliente *);


