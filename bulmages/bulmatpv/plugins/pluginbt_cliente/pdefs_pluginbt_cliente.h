#ifndef PDEFS_PLUGINBT_CLIENTE_H
#define PDEFS_PLUGINBT_CLIENTE_H


#ifdef Q_OS_WIN32

#ifdef PLUGINBT_CLIENTE
#define PLUGINBT_CLIENTE_EXPORT __declspec(dllexport)
#else
#define PLUGINBT_CLIENTE_EXPORT __declspec(dllimport)
#endif

#else

#define PLUGINBT_CLIENTE_EXPORT

#endif


#endif

