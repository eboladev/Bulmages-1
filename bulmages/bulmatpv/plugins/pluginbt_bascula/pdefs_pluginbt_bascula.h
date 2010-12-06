#ifndef PDEFS_PLUGINBT_BASCULA_H
#define PDEFS_PLUGINBT_BASCULA_H


#ifdef Q_OS_WIN32

#ifdef PLUGINBT_BASCULA
#define PLUGINBT_BASCULA_EXPORT __declspec(dllexport)
#else
#define PLUGINBT_BASCULA_EXPORT __declspec(dllimport)
#endif

#else

#define PLUGINBT_BASCULA_EXPORT

#endif


#endif

