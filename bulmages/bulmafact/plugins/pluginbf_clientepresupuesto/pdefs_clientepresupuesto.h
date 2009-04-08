#ifndef PDEFS_CLIENTEPRESUPUESTO_H
#define PDEFS_CLIENTEPRESUPUESTO_H


#ifdef Q_WS_WIN

#ifdef pluginbf_clientepresupuesto
#define MY_EXPORT_CLIENTEPRESUPUESTO __declspec(dllexport)
#else
#define MY_EXPORT_CLIENTEPRESUPUESTO __declspec(dllimport)
#endif

#else
#define MY_EXPORT_CLIENTEPRESUPUESTO
#endif



#endif


