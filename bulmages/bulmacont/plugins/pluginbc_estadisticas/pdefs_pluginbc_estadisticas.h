#ifndef PDEFS_PLUGINBC_ESTADISTICAS_H
#define PDEFS_PLUGINBC_ESTADISTICAS_H


#ifdef Q_OS_WIN32

#ifdef PLUGINBC_ESTADISTICAS
#define PLUGINBC_ESTADISTICAS_EXPORT __declspec(dllexport)
#else
#define PLUGINBC_ESTADISTICAS_EXPORT __declspec(dllimport)
#endif

#else

#define PLUGINBC_ESTADISTICAS_EXPORT

#endif


#endif

