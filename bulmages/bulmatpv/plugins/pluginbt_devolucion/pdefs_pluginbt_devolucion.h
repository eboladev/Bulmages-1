#ifndef PDEFS_PLUGINBT_DEVOLUCION_H
#define PDEFS_PLUGINBT_DEVOLUCION_H


#ifdef Q_OS_WIN32

#ifdef PLUGINBT_DEVOLUCION
#define PLUGINBT_DEVOLUCION_EXPORT __declspec(dllexport)
#else
#define PLUGINBT_DEVOLUCION_EXPORT __declspec(dllimport)
#endif

#else

#define PLUGINBT_DEVOLUCION_EXPORT

#endif


#endif

