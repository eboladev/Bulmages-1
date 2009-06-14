#ifndef PDEFS_MINICONTABILIDAD_H
#define PDEFS_MINICONTABILIDAD_H


#ifdef Q_WS_WIN

#ifdef pluginbf_minicontabilidad
#define MY_EXPORT_MINICONTABILIDAD __declspec(dllexport)
#else
#define MY_EXPORT_MINICONTABILIDAD __declspec(dllimport)
#endif

#else
#define MY_EXPORT_MINICONTABILIDAD
#endif



#endif


