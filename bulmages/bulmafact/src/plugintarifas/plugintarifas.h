#ifdef Q_WS_WIN
# define MY_EXPORT __declspec(dllexport)
#else
# define MY_EXPORT
#endif


#include "bulmafact.h"
#include "postgresiface2.h"
#include "clienteview.h"
#include "articuloview.h"

#include <qstringlist.h>
#include <qwidget.h>
#include <qicon.h>
#include <qapplication.h>
#include <qobject.h>

extern "C" MY_EXPORT void entryPoint(bulmafact *);
extern "C" MY_EXPORT int ClienteView_ClienteView(ClienteView *);
extern "C" MY_EXPORT int ClienteView_saveClient(ClienteView *);
extern "C" MY_EXPORT int Cliente_pintaCliente(Cliente *);
extern "C" MY_EXPORT int ArticuloView_ArticuloView(ArticuloView *);
extern "C" MY_EXPORT int ArticuloView_cargar(ArticuloView *);
extern "C" MY_EXPORT int ArticuloView_guardar_post(ArticuloView *);



