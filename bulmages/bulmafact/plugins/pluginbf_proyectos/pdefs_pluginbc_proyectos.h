#ifndef PDEFS_PLUGINBC_PROYECTOS_H
#define PDEFS_PLUGINBC_PROYECTOS_H


#ifdef Q_OS_WIN32

#ifdef PLUGINBC_PROYECTOS
#define PLUGINBC_PROYECTOS_EXPORT __declspec(dllexport)
#else
#define PLUGINBC_PROYECTOS_EXPORT __declspec(dllimport)
#endif

#else

#define PLUGINBC_PROYECTOS_EXPORT

#endif


#endif

