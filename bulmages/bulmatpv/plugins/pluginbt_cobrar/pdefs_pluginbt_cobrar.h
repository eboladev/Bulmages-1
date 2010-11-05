#ifndef PDEFS_PLUGINBT_COBRAR_H
#define PDEFS_PLUGINBT_COBRAR_H


#ifdef Q_OS_WIN32

#ifdef PLUGINBT_COBRAR
#define PLUGINBT_COBRAR_EXPORT __declspec(dllexport)
#else
#define PLUGINBT_COBRAR_EXPORT __declspec(dllimport)
#endif

#else

#define PLUGINBT_COBRAR_EXPORT

#endif


#endif

