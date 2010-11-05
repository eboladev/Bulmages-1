#ifndef PDEFS_PLUGINBT_DEVOLUCION2_H
#define PDEFS_PLUGINBT_DEVOLUCION2_H


#ifdef Q_OS_WIN32

#ifdef PLUGINBT_DEVOLUCION2
#define PLUGINBT_DEVOLUCION2_EXPORT __declspec(dllexport)
#else
#define PLUGINBT_DEVOLUCION2_EXPORT __declspec(dllimport)
#endif

#else

#define PLUGINBT_DEVOLUCION2_EXPORT

#endif


#endif

