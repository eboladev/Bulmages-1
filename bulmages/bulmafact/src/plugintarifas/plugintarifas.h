#ifdef Q_WS_WIN
# define MY_EXPORT __declspec(dllexport)
#else
# define MY_EXPORT
#endif


#include "bulmafact.h"
#include "postgresiface2.h"

#include <qstringlist.h>
#include <qwidget.h>
#include <qicon.h>
#include <qapplication.h>
#include <qobject.h>

extern "C" MY_EXPORT void entryPoint(bulmafact *);




