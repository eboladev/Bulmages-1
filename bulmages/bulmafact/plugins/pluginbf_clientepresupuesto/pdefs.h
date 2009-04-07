#ifndef PDEFS_H
#define PDEFS_H


#ifdef Q_WS_WIN

#ifdef pluginbf_clientepresupuesto
#define MY_EXPORT __declspec(dllexport)
#else
#define MY_EXPORT __declspec(dllimport)
#endif

#else
#define MY_EXPORT
#endif



#endif


