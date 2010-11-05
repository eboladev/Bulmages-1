#ifndef PDEFS_PLUGINBF_ALMACEN_H
#define PDEFS_PLUGINBF_ALMACEN_H


#ifdef Q_OS_WIN32

#ifdef PLUGINBF_ALMACEN
#define PLUGINBF_ALMACEN_EXPORT __declspec(dllexport)
#else
#define PLUGINBF_ALMACEN_EXPORT __declspec(dllimport)
#endif

#else

#define PLUGINBF_ALMACEN_EXPORT

#endif


#endif

