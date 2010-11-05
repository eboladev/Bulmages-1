#ifndef PDEFS_PLUGINBT_CAMAREROS_H
#define PDEFS_PLUGINBT_CAMAREROS_H


#ifdef Q_OS_WIN32

#ifdef PLUGINBT_CAMAREROS
#define PLUGINBT_CAMAREROS_EXPORT __declspec(dllexport)
#else
#define PLUGINBT_CAMAREROS_EXPORT __declspec(dllimport)
#endif

#else

#define PLUGINBT_CAMAREROS_EXPORT

#endif


#endif

