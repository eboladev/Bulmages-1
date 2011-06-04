#ifndef PDEFS_PLUGINBT_PULSERAS_H
#define PDEFS_PLUGINBT_PULSERAS_H


#ifdef Q_OS_WIN32

#ifdef PLUGINBT_PULSERAS
#define PLUGINBT_PULSERAS_EXPORT __declspec(dllexport)
#else
#define PLUGINBT_PULSERAS_EXPORT __declspec(dllimport)
#endif

#else

#define PLUGINBT_PULSERAS_EXPORT

#endif


#endif

