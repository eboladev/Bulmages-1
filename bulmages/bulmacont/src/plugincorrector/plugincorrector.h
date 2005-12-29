#ifdef Q_WS_WIN
# define MY_EXPORT __declspec(dllexport)
#else
# define MY_EXPORT
#endif


#include "bulmages01.h"
#include "postgresiface2.h"
#include "empresa.h"




extern "C" MY_EXPORT void entryPoint(Bulmages01 *);
