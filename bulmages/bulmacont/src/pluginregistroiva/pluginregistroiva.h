#ifdef Q_WS_WIN
# define MY_EXPORT __declspec(dllexport)
#else
# define MY_EXPORT
#endif


#include "bulmages01.h"
#include "postgresiface2.h"
#include "empresa.h"
#include "asiento1.h"
#include "listlinasiento1view.h"

extern "C" MY_EXPORT int entryPoint(Bulmages01 *);
extern "C" MY_EXPORT int  Asiento1_guardaAsiento1_post(Asiento1 *);
extern "C" MY_EXPORT int  empresa_cobPag(empresa *);
extern "C" MY_EXPORT int  empresa_registroiva(empresa *);
extern "C" MY_EXPORT int  ListLinAsiento1View_boton_iva(ListLinAsiento1View *);




